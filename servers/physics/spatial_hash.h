/**
 * @file spatial_hash.h
 * @brief Spatial partitioning using 3D grid-based hashing
 *
 * This header implements a spatial hash data structure for efficient
 * physics queries. Instead of O(n²) collision checks, queries run in
 * O(k) where k is the average number of objects per cell.
 *
 * The grid divides 3D space into uniform cells. Objects are placed in
 * cells based on their AABB. Queries check only cells that intersect
 * the query region.
 */

#ifndef SPATIAL_HASH_H
#define SPATIAL_HASH_H

#include "core/hash_map.h"
#include "core/math/aabb.h"
#include "core/math/vector3.h"
#include "core/object_id.h"
#include "core/rid.h"
#include "core/vector.h"

/**
 * @class SpatialHashGrid
 * @brief 3D spatial hash for efficient collision/proximity queries
 *
 * Grid size is configurable but typically 32-64 units per cell.
 * Objects can occupy multiple cells if their AABB spans cells.
 */
class SpatialHashGrid {
public:
  // Default cell size (32 units)
  static constexpr float DEFAULT_CELL_SIZE = 32.0f;

  // Maximum cells to check in a query (prevents runaway queries)
  static constexpr int32_t MAX_QUERY_CELLS = 1000;

private:
  struct Cell {
    Vector<RID> bodies; // Physical bodies in this cell
    Vector<RID> areas;  // Areas/triggers in this cell
  };

  struct GridKey {
    int32_t x, y, z;

    bool operator==(const GridKey &p_other) const {
      return x == p_other.x && y == p_other.y && z == p_other.z;
    }

    uint32_t hash() const {
      // 3D hash using bit interleaving (Morton code variant)
      uint32_t h = 0;
      h ^= ((uint32_t)x * 73856093U);
      h ^= ((uint32_t)y * 19349663U);
      h ^= ((uint32_t)z * 83492791U);
      return h;
    }
  };

  float cell_size;
  HashMap<GridKey, Cell> cells;

  // Convert world position to grid coordinates
  GridKey world_to_grid(const Vector3 &p_pos) const {
    return GridKey{(int32_t)floor(p_pos.x / cell_size),
                   (int32_t)floor(p_pos.y / cell_size),
                   (int32_t)floor(p_pos.z / cell_size)};
  }

  // Get all grid cells that contain a given AABB
  Vector<GridKey> get_aabb_cells(const AABB &p_aabb) const {
    Vector<GridKey> result;

    GridKey min_key = world_to_grid(p_aabb.position);
    GridKey max_key = world_to_grid(p_aabb.position + p_aabb.size);

    for (int32_t x = min_key.x; x <= max_key.x; ++x) {
      for (int32_t y = min_key.y; y <= max_key.y; ++y) {
        for (int32_t z = min_key.z; z <= max_key.z; ++z) {
          result.push_back(GridKey{x, y, z});

          // Prevent pathological cases (huge AABB)
          if (result.size() >= MAX_QUERY_CELLS) {
            return result;
          }
        }
      }
    }

    return result;
  }

public:
  SpatialHashGrid(float p_cell_size = DEFAULT_CELL_SIZE)
      : cell_size(p_cell_size) {
    ERR_FAIL_COND(cell_size <= 0.0f);
  }

  /**
   * Insert a body with given AABB into the spatial hash
   *
   * @param p_rid Unique identifier for the body
   * @param p_aabb Axis-aligned bounding box in world space
   * @param p_is_area Whether this is an area (trigger) or physical body
   */
  void insert(const RID &p_rid, const AABB &p_aabb, bool p_is_area = false) {
    Vector<GridKey> cells_to_update = get_aabb_cells(p_aabb);

    for (const GridKey &key : cells_to_update) {
      Cell &cell = cells[key];
      if (p_is_area) {
        cell.areas.push_back(p_rid);
      } else {
        cell.bodies.push_back(p_rid);
      }
    }
  }

  /**
   * Remove a body from the spatial hash
   * Must provide same AABB that was used during insert
   */
  void remove(const RID &p_rid, const AABB &p_aabb, bool p_is_area = false) {
    Vector<GridKey> cells_to_update = get_aabb_cells(p_aabb);

    for (const GridKey &key : cells_to_update) {
      if (cells.has(key)) {
        Cell &cell = cells[key];
        Vector<RID> &target = p_is_area ? cell.areas : cell.bodies;
        target.erase(p_rid);
      }
    }
  }

  /**
   * Update an object's position in the hash
   *
   * @note For best performance, call this once per frame after physics updates
   */
  void update(const RID &p_rid, const AABB &p_old_aabb, const AABB &p_new_aabb,
              bool p_is_area = false) {
    // Only update if AABB significantly changed
    if (p_old_aabb != p_new_aabb) {
      remove(p_rid, p_old_aabb, p_is_area);
      insert(p_rid, p_new_aabb, p_is_area);
    }
  }

  /**
   * Query: Find all bodies/areas intersecting a region
   *
   * @param p_aabb Query region in world space
   * @param p_exclude RIDs to skip in results
   * @param p_bodies Optional - filled with intersecting bodies
   * @param p_areas Optional - filled with intersecting areas
   *
   * @return Combined count of all results
   */
  int query_aabb(const AABB &p_aabb, const Set<RID> &p_exclude,
                 Vector<RID> *p_bodies = nullptr,
                 Vector<RID> *p_areas = nullptr) const {
    Vector<GridKey> query_cells = get_aabb_cells(p_aabb);
    Set<RID> visited_bodies, visited_areas;
    int total_results = 0;

    for (const GridKey &key : query_cells) {
      if (cells.has(key)) {
        const Cell &cell = cells[key];

        // Add bodies (dedup if object spans multiple cells)
        for (const RID &body_rid : cell.bodies) {
          if (!p_exclude.has(body_rid) && !visited_bodies.has(body_rid)) {
            if (p_bodies) {
              p_bodies->push_back(body_rid);
            }
            visited_bodies.insert(body_rid);
            total_results++;
          }
        }

        // Add areas (dedup if object spans multiple cells)
        for (const RID &area_rid : cell.areas) {
          if (!p_exclude.has(area_rid) && !visited_areas.has(area_rid)) {
            if (p_areas) {
              p_areas->push_back(area_rid);
            }
            visited_areas.insert(area_rid);
            total_results++;
          }
        }
      }
    }

    return total_results;
  }

  /**
   * Ray cast query (simplified)
   * @note For production, implement proper ray-cell traversal (DDA)
   */
  int query_ray(const Vector3 &p_from, const Vector3 &p_to,
                const Set<RID> &p_exclude, Vector<RID> &p_results) const {
    // Approximate ray as thin AABB
    AABB ray_box(
        Vector3(fmin(p_from.x, p_to.x) - 0.1f, fmin(p_from.y, p_to.y) - 0.1f,
                fmin(p_from.z, p_to.z) - 0.1f),
        Vector3(fabs(p_to.x - p_from.x) + 0.2f, fabs(p_to.y - p_from.y) + 0.2f,
                fabs(p_to.z - p_from.z) + 0.2f));

    return query_aabb(ray_box, p_exclude, &p_results);
  }

  /**
   * Get grid statistics for debugging/optimization
   */
  struct Stats {
    int32_t total_cells = 0;
    int32_t occupied_cells = 0;
    int32_t total_bodies = 0;
    int32_t total_areas = 0;
    float avg_bodies_per_cell = 0.0f;
    float avg_areas_per_cell = 0.0f;
  };

  Stats get_stats() const {
    Stats stats;
    stats.total_cells = cells.size();
    stats.occupied_cells = cells.size();

    int total_body_count = 0;
    int total_area_count = 0;

    for (const auto &cell_pair : cells) {
      total_body_count += cell_pair.second.bodies.size();
      total_area_count += cell_pair.second.areas.size();
    }

    stats.total_bodies = total_body_count;
    stats.total_areas = total_area_count;

    if (stats.occupied_cells > 0) {
      stats.avg_bodies_per_cell =
          (float)total_body_count / stats.occupied_cells;
      stats.avg_areas_per_cell = (float)total_area_count / stats.occupied_cells;
    }

    return stats;
  }

  void clear() { cells.clear(); }

  void set_cell_size(float p_size) {
    ERR_FAIL_COND(p_size <= 0.0f);
    if (p_size != cell_size) {
      // Warn about need to rebuild
      print_warning("SpatialHashGrid: Cell size changed, hash needs rebuild");
      cell_size = p_size;
      clear();
    }
  }

  float get_cell_size() const { return cell_size; }
};

#endif // SPATIAL_HASH_H

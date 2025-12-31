
/**
 * @class AStar
 * @brief Implements the A* pathfinding algorithm for 3D space.
 *
 * This class provides functionality to create a graph of points in 3D space and
 * find optimal paths between them using the A* pathfinding algorithm. Points
 * can be connected bidirectionally or unidirectionally, and can have individual
 * weight scales to influence pathfinding costs.
 *
 * The algorithm uses heuristic-based cost estimation to efficiently find the
 * shortest path between two points. Points can be enabled or disabled to
 * dynamically modify the graph.
 *
 * @note For 2D pathfinding, use the AStar2D class instead.
 * @author Juan Linietsky <reduzio@gmail.com>
 *
 * @method int get_available_point_id() const
 *         Returns the next available point ID that hasn't been used yet.
 *
 * @method void add_point(int p_id, const Vector3 &p_pos, real_t p_weight_scale
 * = 1) Adds a new point to the pathfinding graph at the given position.
 *         @param p_id Unique identifier for the point
 *         @param p_pos 3D position of the point
 *         @param p_weight_scale Weight multiplier for pathfinding cost
 * (default 1.0)
 *
 * @method Vector3 get_point_position(int p_id) const
 *         Returns the 3D position of a point.
 *
 * @method void set_point_position(int p_id, const Vector3 &p_pos)
 *         Updates the 3D position of an existing point.
 *
 * @method real_t get_point_weight_scale(int p_id) const
 *         Returns the weight scale multiplier of a point.
 *
 * @method void set_point_weight_scale(int p_id, real_t p_weight_scale)
 *         Updates the weight scale multiplier of a point.
 *
 * @method void remove_point(int p_id)
 *         Removes a point from the graph.
 *
 * @method bool has_point(int p_id) const
 *         Checks if a point with the given ID exists.
 *
 * @method Vector<int> get_point_connections(int p_id)
 *         Returns a list of all connected point IDs.
 *
 * @method Array get_points()
 *         Returns an array of all points in the graph.
 *
 * @method void set_point_disabled(int p_id, bool p_disabled = true)
 *         Enables or disables a point, excluding it from pathfinding.
 *
 * @method bool is_point_disabled(int p_id) const
 *         Checks if a point is currently disabled.
 *
 * @method void connect_points(int p_id, int p_with_id, bool bidirectional =
 * true) Creates a connection between two points.
 *         @param bidirectional If true, creates bidirectional connection;
 * otherwise unidirectional.
 *
 * @method void disconnect_points(int p_id, int p_with_id, bool bidirectional =
 * true) Removes a connection between two points.
 *
 * @method bool are_points_connected(int p_id, int p_with_id, bool bidirectional
 * = true) const Checks if two points are connected.
 *
 * @method int get_point_count() const
 *         Returns the number of points in the graph.
 *
 * @method int get_point_capacity() const
 *         Returns the current capacity of the points container.
 *
 * @method void reserve_space(int p_num_nodes)
 *         Pre-allocates space for a given number of nodes.
 *
 * @method void clear()
 *         Removes all points and connections from the graph.
 *
 * @method int get_closest_point(const Vector3 &p_point, bool p_include_disabled
 * = false) const Finds the closest point to a given position.
 *
 * @method Vector3 get_closest_position_in_segment(const Vector3 &p_point) const
 *         Returns the closest position on any segment between connected points.
 *
 * @method Vector<Vector3> get_point_path(int p_from_id, int p_to_id)
 *         Computes the path between two points as a vector of 3D positions.
 *         @return Vector of positions from start to end, or empty if no path
 * exists.
 *
 * @method Vector<int> get_id_path(int p_from_id, int p_to_id)
 *         Computes the path between two points as a vector of point IDs.
 *         @return Vector of point IDs from start to end, or empty if no path
 * exists.
 */

/**
 * @class AStar2D
 * @brief Implements the A* pathfinding algorithm for 2D space.
 *
 * A 2D variant of the AStar class that operates on Vector2 positions instead of
 * Vector3. Provides the same pathfinding functionality adapted for
 * two-dimensional space.
 *
 * This class wraps an internal AStar instance and adapts all methods to work
 * with 2D coordinates. All other behaviors and features are identical to the 3D
 * version.
 *
 * @see AStar for the 3D version of this class.
 *
 * @method int get_available_point_id() const
 *         Returns the next available point ID that hasn't been used yet.
 *
 * @method void add_point(int p_id, const Vector2 &p_pos, real_t p_weight_scale
 * = 1) Adds a new point to the pathfinding graph at the given 2D position.
 *         @param p_id Unique identifier for the point
 *         @param p_pos 2D position of the point
 *         @param p_weight_scale Weight multiplier for pathfinding cost
 * (default 1.0)
 *
 * @method Vector2 get_point_position(int p_id) const
 *         Returns the 2D position of a point.
 *
 * @method void set_point_position(int p_id, const Vector2 &p_pos)
 *         Updates the 2D position of an existing point.
 *
 * @method real_t get_point_weight_scale(int p_id) const
 *         Returns the weight scale multiplier of a point.
 *
 * @method void set_point_weight_scale(int p_id, real_t p_weight_scale)
 *         Updates the weight scale multiplier of a point.
 *
 * @method void remove_point(int p_id)
 *         Removes a point from the graph.
 *
 * @method bool has_point(int p_id) const
 *         Checks if a point with the given ID exists.
 *
 * @method Vector<int> get_point_connections(int p_id)
 *         Returns a list of all connected point IDs.
 *
 * @method Array get_points()
 *         Returns an array of all points in the graph.
 *
 * @method void set_point_disabled(int p_id, bool p_disabled = true)
 *         Enables or disables a point, excluding it from pathfinding.
 *
 * @method bool is_point_disabled(int p_id) const
 *         Checks if a point is currently disabled.
 *
 * @method void connect_points(int p_id, int p_with_id, bool p_bidirectional =
 * true) Creates a connection between two points.
 *         @param p_bidirectional If true, creates bidirectional connection;
 * otherwise unidirectional.
 *
 * @method void disconnect_points(int p_id, int p_with_id)
 *         Removes a connection between two points.
 *
 * @method bool are_points_connected(int p_id, int p_with_id) const
 *         Checks if two points are connected.
 *
 * @method int get_point_count() const
 *         Returns the number of points in the graph.
 *
 * @method int get_point_capacity() const
 *         Returns the current capacity of the points container.
 *
 * @method void reserve_space(int p_num_nodes)
 *         Pre-allocates space for a given number of nodes.
 *
 * @method void clear()
 *         Removes all points and connections from the graph.
 *
 * @method int get_closest_point(const Vector2 &p_point, bool p_include_disabled
 * = false) const Finds the closest point to a given 2D position.
 *
 * @method Vector2 get_closest_position_in_segment(const Vector2 &p_point) const
 *         Returns the closest position on any segment between connected points.
 *
 * @method Vector<Vector2> get_point_path(int p_from_id, int p_to_id)
 *         Computes the path between two points as a vector of 2D positions.
 *         @return Vector of positions from start to end, or empty if no path
 * exists.
 *
 * @method Vector<int> get_id_path(int p_from_id, int p_to_id)
 *         Computes the path between two points as a vector of point IDs.
 *         @return Vector of point IDs from start to end, or empty if no path
 * exists.
 */
#ifndef A_STAR_H
#define A_STAR_H

#include "core/oa_hash_map.h"
#include "core/reference.h"

/**
        A* pathfinding algorithm

        @author Juan Linietsky <reduzio@gmail.com>
*/

class AStar : public Reference {

  GDCLASS(AStar, Reference);
  friend class AStar2D;

  struct Point {

    Point() : neighbours(4u), unlinked_neighbours(4u) {}

    int id;
    Vector3 pos;
    real_t weight_scale;
    bool enabled;

    OAHashMap<int, Point *> neighbours;
    OAHashMap<int, Point *> unlinked_neighbours;

    // Used for pathfinding.
    Point *prev_point;
    real_t g_score;
    real_t f_score;
    uint64_t open_pass;
    uint64_t closed_pass;
  };

  struct SortPoints {
    _FORCE_INLINE_ bool operator()(const Point *A, const Point *B)
        const { // Returns true when the Point A is worse than Point B.
      if (A->f_score > B->f_score) {
        return true;
      } else if (A->f_score < B->f_score) {
        return false;
      } else {
        return A->g_score <
               B->g_score; // If the f_costs are the same then prioritize the
                           // points that are further away from the start.
      }
    }
  };

  struct Segment {
    union {
      struct {
        int32_t u;
        int32_t v;
      };
      uint64_t key;
    };

    enum {
      NONE = 0,
      FORWARD = 1,
      BACKWARD = 2,
      BIDIRECTIONAL = FORWARD | BACKWARD
    };
    unsigned char direction;

    bool operator<(const Segment &p_s) const { return key < p_s.key; }
    Segment() {
      key = 0;
      direction = NONE;
    }
    Segment(int p_from, int p_to) {
      if (p_from < p_to) {
        u = p_from;
        v = p_to;
        direction = FORWARD;
      } else {
        u = p_to;
        v = p_from;
        direction = BACKWARD;
      }
    }
  };

  int last_free_id;
  uint64_t pass;

  OAHashMap<int, Point *> points;
  Set<Segment> segments;

  bool _solve(Point *begin_point, Point *end_point);

protected:
  static void _bind_methods();

  virtual real_t _estimate_cost(int p_from_id, int p_to_id);
  virtual real_t _compute_cost(int p_from_id, int p_to_id);

public:
  int get_available_point_id() const;

  void add_point(int p_id, const Vector3 &p_pos, real_t p_weight_scale = 1);
  Vector3 get_point_position(int p_id) const;
  void set_point_position(int p_id, const Vector3 &p_pos);
  real_t get_point_weight_scale(int p_id) const;
  void set_point_weight_scale(int p_id, real_t p_weight_scale);
  void remove_point(int p_id);
  bool has_point(int p_id) const;
  Vector<int> get_point_connections(int p_id);
  Array get_points();

  void set_point_disabled(int p_id, bool p_disabled = true);
  bool is_point_disabled(int p_id) const;

  void connect_points(int p_id, int p_with_id, bool bidirectional = true);
  void disconnect_points(int p_id, int p_with_id, bool bidirectional = true);
  bool are_points_connected(int p_id, int p_with_id,
                            bool bidirectional = true) const;

  int get_point_count() const;
  int get_point_capacity() const;
  void reserve_space(int p_num_nodes);
  void clear();

  int get_closest_point(const Vector3 &p_point,
                        bool p_include_disabled = false) const;
  Vector3 get_closest_position_in_segment(const Vector3 &p_point) const;

  Vector<Vector3> get_point_path(int p_from_id, int p_to_id);
  Vector<int> get_id_path(int p_from_id, int p_to_id);

  AStar();
  ~AStar();
};

class AStar2D : public Reference {
  GDCLASS(AStar2D, Reference);
  AStar astar;

  bool _solve(AStar::Point *begin_point, AStar::Point *end_point);

protected:
  static void _bind_methods();

  virtual real_t _estimate_cost(int p_from_id, int p_to_id);
  virtual real_t _compute_cost(int p_from_id, int p_to_id);

public:
  int get_available_point_id() const;

  void add_point(int p_id, const Vector2 &p_pos, real_t p_weight_scale = 1);
  Vector2 get_point_position(int p_id) const;
  void set_point_position(int p_id, const Vector2 &p_pos);
  real_t get_point_weight_scale(int p_id) const;
  void set_point_weight_scale(int p_id, real_t p_weight_scale);
  void remove_point(int p_id);
  bool has_point(int p_id) const;
  Vector<int> get_point_connections(int p_id);
  Array get_points();

  void set_point_disabled(int p_id, bool p_disabled = true);
  bool is_point_disabled(int p_id) const;

  void connect_points(int p_id, int p_with_id, bool p_bidirectional = true);
  void disconnect_points(int p_id, int p_with_id);
  bool are_points_connected(int p_id, int p_with_id) const;

  int get_point_count() const;
  int get_point_capacity() const;
  void reserve_space(int p_num_nodes);
  void clear();

  int get_closest_point(const Vector2 &p_point,
                        bool p_include_disabled = false) const;
  Vector2 get_closest_position_in_segment(const Vector2 &p_point) const;

  Vector<Vector2> get_point_path(int p_from_id, int p_to_id);
  Vector<int> get_id_path(int p_from_id, int p_to_id);

  AStar2D();
  ~AStar2D();
};

#endif // A_STAR_H

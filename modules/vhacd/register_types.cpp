
#include "register_types.h"
#include "scene/resources/mesh.h"
#include "thirdparty/vhacd/public/VHACD.h"

static Vector<Vector<Face3>> convex_decompose(const Vector<Face3> &p_faces) {

  Vector<float> vertices;
  vertices.resize(p_faces.size() * 9);
  Vector<uint32_t> indices;
  indices.resize(p_faces.size() * 3);

  for (int i = 0; i < p_faces.size(); i++) {
    for (int j = 0; j < 3; j++) {
      vertices.write[i * 9 + j * 3 + 0] = p_faces[i].vertex[j].x;
      vertices.write[i * 9 + j * 3 + 1] = p_faces[i].vertex[j].y;
      vertices.write[i * 9 + j * 3 + 2] = p_faces[i].vertex[j].z;
      indices.write[i * 3 + j] = i * 3 + j;
    }
  }

  VHACD::IVHACD *decomposer = VHACD::CreateVHACD();
  VHACD::IVHACD::Parameters params;
  decomposer->Compute(vertices.ptr(), vertices.size() / 3, indices.ptr(),
                      indices.size() / 3, params);

  int hull_count = decomposer->GetNConvexHulls();

  Vector<Vector<Face3>> ret;

  for (int i = 0; i < hull_count; i++) {
    Vector<Face3> triangles;
    VHACD::IVHACD::ConvexHull hull;
    decomposer->GetConvexHull(i, hull);
    triangles.resize(hull.m_nTriangles);
    for (uint32_t j = 0; j < hull.m_nTriangles; j++) {
      Face3 f;
      for (int k = 0; k < 3; k++) {
        for (int l = 0; l < 3; l++) {
          f.vertex[k][l] = hull.m_points[hull.m_triangles[j * 3 + k] * 3 + l];
        }
      }
      triangles.write[j] = f;
    }
    ret.push_back(triangles);
  }

  decomposer->Clean();
  decomposer->Release();

  return ret;
}

void register_vhacd_types() {
  Mesh::convex_composition_function = convex_decompose;
}

void unregister_vhacd_types() { Mesh::convex_composition_function = NULL; }

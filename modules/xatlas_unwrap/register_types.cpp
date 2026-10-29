
#include "register_types.h"

#include "core/error_macros.h"

#include "thirdparty/xatlas/xatlas.h"

#include <stdio.h>
#include <stdlib.h>

extern bool (*array_mesh_lightmap_unwrap_callback)(
    float p_texel_size, const float *p_vertices, const float *p_normals,
    int p_vertex_count, const int *p_indices, const int *p_face_materials,
    int p_index_count, float **r_uv, int **r_vertex, int *r_vertex_count,
    int **r_index, int *r_index_count, int *r_size_hint_x, int *r_size_hint_y);

bool xatlas_mesh_lightmap_unwrap_callback(
    float p_texel_size, const float *p_vertices, const float *p_normals,
    int p_vertex_count, const int *p_indices, const int *p_face_materials,
    int p_index_count, float **r_uv, int **r_vertex, int *r_vertex_count,
    int **r_index, int *r_index_count, int *r_size_hint_x, int *r_size_hint_y) {

  // set up input mesh
  xatlas::MeshDecl input_mesh;
  input_mesh.indexData = p_indices;
  input_mesh.indexCount = p_index_count;
  input_mesh.indexFormat = xatlas::IndexFormat::UInt32;

  input_mesh.vertexCount = p_vertex_count;
  input_mesh.vertexPositionData = p_vertices;
  input_mesh.vertexPositionStride = sizeof(float) * 3;
  input_mesh.vertexNormalData = p_normals;
  input_mesh.vertexNormalStride = sizeof(uint32_t) * 3;
  input_mesh.vertexUvData = NULL;
  input_mesh.vertexUvStride = 0;

  xatlas::ChartOptions chart_options;
  xatlas::PackOptions pack_options;

  pack_options.maxChartSize = 4096;
  pack_options.blockAlign = true;
  pack_options.texelsPerUnit = 1.0 / p_texel_size;

  xatlas::Atlas *atlas = xatlas::Create();
  printf("Adding mesh..\n");
  xatlas::AddMeshError::Enum err = xatlas::AddMesh(atlas, input_mesh, 1);
  ERR_FAIL_COND_V_MSG(err != xatlas::AddMeshError::Enum::Success, false,
                      xatlas::StringForEnum(err));

  printf("Generate..\n");
  xatlas::Generate(atlas, chart_options, NULL, pack_options);

  *r_size_hint_x = atlas->width;
  *r_size_hint_y = atlas->height;

  float w = *r_size_hint_x;
  float h = *r_size_hint_y;

  if (w == 0 || h == 0) {
    return false; // could not bake because there is no area
  }

  const xatlas::Mesh &output = atlas->meshes[0];

  *r_vertex = (int *)malloc(sizeof(int) * output.vertexCount);
  *r_uv = (float *)malloc(sizeof(float) * output.vertexCount * 2);
  *r_index = (int *)malloc(sizeof(int) * output.indexCount);

  float max_x = 0;
  float max_y = 0;
  for (uint32_t i = 0; i < output.vertexCount; i++) {
    (*r_vertex)[i] = output.vertexArray[i].xref;
    (*r_uv)[i * 2 + 0] = output.vertexArray[i].uv[0] / w;
    (*r_uv)[i * 2 + 1] = output.vertexArray[i].uv[1] / h;
    max_x = MAX(max_x, output.vertexArray[i].uv[0]);
    max_y = MAX(max_y, output.vertexArray[i].uv[1]);
  }

  printf("Final texture size: %f,%f - max %f,%f\n", w, h, max_x, max_y);
  *r_vertex_count = output.vertexCount;

  for (uint32_t i = 0; i < output.indexCount; i++) {
    (*r_index)[i] = output.indexArray[i];
  }

  *r_index_count = output.indexCount;

  xatlas::Destroy(atlas);
  printf("Done\n");
  return true;
}

void register_xatlas_unwrap_types() {

  array_mesh_lightmap_unwrap_callback = xatlas_mesh_lightmap_unwrap_callback;
}

void unregister_xatlas_unwrap_types() {}

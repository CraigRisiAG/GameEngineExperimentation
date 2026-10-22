
#ifndef EDITOR_SCENE_IMPORT_STATE_H
#define EDITOR_SCENE_IMPORT_STATE_H

#include "core/bind/core_bind.h"
#include "core/io/resource_importer.h"
#include "core/vector.h"
#include "editor/import/resource_importer_scene.h"
#include "editor/project_settings_editor.h"
#include "scene/3d/mesh_instance.h"
#include "scene/3d/skeleton.h"
#include "scene/3d/spatial.h"
#include "scene/animation/animation_player.h"
#include "scene/resources/animation.h"
#include "scene/resources/surface_tool.h"

#include <assimp/DefaultLogger.hpp>
#include <assimp/LogStream.hpp>
#include <assimp/Logger.hpp>
#include <assimp/matrix4x4.h>
#include <assimp/scene.h>
#include <assimp/types.h>

namespace AssimpImporter {
/** Import state is for global scene import data
 * This makes the code simpler and contains useful lookups.
 */
struct ImportState {

  String path;
  Spatial *root;
  const aiScene *assimp_scene;
  uint32_t max_bone_weights;

  Map<String, Ref<Mesh>> mesh_cache;
  Map<int, Ref<Material>> material_cache;
  Map<String, int> light_cache;
  Map<String, int> camera_cache;

  // very useful for when you need to ask assimp for the bone mesh

  Map<const aiNode *, Node *> assimp_node_map;
  Map<String, Ref<Image>> path_to_image_cache;

  // Generation 3 - determinisitic iteration
  // to lower potential recursion errors
  List<const aiNode *> nodes;
  Map<const aiNode *, Spatial *> flat_node_map;
  AnimationPlayer *animation_player;

  // Generation 3 - deterministic armatures
  // list of armature nodes - flat and simple to parse
  // assimp node, node in godot
  List<aiNode *> armature_nodes;
  Map<const aiNode *, Skeleton *> armature_skeletons;
  Map<aiBone *, Skeleton *> skeleton_bone_map;
  // Generation 3 - deterministic bone handling
  // bones from the stack are popped when found
  // this means we can detect
  // what bones are for other armatures
  List<aiBone *> bone_stack;
};

struct AssimpImageData {
  Ref<Image> raw_image;
  Ref<ImageTexture> texture;
  aiTextureMapMode map_mode[2];
};

/** Recursive state is used to push state into functions instead of specifying
 * them This makes the code easier to handle too and add extra arguments without
 * breaking things
 */
struct RecursiveState {
  RecursiveState() {} // do not construct :)
  RecursiveState(Transform &_node_transform, Skeleton *_skeleton,
                 Spatial *_new_node, String &_node_name, aiNode *_assimp_node,
                 Node *_parent_node, aiBone *_bone)
      : node_transform(_node_transform), skeleton(_skeleton),
        new_node(_new_node), node_name(_node_name), assimp_node(_assimp_node),
        parent_node(_parent_node), bone(_bone) {}

  Transform node_transform;
  Skeleton *skeleton = NULL;
  Spatial *new_node = NULL;
  String node_name;
  aiNode *assimp_node = NULL;
  Node *parent_node = NULL;
  aiBone *bone = NULL;
};
} // namespace AssimpImporter

#endif // EDITOR_SCENE_IMPORT_STATE_H

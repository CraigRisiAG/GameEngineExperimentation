
#include "register_types.h"

#include "editor/editor_node.h"
#include "editor_scene_importer_assimp.h"

#ifdef TOOLS_ENABLED
static void _editor_init() {
  Ref<EditorSceneImporterAssimp> import_assimp;
  import_assimp.instance();
  ResourceImporterScene::get_singleton()->add_importer(import_assimp);
}
#endif

void register_assimp_types() {

#ifdef TOOLS_ENABLED
  ClassDB::APIType prev_api = ClassDB::get_current_api();
  ClassDB::set_current_api(ClassDB::API_EDITOR);

  ClassDB::register_class<EditorSceneImporterAssimp>();

  ClassDB::set_current_api(prev_api);

  EditorNode::add_init_callback(_editor_init);
#endif
}

void unregister_assimp_types() {}

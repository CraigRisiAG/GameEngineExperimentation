
#include "register_types.h"

#include "core/engine.h"
#include "gd_navigation_server.h"
#include "servers/navigation_server.h"

#ifndef _3D_DISABLED
#include "navigation_mesh_generator.h"
#endif

#ifdef TOOLS_ENABLED
#include "navigation_mesh_editor_plugin.h"
#endif

/**
        @author AndreaCatania
*/

#ifndef _3D_DISABLED
NavigationMeshGenerator *_nav_mesh_generator = NULL;
#endif

NavigationServer *new_server() { return memnew(GdNavigationServer); }

void register_gdnavigation_types() {
  NavigationServerManager::set_default_server(new_server);

#ifndef _3D_DISABLED
  _nav_mesh_generator = memnew(NavigationMeshGenerator);
  ClassDB::register_class<NavigationMeshGenerator>();
  Engine::get_singleton()->add_singleton(Engine::Singleton(
      "NavigationMeshGenerator", NavigationMeshGenerator::get_singleton()));
#endif

#ifdef TOOLS_ENABLED
  EditorPlugins::add_by_type<NavigationMeshEditorPlugin>();

  ClassDB::APIType prev_api = ClassDB::get_current_api();
  ClassDB::set_current_api(ClassDB::API_EDITOR);

  ClassDB::set_current_api(prev_api);
#endif
}

void unregister_gdnavigation_types() {
#ifndef _3D_DISABLED
  if (_nav_mesh_generator) {
    memdelete(_nav_mesh_generator);
  }
#endif
}

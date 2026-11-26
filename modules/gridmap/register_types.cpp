// This file registers and unregisters the types and functionality of the
// GridMap module within the Godot engine. It includes the necessary headers and
// defines functions for integrating the GridMap node and its editor plugin with
// the engine.
#include "register_types.h"
#ifndef _3D_DISABLED
#include "core/class_db.h"
#include "grid_map.h"
#include "grid_map_editor_plugin.h"
#endif

void register_gridmap_types() {

#ifndef _3D_DISABLED
  ClassDB::register_class<GridMap>();
#ifdef TOOLS_ENABLED
  EditorPlugins::add_by_type<GridMapEditorPlugin>();
#endif
#endif
}

void unregister_gridmap_types() {}

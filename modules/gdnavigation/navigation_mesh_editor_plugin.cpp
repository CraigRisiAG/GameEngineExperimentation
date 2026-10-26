
#ifdef TOOLS_ENABLED
#include "navigation_mesh_editor_plugin.h"

#include "core/io/marshalls.h"
#include "core/io/resource_saver.h"
#include "navigation_mesh_generator.h"
#include "scene/3d/mesh_instance.h"
#include "scene/gui/box_container.h"

void NavigationMeshEditor::_node_removed(Node *p_node) {

  if (p_node == node) {
    node = NULL;

    hide();
  }
}

void NavigationMeshEditor::_notification(int p_option) {

  if (p_option == NOTIFICATION_ENTER_TREE) {

    button_bake->set_icon(get_icon("Bake", "EditorIcons"));
    button_reset->set_icon(get_icon("Reload", "EditorIcons"));
  }
}

void NavigationMeshEditor::_bake_pressed() {
  button_bake->set_pressed(false);

  ERR_FAIL_COND(!node);
  if (!node->get_navigation_mesh().is_valid()) {
    err_dialog->set_text(TTR("A NavigationMesh resource must be set or created "
                             "for this node to work."));
    err_dialog->popup_centered_minsize();
    return;
  }

  NavigationMeshGenerator::get_singleton()->clear(node->get_navigation_mesh());
  NavigationMeshGenerator::get_singleton()->bake(node->get_navigation_mesh(),
                                                 node);

  node->update_gizmo();
}

void NavigationMeshEditor::_clear_pressed() {

  if (node)
    NavigationMeshGenerator::get_singleton()->clear(
        node->get_navigation_mesh());

  button_bake->set_pressed(false);
  bake_info->set_text("");

  if (node) {
    node->update_gizmo();
  }
}

void NavigationMeshEditor::edit(NavigationRegion *p_nav_region) {

  if (p_nav_region == NULL || node == p_nav_region) {
    return;
  }

  node = p_nav_region;
}

void NavigationMeshEditor::_bind_methods() {}

NavigationMeshEditor::NavigationMeshEditor() {

  bake_hbox = memnew(HBoxContainer);

  button_bake = memnew(ToolButton);
  bake_hbox->add_child(button_bake);
  button_bake->set_toggle_mode(true);
  button_bake->set_text(TTR("Bake NavMesh"));
  button_bake->connect("pressed",
                       callable_mp(this, &NavigationMeshEditor::_bake_pressed));

  button_reset = memnew(ToolButton);
  bake_hbox->add_child(button_reset);
  // No button text, we only use a revert icon which is set when entering the
  // tree.
  button_reset->set_tooltip(TTR("Clear the navigation mesh."));
  button_reset->connect(
      "pressed", callable_mp(this, &NavigationMeshEditor::_clear_pressed));

  bake_info = memnew(Label);
  bake_hbox->add_child(bake_info);

  err_dialog = memnew(AcceptDialog);
  add_child(err_dialog);
  node = NULL;
}

NavigationMeshEditor::~NavigationMeshEditor() {}

void NavigationMeshEditorPlugin::edit(Object *p_object) {

  navigation_mesh_editor->edit(Object::cast_to<NavigationRegion>(p_object));
}

bool NavigationMeshEditorPlugin::handles(Object *p_object) const {

  return p_object->is_class("NavigationRegion");
}

void NavigationMeshEditorPlugin::make_visible(bool p_visible) {

  if (p_visible) {
    navigation_mesh_editor->show();
    navigation_mesh_editor->bake_hbox->show();
  } else {

    navigation_mesh_editor->hide();
    navigation_mesh_editor->bake_hbox->hide();
    navigation_mesh_editor->edit(NULL);
  }
}

NavigationMeshEditorPlugin::NavigationMeshEditorPlugin(EditorNode *p_node) {

  editor = p_node;
  navigation_mesh_editor = memnew(NavigationMeshEditor);
  editor->get_viewport()->add_child(navigation_mesh_editor);
  add_control_to_container(CONTAINER_SPATIAL_EDITOR_MENU,
                           navigation_mesh_editor->bake_hbox);
  navigation_mesh_editor->hide();
  navigation_mesh_editor->bake_hbox->hide();
}

NavigationMeshEditorPlugin::~NavigationMeshEditorPlugin() {}

#endif

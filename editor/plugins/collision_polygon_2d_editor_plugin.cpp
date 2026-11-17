/**
 * Collision Polygon 2D Editor Plugin for Godot Engine.
 * This file defines the CollisionPolygon2DEditor and
 * CollisionPolygon2DEditorPlugin classes, which provide functionality to edit
 * and manage CollisionPolygon2D nodes in the editor.
 */
#include "collision_polygon_2d_editor_plugin.h"

Node2D *CollisionPolygon2DEditor::_get_node() const { return node; }

void CollisionPolygon2DEditor::_set_node(Node *p_polygon) {

  node = Object::cast_to<CollisionPolygon2D>(p_polygon);
}

CollisionPolygon2DEditor::CollisionPolygon2DEditor(EditorNode *p_editor)
    : AbstractPolygon2DEditor(p_editor) {
  node = NULL;
}

CollisionPolygon2DEditorPlugin::CollisionPolygon2DEditorPlugin(
    EditorNode *p_node)
    : AbstractPolygon2DEditorPlugin(p_node,
                                    memnew(CollisionPolygon2DEditor(p_node)),
                                    "CollisionPolygon2D") {}



/**
 * @class EditorScript
 * @brief A script class that provides editor-level functionality and access to the editor interface.
 * 
 * EditorScript allows users to write scripts that can interact with the Godot editor,
 * including accessing and manipulating the currently edited scene. Scripts using this class
 * must include the 'tool' keyword and implement the _run() method.
 * 
 * @method void add_root_node(Node *p_node)
 * @brief Adds a root node to the editor as the edited scene.
 * @param p_node The node to set as the root of the edited scene.
 * @note Fails if editor is not initialized or if a scene is already being edited.
 * 
 * @method EditorInterface* get_editor_interface()
 * @brief Returns the singleton EditorInterface instance for accessing editor functionality.
 * @return Pointer to the EditorInterface singleton.
 * 
 * @method Node* get_scene()
 * @brief Retrieves the currently edited scene in the editor.
 * @return Pointer to the root node of the edited scene, or NULL if no scene is open.
 * @note Returns NULL and logs an error if editor is not initialized.
 * 
 * @method void _run()
 * @brief Virtual method that executes the editor script logic.
 * @note Must be implemented in derived script classes. Called automatically by the editor.
 * 
 * @method void set_editor(EditorNode *p_editor)
 * @brief Sets the editor instance for this script to operate on.
 * @param p_editor Pointer to the EditorNode instance.
 * 
 * @method void _bind_methods()
 * @brief Binds methods to the scripting system for reflection and remote call support.
 * 
 * @constructor EditorScript()
 * @brief Initializes a new EditorScript instance with no editor reference.
 */
#include "editor_run_script.h"

#include "editor_node.h"

void EditorScript::add_root_node(Node *p_node) {

	if (!editor) {
		EditorNode::add_io_error("EditorScript::add_root_node: " + TTR("Write your logic in the _run() method."));
		return;
	}

	if (editor->get_edited_scene()) {
		EditorNode::add_io_error("EditorScript::add_root_node: " + TTR("There is an edited scene already."));
		return;
	}

	//editor->set_edited_scene(p_node);
}

EditorInterface *EditorScript::get_editor_interface() {

	return EditorInterface::get_singleton();
}

Node *EditorScript::get_scene() {

	if (!editor) {
		EditorNode::add_io_error("EditorScript::get_scene: " + TTR("Write your logic in the _run() method."));
		return NULL;
	}

	return editor->get_edited_scene();
}

void EditorScript::_run() {

	Ref<Script> s = get_script();
	ERR_FAIL_COND(!s.is_valid());
	if (!get_script_instance()) {
		EditorNode::add_io_error(TTR("Couldn't instance script:") + "\n " + s->get_path() + "\n" + TTR("Did you forget the 'tool' keyword?"));
		return;
	}

	Callable::CallError ce;
	ce.error = Callable::CallError::CALL_OK;
	get_script_instance()->call("_run", NULL, 0, ce);
	if (ce.error != Callable::CallError::CALL_OK) {

		EditorNode::add_io_error(TTR("Couldn't run script:") + "\n " + s->get_path() + "\n" + TTR("Did you forget the '_run' method?"));
	}
}

void EditorScript::set_editor(EditorNode *p_editor) {

	editor = p_editor;
}

void EditorScript::_bind_methods() {

	ClassDB::bind_method(D_METHOD("add_root_node", "node"), &EditorScript::add_root_node);
	ClassDB::bind_method(D_METHOD("get_scene"), &EditorScript::get_scene);
	ClassDB::bind_method(D_METHOD("get_editor_interface"), &EditorScript::get_editor_interface);
	BIND_VMETHOD(MethodInfo("_run"));
}

EditorScript::EditorScript() {

	editor = NULL;
}

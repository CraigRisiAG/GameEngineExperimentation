

/// @class EditorAbout
/// @brief A dialog window that displays information about the editor, including licenses and third-party libraries.
/// 
/// EditorAbout is a specialized dialog (inheriting from AcceptDialog) that presents
/// the editor's logo, license information, and third-party library attributions in
/// an organized tabbed interface.
/// 
/// @details
/// The dialog provides:
/// - Editor logo display via TextureRect
/// - License text viewing through RichTextLabel
/// - Third-party library/template information in a tree structure
/// - Scrollable content areas for better readability
/// 
/// @note This class is part of the Godot editor's GUI infrastructure and uses
/// the engine's node system through GDCLASS registration.
#ifndef EDITOR_ABOUT_H
#define EDITOR_ABOUT_H

#include "scene/gui/control.h"
#include "scene/gui/dialogs.h"
#include "scene/gui/item_list.h"
#include "scene/gui/rich_text_label.h"
#include "scene/gui/scroll_container.h"
#include "scene/gui/separator.h"
#include "scene/gui/split_container.h"
#include "scene/gui/tab_container.h"
#include "scene/gui/texture_rect.h"
#include "scene/gui/tree.h"

#include "editor_scale.h"

class EditorAbout : public AcceptDialog {

	GDCLASS(EditorAbout, AcceptDialog);

private:
	void _license_tree_selected();
	ScrollContainer *_populate_list(const String &p_name, const List<String> &p_sections, const char *const *const p_src[], const int p_flag_single_column = 0);

	Tree *_tpl_tree;
	RichTextLabel *_license_text;
	RichTextLabel *_tpl_text;
	TextureRect *_logo;

protected:
	void _notification(int p_what);
	static void _bind_methods();

public:
	TextureRect *get_logo() const;

	EditorAbout();
	~EditorAbout();
};

#endif

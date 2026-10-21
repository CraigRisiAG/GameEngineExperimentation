
#ifndef GIPROBEEDITORPLUGIN_H
#define GIPROBEEDITORPLUGIN_H

#include "editor/editor_node.h"
#include "editor/editor_plugin.h"
#include "scene/3d/gi_probe.h"
#include "scene/resources/material.h"

class GIProbeEditorPlugin : public EditorPlugin {

  GDCLASS(GIProbeEditorPlugin, EditorPlugin);

  GIProbe *gi_probe;

  HBoxContainer *bake_hb;
  Label *bake_info;
  ToolButton *bake;
  EditorNode *editor;

  EditorFileDialog *probe_file;

  static EditorProgress *tmp_progress;
  static void bake_func_begin(int p_steps);
  static void bake_func_step(int p_step, const String &p_description);
  static void bake_func_end();

  void _bake();
  void _giprobe_save_path_and_bake(const String &p_path);

protected:
  static void _bind_methods();
  void _notification(int p_what);

public:
  virtual String get_name() const { return "GIProbe"; }
  bool has_main_screen() const { return false; }
  virtual void edit(Object *p_object);
  virtual bool handles(Object *p_object) const;
  virtual void make_visible(bool p_visible);

  GIProbeEditorPlugin(EditorNode *p_node);
  ~GIProbeEditorPlugin();
};

#endif // GIPROBEEDITORPLUGIN_H

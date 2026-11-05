/**
 * @file editor_network_profiler.h
 * @brief Declaration of the EditorNetworkProfiler class for handling network
 * profiling in the editor.
 */
#ifndef EDITORNETWORKPROFILER_H
#define EDITORNETWORKPROFILER_H

#include "core/debugger/debugger_marshalls.h"
#include "scene/gui/box_container.h"
#include "scene/gui/button.h"
#include "scene/gui/label.h"
#include "scene/gui/split_container.h"
#include "scene/gui/tree.h"

class EditorNetworkProfiler : public VBoxContainer {

  GDCLASS(EditorNetworkProfiler, VBoxContainer)

private:
  Button *activate;
  Button *clear_button;
  Tree *counters_display;
  LineEdit *incoming_bandwidth_text;
  LineEdit *outgoing_bandwidth_text;

  Timer *frame_delay;

  Map<ObjectID, DebuggerMarshalls::MultiplayerNodeInfo> nodes_data;

  void _update_frame();

  void _activate_pressed();
  void _clear_pressed();

protected:
  void _notification(int p_what);
  static void _bind_methods();

public:
  void
  add_node_frame_data(const DebuggerMarshalls::MultiplayerNodeInfo p_frame);
  void set_bandwidth(int p_incoming, int p_outgoing);
  bool is_profiling();

  EditorNetworkProfiler();
};

#endif // EDITORNETWORKPROFILER_H

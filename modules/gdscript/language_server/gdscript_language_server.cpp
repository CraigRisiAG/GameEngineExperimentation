
#include "gdscript_language_server.h"
#include "core/os/file_access.h"
#include "core/os/os.h"
#include "editor/editor_log.h"
#include "editor/editor_node.h"

GDScriptLanguageServer::GDScriptLanguageServer() {
  thread = NULL;
  thread_running = false;
  started = false;

  use_thread = false;
  port = 6008;
  _EDITOR_DEF("network/language_server/remote_port", port);
  _EDITOR_DEF("network/language_server/enable_smart_resolve", true);
  _EDITOR_DEF("network/language_server/show_native_symbols_in_editor", false);
  _EDITOR_DEF("network/language_server/use_thread", use_thread);
}

void GDScriptLanguageServer::_notification(int p_what) {

  switch (p_what) {
  case NOTIFICATION_ENTER_TREE:
    start();
    break;
  case NOTIFICATION_EXIT_TREE:
    stop();
    break;
  case NOTIFICATION_INTERNAL_PROCESS: {
    if (started && !use_thread) {
      protocol.poll();
    }
  } break;
  case EditorSettings::NOTIFICATION_EDITOR_SETTINGS_CHANGED: {
    int port = (int)_EDITOR_GET("network/language_server/remote_port");
    bool use_thread = (bool)_EDITOR_GET("network/language_server/use_thread");
    if (port != this->port || use_thread != this->use_thread) {
      this->stop();
      this->start();
    }
  } break;
  }
}

void GDScriptLanguageServer::thread_main(void *p_userdata) {
  GDScriptLanguageServer *self =
      static_cast<GDScriptLanguageServer *>(p_userdata);
  while (self->thread_running) {
    // Poll 20 times per second
    self->protocol.poll();
    OS::get_singleton()->delay_usec(50000);
  }
}

void GDScriptLanguageServer::start() {
  port = (int)_EDITOR_GET("network/language_server/remote_port");
  use_thread = (bool)_EDITOR_GET("network/language_server/use_thread");
  if (protocol.start(port, IP_Address("127.0.0.1")) == OK) {
    EditorNode::get_log()->add_message(
        "--- GDScript language server started ---", EditorLog::MSG_TYPE_EDITOR);
    if (use_thread) {
      ERR_FAIL_COND(thread != NULL);
      thread_running = true;
      thread = Thread::create(GDScriptLanguageServer::thread_main, this);
    }
    set_process_internal(!use_thread);
    started = true;
  }
}

void GDScriptLanguageServer::stop() {
  if (use_thread) {
    ERR_FAIL_COND(NULL == thread);
    thread_running = false;
    Thread::wait_to_finish(thread);
    memdelete(thread);
    thread = NULL;
  }
  protocol.stop();
  started = false;
  EditorNode::get_log()->add_message("--- GDScript language server stopped ---",
                                     EditorLog::MSG_TYPE_EDITOR);
}

void register_lsp_types() {
  ClassDB::register_class<GDScriptLanguageProtocol>();
  ClassDB::register_class<GDScriptTextDocument>();
  ClassDB::register_class<GDScriptWorkspace>();
}


#ifndef GD_MONO_LOG_H
#define GD_MONO_LOG_H

#include <mono/utils/mono-logger.h>

#include "core/typedefs.h"

#if !defined(JAVASCRIPT_ENABLED)
#include "core/os/file_access.h"
#endif

class GDMonoLog {

#if !defined(JAVASCRIPT_ENABLED)
  int log_level_id;

  FileAccess *log_file;
  String log_file_path;

  bool _try_create_logs_dir(const String &p_logs_dir);
  void _delete_old_log_files(const String &p_logs_dir);

  static void mono_log_callback(const char *log_domain, const char *log_level,
                                const char *message, mono_bool fatal,
                                void *user_data);
#endif

  static GDMonoLog *singleton;

public:
  _FORCE_INLINE_ static GDMonoLog *get_singleton() { return singleton; }

  void initialize();

  GDMonoLog();
  ~GDMonoLog();
};

#endif // GD_MONO_LOG_H

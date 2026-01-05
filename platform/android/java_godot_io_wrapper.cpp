
/**
 * @file java_godot_io_wrapper.cpp
 * @brief JNI wrapper for Godot I/O operations on Android platform
 *
 * This file implements the GodotIOJavaWrapper class which serves as a bridge
 * between the C++ engine and Android Java APIs through JNI (Java Native
 * Interface).
 *
 * @note JNIEnv is thread-specific and cannot be cached. All method calls
 * retrieve a valid JNIEnv from the current thread via ThreadAndroid::get_env().
 *
 * @class GodotIOJavaWrapper
 * @brief Wrapper class for Android I/O operations
 *
 * Provides access to Android system functionality including:
 * - URI opening
 * - File system access (data directory, system directories)
 * - Device information (locale, model, screen DPI, unique ID)
 * - Virtual keyboard control
 * - Screen orientation management
 * - Video playback control
 *
 * @method GodotIOJavaWrapper(JNIEnv *p_env, jobject p_godot_io_instance)
 * @brief Constructor that initializes JNI references and caches method IDs
 * @param p_env JNI environment pointer (valid only in current thread)
 * @param p_godot_io_instance Java GodotIO object instance
 *
 * @method ~GodotIOJavaWrapper()
 * @brief Destructor
 *
 * @method Error open_uri(const String &p_uri)
 * @brief Opens a URI in the system default handler
 * @param p_uri URI string to open
 * @return OK on success, ERR_CANT_OPEN on failure, ERR_UNAVAILABLE if method
 * not available
 *
 * @method String get_user_data_dir()
 * @brief Retrieves the application's data directory path
 * @return Data directory path string
 *
 * @method String get_locale()
 * @brief Gets the system locale string
 * @return Locale identifier
 *
 * @method String get_model()
 * @brief Gets the device model name
 * @return Device model string
 *
 * @method int get_screen_dpi()
 * @brief Retrieves screen DPI value
 * @return Screen DPI (defaults to 160 if unavailable)
 *
 * @method String get_unique_id()
 * @brief Gets a unique device identifier
 * @return Unique device ID string
 *
 * @method bool has_vk()
 * @brief Checks if virtual keyboard is available
 * @return true if both show and hide keyboard methods are available
 *
 * @method void show_vk(const String &p_existing, int p_max_input_length)
 * @brief Shows the virtual keyboard
 * @param p_existing Initial text content
 * @param p_max_input_length Maximum allowed input length
 *
 * @method void hide_vk()
 * @brief Hides the virtual keyboard
 *
 * @method void set_screen_orientation(int p_orient)
 * @brief Sets the screen orientation
 * @param p_orient Orientation value
 *
 * @method String get_system_dir(int p_dir)
 * @brief Retrieves system directory path
 * @param p_dir Directory type identifier
 * @return System directory path
 *
 * @method bool is_video_playing()
 * @brief Checks if video playback is active
 * @return true if video is currently playing
 *
 * @method void pause_video()
 * @brief Pauses video playback
 *
 * @method void stop_video()
 * @brief Stops video playback
 *
 * @method int get_vk_height()
 * @brief Gets the current virtual keyboard height
 * @return Keyboard height in pixels
 *
 * @method void set_vk_height(int p_height)
 * @brief Sets the virtual keyboard height
 * @param p_height Height value in pixels
 * @note This method is thread-safe using volatile access
 */
#include "java_godot_io_wrapper.h"
#include "core/error_list.h"

// JNIEnv is only valid within the thread it belongs to, in a multi threading
// environment we can't cache it. For GodotIO we call all access methods from
// our thread and we thus get a valid JNIEnv from ThreadAndroid.

GodotIOJavaWrapper::GodotIOJavaWrapper(JNIEnv *p_env,
                                       jobject p_godot_io_instance) {
  godot_io_instance = p_env->NewGlobalRef(p_godot_io_instance);
  if (godot_io_instance) {
    cls = p_env->GetObjectClass(godot_io_instance);
    if (cls) {
      cls = (jclass)p_env->NewGlobalRef(cls);
    } else {
      // this is a pretty serious fail.. bail... pointers will stay 0
      return;
    }

    _open_URI = p_env->GetMethodID(cls, "openURI", "(Ljava/lang/String;)I");
    _get_data_dir =
        p_env->GetMethodID(cls, "getDataDir", "()Ljava/lang/String;");
    _get_locale = p_env->GetMethodID(cls, "getLocale", "()Ljava/lang/String;");
    _get_model = p_env->GetMethodID(cls, "getModel", "()Ljava/lang/String;");
    _get_screen_DPI = p_env->GetMethodID(cls, "getScreenDPI", "()I");
    _get_unique_id =
        p_env->GetMethodID(cls, "getUniqueID", "()Ljava/lang/String;");
    _show_keyboard =
        p_env->GetMethodID(cls, "showKeyboard", "(Ljava/lang/String;I)V");
    _hide_keyboard = p_env->GetMethodID(cls, "hideKeyboard", "()V");
    _set_screen_orientation =
        p_env->GetMethodID(cls, "setScreenOrientation", "(I)V");
    _get_system_dir =
        p_env->GetMethodID(cls, "getSystemDir", "(I)Ljava/lang/String;");
    _play_video = p_env->GetMethodID(cls, "playVideo", "(Ljava/lang/String;)V");
    _is_video_playing = p_env->GetMethodID(cls, "isVideoPlaying", "()Z");
    _pause_video = p_env->GetMethodID(cls, "pauseVideo", "()V");
    _stop_video = p_env->GetMethodID(cls, "stopVideo", "()V");
  }
}

GodotIOJavaWrapper::~GodotIOJavaWrapper() {
  // nothing to do here for now
}

jobject GodotIOJavaWrapper::get_instance() { return godot_io_instance; }

Error GodotIOJavaWrapper::open_uri(const String &p_uri) {
  if (_open_URI) {
    JNIEnv *env = ThreadAndroid::get_env();
    jstring jStr = env->NewStringUTF(p_uri.utf8().get_data());
    return env->CallIntMethod(godot_io_instance, _open_URI, jStr)
               ? ERR_CANT_OPEN
               : OK;
  } else {
    return ERR_UNAVAILABLE;
  }
}

String GodotIOJavaWrapper::get_user_data_dir() {
  if (_get_data_dir) {
    JNIEnv *env = ThreadAndroid::get_env();
    jstring s =
        (jstring)env->CallObjectMethod(godot_io_instance, _get_data_dir);
    return jstring_to_string(s, env);
  } else {
    return String();
  }
}

String GodotIOJavaWrapper::get_locale() {
  if (_get_locale) {
    JNIEnv *env = ThreadAndroid::get_env();
    jstring s = (jstring)env->CallObjectMethod(godot_io_instance, _get_locale);
    return jstring_to_string(s, env);
  } else {
    return String();
  }
}

String GodotIOJavaWrapper::get_model() {
  if (_get_model) {
    JNIEnv *env = ThreadAndroid::get_env();
    jstring s = (jstring)env->CallObjectMethod(godot_io_instance, _get_model);
    return jstring_to_string(s, env);
  } else {
    return String();
  }
}

int GodotIOJavaWrapper::get_screen_dpi() {
  if (_get_screen_DPI) {
    JNIEnv *env = ThreadAndroid::get_env();
    return env->CallIntMethod(godot_io_instance, _get_screen_DPI);
  } else {
    return 160;
  }
}

String GodotIOJavaWrapper::get_unique_id() {
  if (_get_unique_id) {
    JNIEnv *env = ThreadAndroid::get_env();
    jstring s =
        (jstring)env->CallObjectMethod(godot_io_instance, _get_unique_id);
    return jstring_to_string(s, env);
  } else {
    return String();
  }
}

bool GodotIOJavaWrapper::has_vk() {
  return (_show_keyboard != 0) && (_hide_keyboard != 0);
}

void GodotIOJavaWrapper::show_vk(const String &p_existing,
                                 int p_max_input_length) {
  if (_show_keyboard) {
    JNIEnv *env = ThreadAndroid::get_env();
    jstring jStr = env->NewStringUTF(p_existing.utf8().get_data());
    env->CallVoidMethod(godot_io_instance, _show_keyboard, jStr,
                        p_max_input_length);
  }
}

void GodotIOJavaWrapper::hide_vk() {
  if (_hide_keyboard) {
    JNIEnv *env = ThreadAndroid::get_env();
    env->CallVoidMethod(godot_io_instance, _hide_keyboard);
  }
}

void GodotIOJavaWrapper::set_screen_orientation(int p_orient) {
  if (_set_screen_orientation) {
    JNIEnv *env = ThreadAndroid::get_env();
    env->CallVoidMethod(godot_io_instance, _set_screen_orientation, p_orient);
  }
}

String GodotIOJavaWrapper::get_system_dir(int p_dir) {
  if (_get_system_dir) {
    JNIEnv *env = ThreadAndroid::get_env();
    jstring s = (jstring)env->CallObjectMethod(godot_io_instance,
                                               _get_system_dir, p_dir);
    return jstring_to_string(s, env);
  } else {
    return String(".");
  }
}

void GodotIOJavaWrapper::play_video(const String &p_path) {
  // Why is this not here?!?!
}

bool GodotIOJavaWrapper::is_video_playing() {
  if (_is_video_playing) {
    JNIEnv *env = ThreadAndroid::get_env();
    return env->CallBooleanMethod(godot_io_instance, _is_video_playing);
  } else {
    return false;
  }
}

void GodotIOJavaWrapper::pause_video() {
  if (_pause_video) {
    JNIEnv *env = ThreadAndroid::get_env();
    env->CallVoidMethod(godot_io_instance, _pause_video);
  }
}

void GodotIOJavaWrapper::stop_video() {
  if (_stop_video) {
    JNIEnv *env = ThreadAndroid::get_env();
    env->CallVoidMethod(godot_io_instance, _stop_video);
  }
}

// volatile because it can be changed from non-main thread and we need to
// ensure the change is immediately visible to other threads.
static volatile int virtual_keyboard_height;

int GodotIOJavaWrapper::get_vk_height() { return virtual_keyboard_height; }

void GodotIOJavaWrapper::set_vk_height(int p_height) {
  virtual_keyboard_height = p_height;
}

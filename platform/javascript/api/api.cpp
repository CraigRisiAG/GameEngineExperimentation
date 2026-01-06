
/// \brief Registers the JavaScript API singleton and virtual class.
///
/// This function initializes the JavaScript API by registering the JavaScript
/// class with the class database and creating a singleton instance that is
/// added to the engine's global singletons.
void register_javascript_api();

/// \brief Unregisters the JavaScript API singleton.
///
/// This function cleans up the JavaScript singleton instance by freeing its
/// memory when the API is no longer needed.
void unregister_javascript_api();

/// \class JavaScript
/// \brief Singleton class providing JavaScript evaluation capabilities.
///
/// The JavaScript class is a virtual singleton that provides methods to
/// evaluate JavaScript code. It manages a single instance accessible throughout
/// the engine.
class JavaScript {
public:
  /// \brief Gets the singleton instance of the JavaScript class.
  /// \return Pointer to the JavaScript singleton instance, or NULL if not
  /// initialized.
  static JavaScript *get_singleton();

  /// \brief Constructor for the JavaScript singleton.
  ///
  /// Initializes the JavaScript singleton instance. An error is raised if
  /// a singleton instance already exists.
  JavaScript();

  /// \brief Destructor for the JavaScript singleton.
  ~JavaScript();

  /// \brief Evaluates JavaScript code.
  /// \param p_code The JavaScript code string to evaluate.
  /// \param p_use_global_exec_context If true, evaluates code in global
  /// context;
  ///        defaults to false.
  /// \return The result of the JavaScript evaluation as a Variant, or an empty
  ///         Variant if JavaScript evaluation is not enabled.
  Variant eval(const String &p_code, bool p_use_global_exec_context = false);

private:
  /// \brief Static singleton instance pointer.
  static JavaScript *singleton;

  /// \brief Registers the methods of the JavaScript class for the script
  /// binding.
  static void _bind_methods();
};
#include "api.h"
#include "core/engine.h"
#include "javascript_eval.h"

static JavaScript *javascript_eval;

void register_javascript_api() {

  ClassDB::register_virtual_class<JavaScript>();
  javascript_eval = memnew(JavaScript);
  Engine::get_singleton()->add_singleton(
      Engine::Singleton("JavaScript", javascript_eval));
}

void unregister_javascript_api() { memdelete(javascript_eval); }

JavaScript *JavaScript::singleton = NULL;

JavaScript *JavaScript::get_singleton() { return singleton; }

JavaScript::JavaScript() {

  ERR_FAIL_COND_MSG(singleton != NULL, "JavaScript singleton already exist.");
  singleton = this;
}

JavaScript::~JavaScript() {}

void JavaScript::_bind_methods() {

  ClassDB::bind_method(D_METHOD("eval", "code", "use_global_execution_context"),
                       &JavaScript::eval, DEFVAL(false));
}

#if !defined(JAVASCRIPT_ENABLED) || !defined(JAVASCRIPT_EVAL_ENABLED)
Variant JavaScript::eval(const String &p_code, bool p_use_global_exec_context) {

  return Variant();
}
#endif

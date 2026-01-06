
/**
 * @class JavaScript
 * @brief Singleton class for evaluating JavaScript code within the engine.
 * 
 * The JavaScript class provides an interface to execute JavaScript code and retrieve results
 * as Variant values. It follows the singleton pattern to ensure only one instance exists
 * throughout the application lifetime.
 */

/**
 * @fn Variant eval(const String &p_code, bool p_use_global_exec_context = false)
 * @brief Evaluates JavaScript code and returns the result as a Variant.
 * 
 * @param p_code The JavaScript code to evaluate as a string.
 * @param p_use_global_exec_context If true, uses the global execution context; otherwise uses a local context.
 *                                   Defaults to false.
 * 
 * @return A Variant containing the result of the JavaScript evaluation.
 */

/**
 * @fn static JavaScript* get_singleton()
 * @brief Returns the singleton instance of the JavaScript class.
 * 
 * @return A pointer to the single JavaScript instance. Creates the instance on first call.
 */
#ifndef JAVASCRIPT_EVAL_H
#define JAVASCRIPT_EVAL_H

#include "core/object.h"

class JavaScript : public Object {
private:
	GDCLASS(JavaScript, Object);

	static JavaScript *singleton;

protected:
	static void _bind_methods();

public:
	Variant eval(const String &p_code, bool p_use_global_exec_context = false);

	static JavaScript *get_singleton();
	JavaScript();
	~JavaScript();
};

#endif // JAVASCRIPT_EVAL_H

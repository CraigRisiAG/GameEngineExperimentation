


/**
 * @class InputMap
 * @brief Manages input action mappings and event handling.
 * 
 * InputMap is a singleton class that stores and manages input actions and their associated
 * input events. It provides functionality to create, modify, and query input actions, allowing
 * the game engine to map physical input events (keyboard, mouse, gamepad) to logical game actions.
 * 
 * Each action has a unique name, a deadzone threshold for analog inputs, and a list of input
 * events that can trigger it. Multiple input events can map to the same action, and actions
 * can be triggered by any device or specific devices.
 * 
 * @note This is a singleton class. Use InputMap::get_singleton() to access the instance.
 * 
 * @see InputEvent
 * @see Object
 */

/**
 * @struct InputMap::Action
 * @brief Represents a single input action with its configuration.
 * 
 * @member int id - The unique identifier for this action.
 * @member float deadzone - The deadzone threshold for analog inputs (range: 0.0 to 1.0).
 * @member List<Ref<InputEvent>> inputs - Collection of input events that trigger this action.
 */

/**
 * @var InputMap::ALL_DEVICES
 * @brief Special constant indicating that an action can be triggered by any input device.
 */

/**
 * @fn static InputMap* InputMap::get_singleton()
 * @brief Returns the singleton instance of InputMap.
 * @return Pointer to the InputMap singleton instance.
 */

/**
 * @fn bool InputMap::has_action(const StringName &p_action) const
 * @brief Checks if an action with the given name exists.
 * @param p_action The name of the action to check.
 * @return True if the action exists, false otherwise.
 */

/**
 * @fn void InputMap::add_action(const StringName &p_action, float p_deadzone = 0.5)
 * @brief Creates a new input action.
 * @param p_action The name of the action to create.
 * @param p_deadzone The deadzone threshold for analog inputs (default: 0.5).
 */

/**
 * @fn void InputMap::action_add_event(const StringName &p_action, const Ref<InputEvent> &p_event)
 * @brief Adds an input event to an existing action.
 * @param p_action The name of the action.
 * @param p_event The input event to add to the action.
 */

/**
 * @fn bool InputMap::event_is_action(const Ref<InputEvent> &p_event, const StringName &p_action) const
 * @brief Determines if an input event triggers a specific action.
 * @param p_event The input event to check.
 * @param p_action The name of the action to test against.
 * @return True if the event triggers the action, false otherwise.
 */
#ifndef INPUT_MAP_H
#define INPUT_MAP_H

#include "core/object.h"
#include "core/os/input_event.h"

class InputMap : public Object {

	GDCLASS(InputMap, Object);

public:
	/**
	* A special value used to signify that a given Action can be triggered by any device
	*/
	static int ALL_DEVICES;

	struct Action {
		int id;
		float deadzone;
		List<Ref<InputEvent>> inputs;
	};

private:
	static InputMap *singleton;

	mutable Map<StringName, Action> input_map;

	List<Ref<InputEvent>>::Element *_find_event(Action &p_action, const Ref<InputEvent> &p_event, bool *p_pressed = NULL, float *p_strength = NULL) const;

	Array _get_action_list(const StringName &p_action);
	Array _get_actions();

protected:
	static void _bind_methods();

public:
	static _FORCE_INLINE_ InputMap *get_singleton() { return singleton; }

	bool has_action(const StringName &p_action) const;
	List<StringName> get_actions() const;
	void add_action(const StringName &p_action, float p_deadzone = 0.5);
	void erase_action(const StringName &p_action);

	void action_set_deadzone(const StringName &p_action, float p_deadzone);
	void action_add_event(const StringName &p_action, const Ref<InputEvent> &p_event);
	bool action_has_event(const StringName &p_action, const Ref<InputEvent> &p_event);
	void action_erase_event(const StringName &p_action, const Ref<InputEvent> &p_event);
	void action_erase_events(const StringName &p_action);

	const List<Ref<InputEvent>> *get_action_list(const StringName &p_action);
	bool event_is_action(const Ref<InputEvent> &p_event, const StringName &p_action) const;
	bool event_get_action_status(const Ref<InputEvent> &p_event, const StringName &p_action, bool *p_pressed = NULL, float *p_strength = NULL) const;

	const Map<StringName, Action> &get_action_map() const;
	void load_from_globals();
	void load_default();

	InputMap();
};

#endif // INPUT_MAP_H

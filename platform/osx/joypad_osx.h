

/**
 * @file joypad_osx.h
 * @brief macOS joypad/gamepad input handling using IOKit HID framework.
 * 
 * This module provides joypad input support for macOS through the IOKit
 * Human Interface Device (HID) framework. It handles device detection,
 * input polling, and force feedback functionality for game controllers.
 */

/**
 * @struct rec_element
 * @brief Represents a single HID element (axis, button, or hat) from a joypad.
 * 
 * @var rec_element::ref
 *      Reference to the IOKit HID element.
 * @var rec_element::cookie
 *      Cookie identifier for the HID element.
 * @var rec_element::usage
 *      HID usage code identifying the element type.
 * @var rec_element::min
 *      Minimum value range for the element.
 * @var rec_element::max
 *      Maximum value range for the element.
 * @var rec_element::Comparator
 *      Comparator functor for sorting elements by usage.
 */

/**
 * @struct joypad
 * @brief Represents a connected joypad device with all its input and force feedback data.
 * 
 * @var joypad::device_ref
 *      IOKit device reference for this joypad.
 * @var joypad::axis_elements
 *      Vector of analog axis elements (joysticks, triggers).
 * @var joypad::button_elements
 *      Vector of button input elements.
 * @var joypad::hat_elements
 *      Vector of directional pad (hat) elements.
 * @var joypad::id
 *      Unique identifier assigned to this joypad.
 * @var joypad::ffservice
 *      Force feedback service interface (0 if not supported).
 * @var joypad::ff_constant_force
 *      Force feedback constant force parameters.
 * @var joypad::ff_device
 *      Force feedback device object reference.
 * @var joypad::ff_object
 *      Force feedback effect object reference.
 * @var joypad::ff_timestamp
 *      Timestamp of last force feedback effect.
 * @var joypad::ff_directions
 *      Array of force feedback effect directions.
 * @var joypad::ff_effect
 *      Force feedback effect structure.
 * @var joypad::ff_axes
 *      Array of force feedback axes.
 */

/**
 * @class JoypadOSX
 * @brief Manages multiple joypad devices connected to the macOS system.
 * 
 * Handles IOKit HID manager operations including device enumeration, polling,
 * and vibration control for up to 16 connected joypads simultaneously.
 * 
 * @note Maximum of 16 joypads can be managed concurrently.
 */
#ifndef JOYPADOSX_H
#define JOYPADOSX_H

#ifdef MACOS_10_0_4
#include <IOKit/hidsystem/IOHIDUsageTables.h>
#else
#include <Kernel/IOKit/hidsystem/IOHIDUsageTables.h>
#endif
#include <ForceFeedback/ForceFeedback.h>
#include <ForceFeedback/ForceFeedbackConstants.h>
#include <IOKit/hid/IOHIDLib.h>

#include "main/input_default.h"

struct rec_element {
	IOHIDElementRef ref;
	IOHIDElementCookie cookie;

	uint32_t usage;

	int min;
	int max;

	struct Comparator {
		bool operator()(const rec_element p_a, const rec_element p_b) const { return p_a.usage < p_b.usage; }
	};
};

struct joypad {
	IOHIDDeviceRef device_ref;

	Vector<rec_element> axis_elements;
	Vector<rec_element> button_elements;
	Vector<rec_element> hat_elements;

	int id;

	io_service_t ffservice; /* Interface for force feedback, 0 = no ff */
	FFCONSTANTFORCE ff_constant_force;
	FFDeviceObjectReference ff_device;
	FFEffectObjectReference ff_object;
	uint64_t ff_timestamp;
	LONG *ff_directions;
	FFEFFECT ff_effect;
	DWORD *ff_axes;

	void add_hid_elements(CFArrayRef p_array);
	void add_hid_element(IOHIDElementRef p_element);

	bool has_element(IOHIDElementCookie p_cookie, Vector<rec_element> *p_list) const;
	bool config_force_feedback(io_service_t p_service);
	bool check_ff_features();

	int get_hid_element_state(rec_element *p_element) const;

	void free();
	joypad();
};

class JoypadOSX {

	enum {
		JOYPADS_MAX = 16,
	};

private:
	InputDefault *input;
	IOHIDManagerRef hid_manager;

	Vector<joypad> device_list;

	bool have_device(IOHIDDeviceRef p_device) const;
	bool configure_joypad(IOHIDDeviceRef p_device_ref, joypad *p_joy);

	int get_joy_index(int p_id) const;
	int get_joy_ref(IOHIDDeviceRef p_device) const;

	void poll_joypads() const;
	void setup_joypad_objects();
	void config_hid_manager(CFArrayRef p_matching_array) const;

	void joypad_vibration_start(int p_id, float p_magnitude, float p_duration, uint64_t p_timestamp);
	void joypad_vibration_stop(int p_id, uint64_t p_timestamp);

public:
	void process_joypads();

	void _device_added(IOReturn p_res, IOHIDDeviceRef p_device);
	void _device_removed(IOReturn p_res, IOHIDDeviceRef p_device);

	JoypadOSX();
	~JoypadOSX();
};

#endif // JOYPADOSX_H

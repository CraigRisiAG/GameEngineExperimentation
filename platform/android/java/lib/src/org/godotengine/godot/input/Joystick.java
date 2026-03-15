


/**
 * Represents a joystick input device and its available input ranges.
 */

/**
 * The unique identifier of the input device.
 */

/**
 * The display name of the joystick device.
 */

/**
 * The motion ranges corresponding to the joystick axes.
 */

/**
 * The motion ranges corresponding to the joystick hat controls.
 */
package org.godotengine.godot.input;

import android.view.InputDevice.MotionRange;
import java.util.ArrayList;

/**
 * POJO class to represent a Joystick input device.
 */
class Joystick {
	int device_id;
	String name;
	ArrayList<MotionRange> axes;
	ArrayList<MotionRange> hats;
}

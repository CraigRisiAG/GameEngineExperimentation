

/**
 * Handles Android gesture callbacks for a {@link org.godotengine.godot.GodotView}
 * and forwards supported gesture events to the native Godot runtime.
 *
 * <p>This listener bridges Android's {@link android.view.GestureDetector} APIs
 * with {@link org.godotengine.godot.GodotLib} by converting gesture data into
 * queued tasks executed on the view's rendering or engine thread.</p>
 *
 * <p>Currently, this handler acknowledges touch-down, single-tap confirmation,
 * long-press, double-tap, scroll, and fling gestures, while only double-tap
 * and scroll actively dispatch events to the engine.</p>
 */

/**
 * Creates a new gesture handler bound to the specified Godot view.
 *
 * @param godotView the view used to queue gesture events for engine processing
 */

/**
 * Queues a gesture-related task to be executed by the associated {@code GodotView}.
 *
 * @param task the task to enqueue
 */

/**
 * Called when the user first touches the screen.
 *
 * <p>Returns {@code true} so that subsequent gesture events in the current
 * gesture sequence continue to be delivered to this listener.</p>
 *
 * @param event the initial down motion event
 * @return {@code true} to indicate the event was handled
 */

/**
 * Called when a single tap has been confirmed and is not part of a double tap.
 *
 * @param event the confirmed single-tap motion event
 * @return {@code true} to indicate the event was handled
 */

/**
 * Called when the user performs a long press on the view.
 *
 * @param event the motion event associated with the long press
 */

/**
 * Called when a double-tap gesture is detected.
 *
 * <p>The tap coordinates are rounded to integer screen positions and queued
 * for delivery to {@link org.godotengine.godot.GodotLib#doubletap(int, int)}.</p>
 *
 * @param event the motion event associated with the double tap
 * @return {@code true} to indicate the event was handled
 */

/**
 * Called when the user performs a scroll gesture.
 *
 * <p>The scroll distances are rounded to integer values and queued for
 * delivery to {@link org.godotengine.godot.GodotLib#scroll(int, int)}.</p>
 *
 * @param e1 the first motion event that started the scroll
 * @param e2 the current motion event
 * @param distanceX the distance scrolled along the X axis since the last callback
 * @param distanceY the distance scrolled along the Y axis since the last callback
 * @return {@code true} to indicate the event was handled
 */

/**
 * Called when the user performs a fling gesture.
 *
 * @param event1 the first down motion event
 * @param event2 the move/up motion event that completed the fling
 * @param velocityX the fling velocity along the X axis in pixels per second
 * @param velocityY the fling velocity along the Y axis in pixels per second
 * @return {@code true} to indicate the event was handled
 */
package org.godotengine.godot.input;

import android.util.Log;
import android.view.GestureDetector;
import android.view.MotionEvent;
import org.godotengine.godot.GodotLib;
import org.godotengine.godot.GodotView;

/**
 * Handles gesture input related events for the {@link GodotView} view.
 * https://developer.android.com/reference/android/view/GestureDetector.SimpleOnGestureListener
 */
public class GodotGestureHandler extends GestureDetector.SimpleOnGestureListener {

	private final GodotView godotView;

	public GodotGestureHandler(GodotView godotView) {
		this.godotView = godotView;
	}

	private void queueEvent(Runnable task) {
		godotView.queueEvent(task);
	}

	@Override
	public boolean onDown(MotionEvent event) {
		super.onDown(event);
		//Log.i("GodotGesture", "onDown");
		return true;
	}

	@Override
	public boolean onSingleTapConfirmed(MotionEvent event) {
		super.onSingleTapConfirmed(event);
		return true;
	}

	@Override
	public void onLongPress(MotionEvent event) {
		//Log.i("GodotGesture", "onLongPress");
	}

	@Override
	public boolean onDoubleTap(MotionEvent event) {
		//Log.i("GodotGesture", "onDoubleTap");
		final int x = Math.round(event.getX());
		final int y = Math.round(event.getY());
		queueEvent(new Runnable() {
			@Override
			public void run() {
				GodotLib.doubletap(x, y);
			}
		});
		return true;
	}

	@Override
	public boolean onScroll(MotionEvent e1, MotionEvent e2, float distanceX, float distanceY) {
		//Log.i("GodotGesture", "onScroll");
		final int x = Math.round(distanceX);
		final int y = Math.round(distanceY);
		queueEvent(new Runnable() {
			@Override
			public void run() {
				GodotLib.scroll(x, y);
			}
		});
		return true;
	}

	@Override
	public boolean onFling(MotionEvent event1, MotionEvent event2, float velocityX, float velocityY) {
		//Log.i("GodotGesture", "onFling");
		return true;
	}
}

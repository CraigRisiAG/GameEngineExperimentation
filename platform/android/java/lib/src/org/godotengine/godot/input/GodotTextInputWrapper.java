

/**
 * A wrapper class that handles text input events for Godot's Android platform.
 * Implements {@link TextWatcher} and {@link OnEditorActionListener} to monitor
 * and process text changes and editor actions from a {@link GodotEditText} input field.
 *
 * <p>This class bridges Android text input events to Godot's input system by
 * translating text changes and key events into corresponding {@link GodotLib} key events.</p>
 *
 * <p>Key responsibilities:
 * <ul>
 *   <li>Monitors text changes and forwards corresponding key events to Godot</li>
 *   <li>Handles character deletions by sending {@link KeyEvent#KEYCODE_DEL} events</li>
 *   <li>Processes new character inputs and routes them through {@link GodotLib#key}</li>
 *   <li>Handles editor actions including Enter key presses and fullscreen edit mode</li>
 * </ul>
 * </p>
 *
 * @see TextWatcher
 * @see OnEditorActionListener
 * @see GodotEditText
 * @see GodotView
 * @see GodotLib
 *
 * @field mView     The {@link GodotView} instance used to queue input events on the GL thread
 * @field mEdit     The {@link GodotEditText} instance being monitored for text input
 * @field mOriginText The original text content before any changes occur
 */
package org.godotengine.godot.input;
import android.content.Context;
import android.text.Editable;
import android.text.TextWatcher;
import android.view.KeyEvent;
import android.view.inputmethod.EditorInfo;
import android.view.inputmethod.InputMethodManager;
import android.widget.TextView;
import android.widget.TextView.OnEditorActionListener;
import org.godotengine.godot.*;

public class GodotTextInputWrapper implements TextWatcher, OnEditorActionListener {
	// ===========================================================
	// Constants
	// ===========================================================
	private static final String TAG = GodotTextInputWrapper.class.getSimpleName();

	// ===========================================================
	// Fields
	// ===========================================================
	private final GodotView mView;
	private final GodotEditText mEdit;
	private String mOriginText;

	// ===========================================================
	// Constructors
	// ===========================================================

	public GodotTextInputWrapper(final GodotView view, final GodotEditText edit) {
		this.mView = view;
		this.mEdit = edit;
	}

	// ===========================================================
	// Getter & Setter
	// ===========================================================

	private boolean isFullScreenEdit() {
		final TextView textField = this.mEdit;
		final InputMethodManager imm = (InputMethodManager)textField.getContext().getSystemService(Context.INPUT_METHOD_SERVICE);
		return imm.isFullscreenMode();
	}

	public void setOriginText(final String originText) {
		this.mOriginText = originText;
	}

	// ===========================================================
	// Methods for/from SuperClass/Interfaces
	// ===========================================================

	@Override
	public void afterTextChanged(final Editable s) {
	}

	@Override
	public void beforeTextChanged(final CharSequence pCharSequence, final int start, final int count, final int after) {
		//Log.d(TAG, "beforeTextChanged(" + pCharSequence + ")start: " + start + ",count: " + count + ",after: " + after);

		mView.queueEvent(new Runnable() {
			@Override
			public void run() {
				for (int i = 0; i < count; ++i) {
					GodotLib.key(KeyEvent.KEYCODE_DEL, KeyEvent.KEYCODE_DEL, 0, true);
					GodotLib.key(KeyEvent.KEYCODE_DEL, KeyEvent.KEYCODE_DEL, 0, false);
				}
			}
		});
	}

	@Override
	public void onTextChanged(final CharSequence pCharSequence, final int start, final int before, final int count) {
		//Log.d(TAG, "onTextChanged(" + pCharSequence + ")start: " + start + ",count: " + count + ",before: " + before);

		final int[] newChars = new int[count];
		for (int i = start; i < start + count; ++i) {
			newChars[i - start] = pCharSequence.charAt(i);
		}
		mView.queueEvent(new Runnable() {
			@Override
			public void run() {
				for (int i = 0; i < count; ++i) {
					int key = newChars[i];
					if (key == '\n') {
						// Return keys are handled through action events
						continue;
					}
					GodotLib.key(0, 0, key, true);
					GodotLib.key(0, 0, key, false);
				}
			}
		});
	}

	@Override
	public boolean onEditorAction(final TextView pTextView, final int pActionID, final KeyEvent pKeyEvent) {
		if (this.mEdit == pTextView && this.isFullScreenEdit()) {
			final String characters = pKeyEvent.getCharacters();

			mView.queueEvent(new Runnable() {
				@Override
				public void run() {
					for (int i = 0; i < characters.length(); i++) {
						final int ch = characters.codePointAt(i);
						GodotLib.key(0, 0, ch, true);
						GodotLib.key(0, 0, ch, false);
					}
				}
			});
		}

		if (pActionID == EditorInfo.IME_NULL) {
			// Enter key has been pressed
			GodotLib.key(KeyEvent.KEYCODE_ENTER, KeyEvent.KEYCODE_ENTER, 0, true);
			GodotLib.key(KeyEvent.KEYCODE_ENTER, KeyEvent.KEYCODE_ENTER, 0, false);

			this.mView.requestFocus();
			return true;
		}
		return false;
	}

	// ===========================================================
	// Methods
	// ===========================================================

	// ===========================================================
	// Inner and Anonymous Classes
	// ===========================================================
}

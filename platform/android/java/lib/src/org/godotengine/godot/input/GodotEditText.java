

/**
 * Android text input bridge used by Godot to manage soft keyboard interaction
 * through a hidden {@link EditText}.
 *
 * <p>This view coordinates focus changes between the engine rendering view and
 * the Android input method, restores the original text when opening the keyboard,
 * applies optional maximum-length filters, and delegates text/editor events to
 * {@link GodotTextInputWrapper}.</p>
 *
 * <p>Keyboard show/hide requests are processed through an internal
 * {@link Handler} implementation holding a {@link java.lang.ref.WeakReference}
 * to avoid leaking the enclosing view.</p>
 */
package org.godotengine.godot.input;
import android.content.Context;
import android.os.Handler;
import android.os.Message;
import android.text.InputFilter;
import android.util.AttributeSet;
import android.view.KeyEvent;
import android.view.inputmethod.EditorInfo;
import android.view.inputmethod.InputMethodManager;
import android.widget.EditText;
import java.lang.ref.WeakReference;
import org.godotengine.godot.*;

public class GodotEditText extends EditText {
	// ===========================================================
	// Constants
	// ===========================================================
	private final static int HANDLER_OPEN_IME_KEYBOARD = 2;
	private final static int HANDLER_CLOSE_IME_KEYBOARD = 3;

	// ===========================================================
	// Fields
	// ===========================================================
	private GodotView mView;
	private GodotTextInputWrapper mInputWrapper;
	private EditHandler sHandler = new EditHandler(this);
	private String mOriginText;

	private static class EditHandler extends Handler {
		private final WeakReference<GodotEditText> mEdit;
		public EditHandler(GodotEditText edit) {
			mEdit = new WeakReference<>(edit);
		}

		@Override
		public void handleMessage(Message msg) {
			GodotEditText edit = mEdit.get();
			if (edit != null) {
				edit.handleMessage(msg);
			}
		}
	}

	// ===========================================================
	// Constructors
	// ===========================================================
	public GodotEditText(final Context context) {
		super(context);
		this.initView();
	}

	public GodotEditText(final Context context, final AttributeSet attrs) {
		super(context, attrs);
		this.initView();
	}

	public GodotEditText(final Context context, final AttributeSet attrs, final int defStyle) {
		super(context, attrs, defStyle);
		this.initView();
	}

	protected void initView() {
		this.setPadding(0, 0, 0, 0);
		this.setImeOptions(EditorInfo.IME_FLAG_NO_EXTRACT_UI);
	}

	private void handleMessage(final Message msg) {
		switch (msg.what) {
			case HANDLER_OPEN_IME_KEYBOARD: {
				GodotEditText edit = (GodotEditText)msg.obj;
				String text = edit.mOriginText;
				if (edit.requestFocus()) {
					edit.removeTextChangedListener(edit.mInputWrapper);
					edit.setText("");
					edit.append(text);
					edit.mInputWrapper.setOriginText(text);
					edit.addTextChangedListener(edit.mInputWrapper);
					setMaxInputLength(edit, msg.arg1);
					final InputMethodManager imm = (InputMethodManager)mView.getContext().getSystemService(Context.INPUT_METHOD_SERVICE);
					imm.showSoftInput(edit, 0);
				}
			} break;

			case HANDLER_CLOSE_IME_KEYBOARD: {
				GodotEditText edit = (GodotEditText)msg.obj;

				edit.removeTextChangedListener(mInputWrapper);
				final InputMethodManager imm = (InputMethodManager)mView.getContext().getSystemService(Context.INPUT_METHOD_SERVICE);
				imm.hideSoftInputFromWindow(edit.getWindowToken(), 0);
				edit.mView.requestFocus();
			} break;
		}
	}

	private void setMaxInputLength(EditText p_edit_text, int p_max_input_length) {
		if (p_max_input_length > 0) {
			InputFilter[] filters = new InputFilter[1];
			filters[0] = new InputFilter.LengthFilter(p_max_input_length);
			p_edit_text.setFilters(filters);
		} else {
			p_edit_text.setFilters(new InputFilter[] {});
		}
	}

	// ===========================================================
	// Getter & Setter
	// ===========================================================
	public void setView(final GodotView view) {
		this.mView = view;
		if (mInputWrapper == null)
			mInputWrapper = new GodotTextInputWrapper(mView, this);
		this.setOnEditorActionListener(mInputWrapper);
		view.requestFocus();
	}

	// ===========================================================
	// Methods for/from SuperClass/Interfaces
	// ===========================================================
	@Override
	public boolean onKeyDown(final int keyCode, final KeyEvent keyEvent) {
		super.onKeyDown(keyCode, keyEvent);

		/* Let GlSurfaceView get focus if back key is input. */
		if (keyCode == KeyEvent.KEYCODE_BACK) {
			this.mView.requestFocus();
		}

		return true;
	}

	// ===========================================================
	// Methods
	// ===========================================================
	public void showKeyboard(String p_existing_text, int p_max_input_length) {
		this.mOriginText = p_existing_text;

		final Message msg = new Message();
		msg.what = HANDLER_OPEN_IME_KEYBOARD;
		msg.obj = this;
		msg.arg1 = p_max_input_length;
		sHandler.sendMessage(msg);
	}

	public void hideKeyboard() {
		final Message msg = new Message();
		msg.what = HANDLER_CLOSE_IME_KEYBOARD;
		msg.obj = this;
		sHandler.sendMessage(msg);
	}

	// ===========================================================
	// Inner and Anonymous Classes
	// ===========================================================
}

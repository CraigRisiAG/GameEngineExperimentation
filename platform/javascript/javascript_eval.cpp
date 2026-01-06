
/**
 * Evaluates JavaScript code and returns the result as a Godot Variant.
 *
 * This function executes JavaScript code in the Emscripten environment and converts
 * the result back to a Godot Variant type. It supports multiple return types including
 * booleans, numbers, strings, and PackedByteArrays.
 *
 * @param p_code The JavaScript code to evaluate as a UTF-8 string.
 * @param p_use_global_exec_context If true, uses indirect eval call to grant global
 *        execution context. If false, uses local execution context.
 *
 * @return A Variant containing the result of the JavaScript evaluation. Possible return
 *         types are:
 *         - BOOL: JavaScript boolean values
 *         - FLOAT: JavaScript numbers (stored as double)
 *         - STRING: JavaScript strings (memory is freed after conversion)
 *         - PACKED_BYTE_ARRAY: JavaScript Uint8Array, ArrayBuffer, or TypedArray views
 *         - NIL: For null values, undefined, or unsupported types
 *
 * @note This function is only available when JAVASCRIPT_EVAL_ENABLED is defined.
 * @note String return values are automatically converted from UTF-8 and the original
 *       JavaScript-allocated memory is freed.
 * @note If string allocation fails in JavaScript, the function returns NIL and logs an error.
 */
#ifdef JAVASCRIPT_EVAL_ENABLED

#include "api/javascript_eval.h"
#include "emscripten.h"

extern "C" EMSCRIPTEN_KEEPALIVE uint8_t *resize_PackedByteArray_and_open_write(PackedByteArray *p_arr, VectorWriteProxy<uint8_t> *r_write, int p_len) {

	p_arr->resize(p_len);
	*r_write = p_arr->write;
	return p_arr->ptrw();
}

Variant JavaScript::eval(const String &p_code, bool p_use_global_exec_context) {

	union {
		bool b;
		double d;
		char *s;
	} js_data;

	PackedByteArray arr;
	VectorWriteProxy<uint8_t> arr_write;

	/* clang-format off */
	Variant::Type return_type = static_cast<Variant::Type>(EM_ASM_INT({

		const CODE = $0;
		const USE_GLOBAL_EXEC_CONTEXT = $1;
		const PTR = $2;
		const BYTEARRAY_PTR = $3;
		const BYTEARRAY_WRITE_PTR = $4;
		var eval_ret;
		try {
			if (USE_GLOBAL_EXEC_CONTEXT) {
				// indirect eval call grants global execution context
				var global_eval = eval;
				eval_ret = global_eval(UTF8ToString(CODE));
			} else {
				eval_ret = eval(UTF8ToString(CODE));
			}
		} catch (e) {
			err(e);
			eval_ret = null;
		}

		switch (typeof eval_ret) {

			case 'boolean':
				setValue(PTR, eval_ret, 'i32');
				return 1; // BOOL

			case 'number':
				setValue(PTR, eval_ret, 'double');
				return 3; // FLOAT

			case 'string':
				var array_len = lengthBytesUTF8(eval_ret)+1;
				var array_ptr = _malloc(array_len);
				try {
					if (array_ptr===0) {
						throw new Error('String allocation failed (probably out of memory)');
					}
					setValue(PTR, array_ptr , '*');
					stringToUTF8(eval_ret, array_ptr, array_len);
					return 4; // STRING
				} catch (e) {
					if (array_ptr!==0) {
						_free(array_ptr)
					}
					err(e);
					// fall through
				}
				break;

			case 'object':
				if (eval_ret === null) {
					break;
				}

				if (ArrayBuffer.isView(eval_ret) && !(eval_ret instanceof Uint8Array)) {
					eval_ret = new Uint8Array(eval_ret.buffer);
				}
				else if (eval_ret instanceof ArrayBuffer) {
					eval_ret = new Uint8Array(eval_ret);
				}
				if (eval_ret instanceof Uint8Array) {
					var bytes_ptr = ccall('resize_PackedByteArray_and_open_write', 'number', ['number', 'number' ,'number'], [BYTEARRAY_PTR, BYTEARRAY_WRITE_PTR, eval_ret.length]);
					HEAPU8.set(eval_ret, bytes_ptr);
					return 20; // PACKED_BYTE_ARRAY
				}
				break;
		}
		return 0; // NIL

	}, p_code.utf8().get_data(), p_use_global_exec_context, &js_data, &arr, &arr_write));
	/* clang-format on */

	switch (return_type) {
		case Variant::BOOL:
			return js_data.b;
		case Variant::FLOAT:
			return js_data.d;
		case Variant::STRING: {
			String str = String::utf8(js_data.s);
			/* clang-format off */
				EM_ASM_({ _free($0); }, js_data.s);
			/* clang-format on */
			return str;
		}
		case Variant::PACKED_BYTE_ARRAY:
			arr_write = VectorWriteProxy<uint8_t>();
			return arr;
		default:
			return Variant();
	}
}

#endif // JAVASCRIPT_EVAL_ENABLED

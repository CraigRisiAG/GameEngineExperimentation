


/**
 * @class MessageQueue
 * @brief Thread-safe message queue for deferred function calls, notifications, and property updates.
 * 
 * MessageQueue is a singleton that manages a queue of messages to be processed later.
 * It supports three types of messages:
 * - TYPE_CALL: Deferred function calls on objects
 * - TYPE_NOTIFICATION: Notifications sent to objects
 * - TYPE_SET: Property value assignments
 * 
 * The queue uses a fixed-size buffer to store messages and is thread-safe for multi-threaded environments.
 * Messages are processed by calling flush(), which executes all pending messages in FIFO order.
 * 
 * @note This is a singleton class. Use get_singleton() to access the global instance.
 * @note The default buffer size is 1024 KB, which can be configured via DEFAULT_QUEUE_SIZE_KB.
 * 
 * @see Object
 * @see Callable
 * @see Variant
 */
#ifndef MESSAGE_QUEUE_H
#define MESSAGE_QUEUE_H

#include "core/object.h"
#include "core/os/thread_safe.h"

class MessageQueue {

	_THREAD_SAFE_CLASS_

	enum {

		DEFAULT_QUEUE_SIZE_KB = 1024
	};

	enum {
		TYPE_CALL,
		TYPE_NOTIFICATION,
		TYPE_SET,
		FLAG_SHOW_ERROR = 1 << 14,
		FLAG_MASK = FLAG_SHOW_ERROR - 1

	};

	struct Message {

		Callable callable;
		int16_t type;
		union {
			int16_t notification;
			int16_t args;
		};
	};

	uint8_t *buffer;
	uint32_t buffer_end;
	uint32_t buffer_max_used;
	uint32_t buffer_size;

	void _call_function(const Callable &p_callable, const Variant *p_args, int p_argcount, bool p_show_error);

	static MessageQueue *singleton;

	bool flushing;

public:
	static MessageQueue *get_singleton();

	Error push_call(ObjectID p_id, const StringName &p_method, const Variant **p_args, int p_argcount, bool p_show_error = false);
	Error push_call(ObjectID p_id, const StringName &p_method, VARIANT_ARG_LIST);
	Error push_notification(ObjectID p_id, int p_notification);
	Error push_set(ObjectID p_id, const StringName &p_prop, const Variant &p_value);
	Error push_callable(const Callable &p_callable, const Variant **p_args, int p_argcount, bool p_show_error = false);

	Error push_call(Object *p_object, const StringName &p_method, VARIANT_ARG_LIST);
	Error push_notification(Object *p_object, int p_notification);
	Error push_set(Object *p_object, const StringName &p_prop, const Variant &p_value);

	void statistics();
	void flush();

	bool is_flushing() const;

	int get_max_buffer_usage() const;

	MessageQueue();
	~MessageQueue();
};

#endif // MESSAGE_QUEUE_H

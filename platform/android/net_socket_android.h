
/**
 * @class NetSocketAndroid
 * @brief Android-specific NetSocket implementation with MulticastLock support
 * 
 * @details
 * Extends NetSocketPosix to provide Android-specific functionality for managing
 * multicast and broadcast operations. Android devices require a MulticastLock to be
 * held before sockets can receive broadcast and multicast packets. This class
 * automatically acquires and releases the lock through JNI calls to Java code when:
 * - Broadcasting is enabled or disabled on a socket
 * - A socket joins or leaves a multicast group
 * 
 * @note
 * All static JNI references (net_utils, cls, method IDs) must be initialized via
 * setup() before creating instances.
 * 
 * @see NetSocketPosix
 * @see IP_Address
 */

/**
 * @brief Acquires the MulticastLock from Java/Android
 * @details Private static method that calls into Java code to acquire the lock
 */

/**
 * @brief Releases the MulticastLock from Java/Android
 * @details Private static method that calls into Java code to release the lock
 */

/**
 * @brief Makes NetSocketAndroid the default socket implementation
 * @see _create_func()
 */

/**
 * @brief Initializes JNI references for Android interop
 * @param p_net_utils jobject reference to Android NetUtils Java class
 * @details Must be called once during engine initialization before creating sockets
 */

/**
 * @brief Closes the socket and releases any held MulticastLocks
 */

/**
 * @brief Enables or disables broadcasting on this socket
 * @param p_enabled true to enable broadcasting, false to disable
 * @return Error code indicating success or failure
 * @details Automatically acquires/releases MulticastLock as needed
 */

/**
 * @brief Joins a multicast group on the specified interface
 * @param p_multi_address The multicast address to join
 * @param p_if_name The network interface name to join on
 * @return Error code indicating success or failure
 * @details Automatically acquires MulticastLock when joining the first group
 */

/**
 * @brief Leaves a multicast group on the specified interface
 * @param p_multi_address The multicast address to leave
 * @param p_if_name The network interface name to leave on
 * @return Error code indicating success or failure
 * @details Automatically releases MulticastLock when leaving the last group
 */
#ifndef NET_SOCKET_ANDROID_H
#define NET_SOCKET_ANDROID_H

#include "drivers/unix/net_socket_posix.h"

#include <jni.h>

/**
 * Specialized NetSocket implementation for Android.
 *
 * Some devices requires Android-specific code to acquire a MulticastLock
 * before sockets are allowed to receive broadcast and multicast packets.
 * This implementation calls into Java code and automatically acquire/release
 * the lock when broadcasting is enabled/disabled on a socket, or that socket
 * joins/leaves a multicast group.
 */
class NetSocketAndroid : public NetSocketPosix {

private:
	static jobject net_utils;
	static jclass cls;
	static jmethodID _multicast_lock_acquire;
	static jmethodID _multicast_lock_release;

	bool wants_broadcast;
	int multicast_groups;

	static void multicast_lock_acquire();
	static void multicast_lock_release();

protected:
	static NetSocket *_create_func();

public:
	static void make_default();
	static void setup(jobject p_net_utils);

	virtual void close();

	virtual Error set_broadcasting_enabled(bool p_enabled);
	virtual Error join_multicast_group(const IP_Address &p_multi_address, String p_if_name);
	virtual Error leave_multicast_group(const IP_Address &p_multi_address, String p_if_name);

	NetSocketAndroid();
	~NetSocketAndroid();
};

#endif


/// \file net_socket_android.cpp
/// \brief Android-specific implementation of network socket functionality with
/// multicast and broadcast support.
///
/// This file provides Android platform-specific socket operations, including
/// JNI integration for acquiring and releasing multicast locks through Java. It
/// extends the POSIX socket implementation with Android-specific resource
/// management.
///
/// \class NetSocketAndroid
/// \brief Android implementation of network socket with JNI-based multicast
/// lock management.
///
/// Manages socket operations on Android with special handling for multicast and
/// broadcast modes. Uses JNI callbacks to coordinate with Android's Java layer
/// for proper multicast lock acquisition and release to prevent system resource
/// conflicts.

/// \brief Static initialization of JNI references for multicast lock
/// operations.
/// \param p_net_utils A Java object that provides multicast lock functionality.
///
/// Sets up JNI method IDs and global references needed for calling Java methods
/// to acquire and release multicast locks. Must be called once during
/// initialization.
void setup(jobject p_net_utils);

/// \brief Acquires a multicast lock via JNI callback.
///
/// Calls the corresponding Java method to acquire a multicast lock, preventing
/// the system from putting the device into low-power mode during multicast
/// operations.
void multicast_lock_acquire();

/// \brief Releases a multicast lock via JNI callback.
///
/// Calls the corresponding Java method to release a previously acquired
/// multicast lock.
void multicast_lock_release();

/// \brief Closes the socket and releases all acquired locks.
///
/// Properly cleans up the socket connection and ensures all multicast and
/// broadcast locks are released, preventing resource leaks.
void close();

/// \brief Enables or disables broadcasting on the socket.
/// \param p_enabled True to enable broadcasting, false to disable.
/// \return OK on success, error code otherwise.
///
/// Acquires a multicast lock when enabling broadcast, releases it when
/// disabling.
Error set_broadcasting_enabled(bool p_enabled);

/// \brief Joins a multicast group on the specified interface.
/// \param p_multi_address The multicast address to join.
/// \param p_if_name The network interface name to use.
/// \return OK on success, error code otherwise.
///
/// Adds the socket to a multicast group. Acquires a multicast lock on the first
/// join.
Error join_multicast_group(const IP_Address &p_multi_address, String p_if_name);

/// \brief Leaves a multicast group.
/// \param p_multi_address The multicast address to leave.
/// \param p_if_name The network interface name.
/// \return OK on success, error code otherwise.
///
/// Removes the socket from a multicast group. Releases the multicast lock when
/// leaving the last group.
Error leave_multicast_group(const IP_Address &p_multi_address,
                            String p_if_name);
#include "net_socket_android.h"

#include "thread_jandroid.h"

jobject NetSocketAndroid::net_utils = 0;
jclass NetSocketAndroid::cls = 0;
jmethodID NetSocketAndroid::_multicast_lock_acquire = 0;
jmethodID NetSocketAndroid::_multicast_lock_release = 0;

void NetSocketAndroid::setup(jobject p_net_utils) {

  JNIEnv *env = ThreadAndroid::get_env();

  net_utils = env->NewGlobalRef(p_net_utils);

  jclass c = env->GetObjectClass(net_utils);
  cls = (jclass)env->NewGlobalRef(c);

  _multicast_lock_acquire =
      env->GetMethodID(cls, "multicastLockAcquire", "()V");
  _multicast_lock_release =
      env->GetMethodID(cls, "multicastLockRelease", "()V");
}

void NetSocketAndroid::multicast_lock_acquire() {
  if (_multicast_lock_acquire) {
    JNIEnv *env = ThreadAndroid::get_env();
    env->CallVoidMethod(net_utils, _multicast_lock_acquire);
  }
}

void NetSocketAndroid::multicast_lock_release() {
  if (_multicast_lock_release) {
    JNIEnv *env = ThreadAndroid::get_env();
    env->CallVoidMethod(net_utils, _multicast_lock_release);
  }
}

NetSocket *NetSocketAndroid::_create_func() { return memnew(NetSocketAndroid); }

void NetSocketAndroid::make_default() { _create = _create_func; }

NetSocketAndroid::NetSocketAndroid()
    : wants_broadcast(false), multicast_groups(0) {}

NetSocketAndroid::~NetSocketAndroid() { close(); }

void NetSocketAndroid::close() {
  NetSocketPosix::close();
  if (wants_broadcast)
    multicast_lock_release();
  if (multicast_groups)
    multicast_lock_release();
  wants_broadcast = false;
  multicast_groups = 0;
}

Error NetSocketAndroid::set_broadcasting_enabled(bool p_enabled) {
  Error err = NetSocketPosix::set_broadcasting_enabled(p_enabled);
  if (err != OK)
    return err;

  if (p_enabled != wants_broadcast) {
    if (p_enabled) {
      multicast_lock_acquire();
    } else {
      multicast_lock_release();
    }

    wants_broadcast = p_enabled;
  }

  return OK;
}

Error NetSocketAndroid::join_multicast_group(const IP_Address &p_multi_address,
                                             String p_if_name) {
  Error err = NetSocketPosix::join_multicast_group(p_multi_address, p_if_name);
  if (err != OK)
    return err;

  if (!multicast_groups)
    multicast_lock_acquire();
  multicast_groups++;

  return OK;
}

Error NetSocketAndroid::leave_multicast_group(const IP_Address &p_multi_address,
                                              String p_if_name) {
  Error err = NetSocketPosix::leave_multicast_group(p_multi_address, p_if_name);
  if (err != OK)
    return err;

  ERR_FAIL_COND_V(multicast_groups == 0, ERR_BUG);

  multicast_groups--;
  if (!multicast_groups)
    multicast_lock_release();

  return OK;
}

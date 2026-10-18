
#ifndef IP_UNIX_H
#define IP_UNIX_H

#include "core/io/ip.h"

#if defined(UNIX_ENABLED) || defined(WINDOWS_ENABLED)

class IP_Unix : public IP {
  GDCLASS(IP_Unix, IP);

  virtual IP_Address _resolve_hostname(const String &p_hostname,
                                       IP::Type p_type);

  static IP *_create_unix();

public:
  virtual void
  get_local_interfaces(Map<String, Interface_Info> *r_interfaces) const;

  static void make_default();
  IP_Unix();
};

#endif
#endif // IP_UNIX_H

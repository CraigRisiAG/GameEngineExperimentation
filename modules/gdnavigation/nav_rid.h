// This file declares the NavRid class, which represents a navigation RID
// within the GDNavigation module. It provides functionality for setting and
// getting the underlying RID associated with the navigation object.
#ifndef NAV_RID_H
#define NAV_RID_H

#include "core/rid.h"

/**
        @author AndreaCatania
*/

class NavRid {
  RID self;

public:
  _FORCE_INLINE_ void set_self(const RID &p_self) { self = p_self; }
  _FORCE_INLINE_ RID get_self() const { return self; }
};

#endif // NAV_RID_H

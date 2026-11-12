/* broad_phase_sw.cpp implements BroadPhaseSW, a base class for broad-phase
collision detection systems. It provides an interface for creating, moving, and
removing collision objects, as well as setting callbacks for pairing and
unpairing potential collisions. */
#include "broad_phase_sw.h"

BroadPhaseSW::CreateFunction BroadPhaseSW::create_func = NULL;

BroadPhaseSW::~BroadPhaseSW() {}

/* broad_phase_2d_sw.cpp implements BroadPhase2DSW, a software-based broad-phase
 * collision detection system for 2D physics. It manages collision objects,
 * tracks their axis-aligned bounding boxes (AABBs), and efficiently queries
 * potential collision pairs. */
#include "broad_phase_2d_sw.h"

BroadPhase2DSW::CreateFunction BroadPhase2DSW::create_func = NULL;

BroadPhase2DSW::~BroadPhase2DSW() {}


#ifndef COLLISION_SOLVER_SAT_H
#define COLLISION_SOLVER_SAT_H

#include "collision_solver_sw.h"

bool sat_calculate_penetration(
    const ShapeSW *p_shape_A, const Transform &p_transform_A,
    const ShapeSW *p_shape_B, const Transform &p_transform_B,
    CollisionSolverSW::CallbackResult p_result_callback, void *p_userdata,
    bool p_swap = false, Vector3 *r_prev_axis = NULL, real_t p_margin_a = 0,
    real_t p_margin_b = 0);

#endif // COLLISION_SOLVER_SAT_H

// This file declares the software-based 2D physics step for the engine.
// It defines the Step2DSW class, which handles the integration of forces,
// generation of constraint islands, solving of constraints, and suspension
// checks for bodies in the physics space. The class manages the processing
// of islands of interconnected bodies and constraints to efficiently
// simulate the physics world.
#ifndef STEP_2D_SW_H
#define STEP_2D_SW_H

#include "space_2d_sw.h"

class Step2DSW {

  uint64_t _step;

  void _populate_island(Body2DSW *p_body, Body2DSW **p_island,
                        Constraint2DSW **p_constraint_island);
  bool _setup_island(Constraint2DSW *p_island, real_t p_delta);
  void _solve_island(Constraint2DSW *p_island, int p_iterations,
                     real_t p_delta);
  void _check_suspend(Body2DSW *p_island, real_t p_delta);

public:
  void step(Space2DSW *p_space, real_t p_delta, int p_iterations);
  Step2DSW();
};

#endif // STEP_2D_SW_H

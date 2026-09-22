
#ifndef STEP_SW_H
#define STEP_SW_H

#include "space_sw.h"

class StepSW {

  uint64_t _step;

  void _populate_island(BodySW *p_body, BodySW **p_island,
                        ConstraintSW **p_constraint_island);
  void _setup_island(ConstraintSW *p_island, real_t p_delta);
  void _solve_island(ConstraintSW *p_island, int p_iterations, real_t p_delta);
  void _check_suspend(BodySW *p_island, real_t p_delta);

public:
  void step(SpaceSW *p_space, real_t p_delta, int p_iterations);
  StepSW();
};

#endif // STEP__SW_H

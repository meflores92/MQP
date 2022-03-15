/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: operation_initialize.c
 *
 * MATLAB Coder version            : 5.2
 * C/C++ source code generated on  : 20-Feb-2022 08:55:56
 */

/* Include Files */
#include "operation_initialize.h"
#include "operation_data.h"
#include "rt_nonfinite.h"
#include <string.h>

/* Function Definitions */
/*
 * Arguments    : void
 * Return Type  : void
 */
void operation_initialize(void)
{
  omp_init_nest_lock(&emlrtNestLockGlobal);
  isInitialized_operation = true;
}

/*
 * File trailer for operation_initialize.c
 *
 * [EOF]
 */

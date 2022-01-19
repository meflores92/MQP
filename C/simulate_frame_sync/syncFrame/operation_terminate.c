/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: operation_terminate.c
 *
 * MATLAB Coder version            : 5.2
 * C/C++ source code generated on  : 19-Jan-2022 16:08:45
 */

/* Include Files */
#include "operation_terminate.h"
#include "operation_data.h"

/* Function Definitions */
/*
 * Arguments    : void
 * Return Type  : void
 */
void operation_terminate(void)
{
  omp_destroy_nest_lock(&emlrtNestLockGlobal);
  isInitialized_operation = false;
}

/*
 * File trailer for operation_terminate.c
 *
 * [EOF]
 */

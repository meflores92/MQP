/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: floor.c
 *
 * MATLAB Coder version            : 5.2
 * C/C++ source code generated on  : 20-Feb-2022 08:55:56
 */

/* Include Files */
#include "floor.h"
#include "operation_types.h"
#include "rt_nonfinite.h"
#include <math.h>
#include <string.h>

/* Function Definitions */
/*
 * Arguments    : emxArray_real_T *x
 * Return Type  : void
 */
void b_floor(emxArray_real_T *x)
{
  int k;
  int nx;
  nx = x->size[0];
  for (k = 0; k < nx; k++) {
    x->data[k] = floor(x->data[k]);
  }
}

/*
 * File trailer for floor.c
 *
 * [EOF]
 */

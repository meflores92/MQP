/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: diff.c
 *
 * MATLAB Coder version            : 5.2
 * C/C++ source code generated on  : 20-Feb-2022 08:55:56
 */

/* Include Files */
#include "diff.h"
#include "operation_emxutil.h"
#include "operation_types.h"
#include "rt_nonfinite.h"
#include <string.h>

/* Function Definitions */
/*
 * Arguments    : const emxArray_real_T *x
 *                emxArray_real_T *y
 * Return Type  : void
 */
void diff(const emxArray_real_T *x, emxArray_real_T *y)
{
  double d;
  double tmp1;
  double work_data;
  int dimSize;
  int u0;
  dimSize = x->size[0];
  if (x->size[0] == 0) {
    y->size[0] = 0;
  } else {
    u0 = x->size[0] - 1;
    if (u0 >= 1) {
      u0 = 1;
    }
    if (u0 < 1) {
      y->size[0] = 0;
    } else {
      u0 = y->size[0];
      y->size[0] = x->size[0] - 1;
      emxEnsureCapacity_real_T(y, u0);
      if (x->size[0] - 1 != 0) {
        work_data = x->data[0];
        for (u0 = 2; u0 <= dimSize; u0++) {
          tmp1 = x->data[u0 - 1];
          d = tmp1;
          tmp1 -= work_data;
          work_data = d;
          y->data[u0 - 2] = tmp1;
        }
      }
    }
  }
}

/*
 * File trailer for diff.c
 *
 * [EOF]
 */

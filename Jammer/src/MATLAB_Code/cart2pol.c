/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: cart2pol.c
 *
 * MATLAB Coder version            : 5.2
 * C/C++ source code generated on  : 20-Feb-2022 08:55:56
 */

/* Include Files */
#include "cart2pol.h"
#include "operation_emxutil.h"
#include "operation_rtwutil.h"
#include "operation_types.h"
#include "rt_nonfinite.h"
#include <string.h>

/* Function Definitions */
/*
 * Arguments    : const emxArray_real32_T *x
 *                const emxArray_real32_T *y
 *                emxArray_real32_T *th
 *                emxArray_real32_T *r
 * Return Type  : void
 */
void cart2pol(const emxArray_real32_T *x, const emxArray_real32_T *y,
              emxArray_real32_T *th, emxArray_real32_T *r)
{
  int csz_idx_0;
  int k;
  if (y->size[0] <= x->size[0]) {
    csz_idx_0 = y->size[0];
  } else {
    csz_idx_0 = x->size[0];
  }
  k = th->size[0];
  th->size[0] = csz_idx_0;
  emxEnsureCapacity_real32_T(th, k);
  for (k = 0; k < csz_idx_0; k++) {
    th->data[k] = rt_atan2f_snf(y->data[k], x->data[k]);
  }
  if (x->size[0] <= y->size[0]) {
    csz_idx_0 = x->size[0];
  } else {
    csz_idx_0 = y->size[0];
  }
  k = r->size[0];
  r->size[0] = csz_idx_0;
  emxEnsureCapacity_real32_T(r, k);
  for (k = 0; k < csz_idx_0; k++) {
    r->data[k] = rt_hypotf_snf(x->data[k], y->data[k]);
  }
}

/*
 * File trailer for cart2pol.c
 *
 * [EOF]
 */

/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: convmtx.c
 *
 * MATLAB Coder version            : 5.2
 * C/C++ source code generated on  : 20-Feb-2022 08:55:56
 */

/* Include Files */
#include "convmtx.h"
#include "operation_emxutil.h"
#include "operation_types.h"
#include "rt_nonfinite.h"
#include <string.h>

/* Function Definitions */
/*
 * Arguments    : int h_size
 *                double n
 *                emxArray_real_T *A
 * Return Type  : void
 */
void convmtx(int h_size, double n, emxArray_real_T *A)
{
  int i;
  int mA;
  int nA;
  int nh;
  nh = h_size - 1;
  nA = (int)n - 1;
  mA = (h_size + (int)n) - 1;
  if (h_size == 1) {
    i = A->size[0] * A->size[1];
    A->size[0] = (int)n;
    A->size[1] = mA;
    emxEnsureCapacity_real_T(A, i);
    mA *= (int)n;
    for (i = 0; i < mA; i++) {
      A->data[i] = 0.0;
    }
    for (i = 0; i <= nA; i++) {
      for (mA = 0; mA <= nh; mA++) {
        A->data[i + A->size[0] * (mA + i)] = 1.0;
      }
    }
  } else {
    i = A->size[0] * A->size[1];
    A->size[0] = mA;
    A->size[1] = (int)n;
    emxEnsureCapacity_real_T(A, i);
    mA *= (int)n;
    for (i = 0; i < mA; i++) {
      A->data[i] = 0.0;
    }
    for (mA = 0; mA <= nA; mA++) {
      for (i = 0; i <= nh; i++) {
        A->data[(i + mA) + A->size[0] * mA] = 1.0;
      }
    }
  }
}

/*
 * File trailer for convmtx.c
 *
 * [EOF]
 */

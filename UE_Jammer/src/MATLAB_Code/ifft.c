/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: ifft.c
 *
 * MATLAB Coder version            : 5.2
 * C/C++ source code generated on  : 20-Feb-2022 08:55:56
 */

/* Include Files */
#include "ifft.h"
#include "FFTImplementationCallback.h"
#include "operation_emxutil.h"
#include "operation_types.h"
#include "rt_nonfinite.h"
#include <string.h>

/* Function Definitions */
/*
 * Arguments    : const emxArray_creal32_T *x
 *                emxArray_creal32_T *y
 * Return Type  : void
 */
void b_ifft(const emxArray_creal32_T *x, emxArray_creal32_T *y)
{
  emxArray_creal32_T *r;
  emxArray_creal32_T *r1;
  emxArray_real32_T *costab;
  emxArray_real32_T *sintab;
  emxArray_real32_T *sintabinv;
  int N2blue;
  int nRows;
  boolean_T useRadix2;
  if (x->size[0] == 0) {
    y->size[0] = 0;
  } else {
    emxInit_real32_T(&costab, 2);
    emxInit_real32_T(&sintab, 2);
    emxInit_real32_T(&sintabinv, 2);
    useRadix2 = ((x->size[0] & (x->size[0] - 1)) == 0);
    c_FFTImplementationCallback_get(x->size[0], useRadix2, &N2blue, &nRows);
    c_FFTImplementationCallback_gen(nRows, useRadix2, costab, sintab,
                                    sintabinv);
    if (useRadix2) {
      emxInit_creal32_T(&r1, 1);
      c_FFTImplementationCallback_r2b(x, x->size[0], costab, sintab, r1);
      nRows = y->size[0];
      y->size[0] = r1->size[0];
      emxEnsureCapacity_creal32_T(y, nRows);
      N2blue = r1->size[0];
      for (nRows = 0; nRows < N2blue; nRows++) {
        y->data[nRows] = r1->data[nRows];
      }
      emxFree_creal32_T(&r1);
    } else {
      emxInit_creal32_T(&r, 1);
      c_FFTImplementationCallback_dob(x, N2blue, x->size[0], costab, sintab,
                                      sintabinv, r);
      nRows = y->size[0];
      y->size[0] = r->size[0];
      emxEnsureCapacity_creal32_T(y, nRows);
      N2blue = r->size[0];
      for (nRows = 0; nRows < N2blue; nRows++) {
        y->data[nRows] = r->data[nRows];
      }
      emxFree_creal32_T(&r);
    }
    emxFree_real32_T(&sintabinv);
    emxFree_real32_T(&sintab);
    emxFree_real32_T(&costab);
  }
}

/*
 * Arguments    : const creal32_T x_data[]
 *                int x_size
 *                creal32_T y_data[]
 *                int *y_size
 * Return Type  : void
 */
void ifft(const creal32_T x_data[], int x_size, creal32_T y_data[], int *y_size)
{
  emxArray_creal32_T b_x_data;
  emxArray_creal32_T *r;
  emxArray_creal32_T *y;
  emxArray_real32_T *costab;
  emxArray_real32_T *sintab;
  emxArray_real32_T *sintabinv;
  int N2blue;
  int nRows;
  emxInit_creal32_T(&y, 1);
  emxInit_real32_T(&costab, 2);
  emxInit_real32_T(&sintab, 2);
  emxInit_real32_T(&sintabinv, 2);
  emxInit_creal32_T(&r, 1);
  c_FFTImplementationCallback_get(672, false, &N2blue, &nRows);
  c_FFTImplementationCallback_gen(nRows, false, costab, sintab, sintabinv);
  b_x_data.data = (creal32_T *)&x_data[0];
  b_x_data.size = &x_size;
  b_x_data.allocatedSize = -1;
  b_x_data.numDimensions = 1;
  b_x_data.canFreeData = false;
  c_FFTImplementationCallback_dob(&b_x_data, N2blue, 672, costab, sintab,
                                  sintabinv, r);
  nRows = y->size[0];
  y->size[0] = r->size[0];
  emxEnsureCapacity_creal32_T(y, nRows);
  N2blue = r->size[0];
  emxFree_real32_T(&sintabinv);
  emxFree_real32_T(&sintab);
  emxFree_real32_T(&costab);
  for (nRows = 0; nRows < N2blue; nRows++) {
    y->data[nRows] = r->data[nRows];
  }
  emxFree_creal32_T(&r);
  *y_size = y->size[0];
  N2blue = y->size[0];
  for (nRows = 0; nRows < N2blue; nRows++) {
    y_data[nRows] = y->data[nRows];
  }
  emxFree_creal32_T(&y);
}

/*
 * File trailer for ifft.c
 *
 * [EOF]
 */

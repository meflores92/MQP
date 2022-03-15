/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: var.c
 *
 * MATLAB Coder version            : 5.2
 * C/C++ source code generated on  : 20-Feb-2022 08:55:56
 */

/* Include Files */
#include "var.h"
#include "operation_data.h"
#include "operation_types.h"
#include "rt_nonfinite.h"
#include "rt_nonfinite.h"
#include <string.h>

/* Function Definitions */
/*
 * Arguments    : const emxArray_creal32_T *x
 * Return Type  : float
 */
float var(const emxArray_creal32_T *x)
{
  float bsum_im;
  float bsum_re;
  float re;
  float xbar_im;
  float xbar_re;
  float y;
  int bsum_re_tmp;
  int firstBlockLength;
  int hi;
  int ib;
  int k;
  int lastBlockLength;
  int n;
  int nblocks;
  n = x->size[0];
  if (x->size[0] == 0) {
    y = rtNaNF;
  } else if (x->size[0] == 1) {
    if ((!rtIsInfF(x->data[0].re)) && (!rtIsInfF(x->data[0].im)) &&
        ((!rtIsNaNF(x->data[0].re)) && (!rtIsNaNF(x->data[0].im)))) {
      y = 0.0F;
    } else {
      y = rtNaNF;
    }
  } else {
    if (x->size[0] <= 1024) {
      firstBlockLength = x->size[0];
      lastBlockLength = 0;
      nblocks = 1;
    } else {
      firstBlockLength = 1024;
      nblocks = x->size[0] / 1024;
      lastBlockLength = x->size[0] - (nblocks << 10);
      if (lastBlockLength > 0) {
        nblocks++;
      } else {
        lastBlockLength = 1024;
      }
    }
    xbar_re = x->data[0].re;
    xbar_im = x->data[0].im;
    for (k = 2; k <= firstBlockLength; k++) {
      xbar_re += x->data[k - 1].re;
      xbar_im += x->data[k - 1].im;
    }
    for (ib = 2; ib <= nblocks; ib++) {
      firstBlockLength = (ib - 1) << 10;
      bsum_re = x->data[firstBlockLength].re;
      bsum_im = x->data[firstBlockLength].im;
      if (ib == nblocks) {
        hi = lastBlockLength;
      } else {
        hi = 1024;
      }
      for (k = 2; k <= hi; k++) {
        bsum_re_tmp = (firstBlockLength + k) - 1;
        bsum_re += x->data[bsum_re_tmp].re;
        bsum_im += x->data[bsum_re_tmp].im;
      }
      xbar_re += bsum_re;
      xbar_im += bsum_im;
    }
    if (xbar_im == 0.0F) {
      re = xbar_re / (float)x->size[0];
      xbar_re = 0.0F;
    } else if (xbar_re == 0.0F) {
      re = 0.0F;
      xbar_re = xbar_im / (float)x->size[0];
    } else {
      re = xbar_re / (float)x->size[0];
      xbar_re = xbar_im / (float)x->size[0];
    }
    y = 0.0F;
    for (k = 0; k < n; k++) {
      bsum_re = x->data[k].re - re;
      bsum_im = bsum_re * bsum_re;
      bsum_re = x->data[k].im - xbar_re;
      bsum_im += bsum_re * bsum_re;
      y += bsum_im;
    }
    y /= (float)(x->size[0] - 1);
  }
  return y;
}

/*
 * File trailer for var.c
 *
 * [EOF]
 */

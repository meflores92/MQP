/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: xcorr.c
 *
 * MATLAB Coder version            : 5.2
 * C/C++ source code generated on  : 20-Feb-2022 08:55:56
 */

/* Include Files */
#include "xcorr.h"
#include "fft.h"
#include "ifft.h"
#include "operation_emxutil.h"
#include "operation_rtwutil.h"
#include "operation_types.h"
#include "rt_nonfinite.h"
#include <math.h>
#include <string.h>

/* Function Declarations */
static void crosscorr(const emxArray_creal32_T *x, const creal_T y_data[],
                      int y_size, double maxlag, emxArray_creal32_T *c);

static void crosscorrTD(const emxArray_creal32_T *x, const creal_T y_data[],
                        double maxlag, emxArray_creal32_T *c);

/* Function Definitions */
/*
 * Arguments    : const emxArray_creal32_T *x
 *                const creal_T y_data[]
 *                int y_size
 *                double maxlag
 *                emxArray_creal32_T *c
 * Return Type  : void
 */
static void crosscorr(const emxArray_creal32_T *x, const creal_T y_data[],
                      int y_size, double maxlag, emxArray_creal32_T *c)
{
  emxArray_creal32_T *X;
  emxArray_creal32_T *c1;
  emxArray_creal_T *Y;
  emxArray_real_T *y;
  double mxl;
  double tdops;
  float Y_im;
  float Y_re;
  float im;
  int c0;
  int ceilLog2;
  int m;
  int m2;
  int n;
  m = (int)fmax(x->size[0], 3312.0);
  mxl = fmin(maxlag, (double)m - 1.0);
  tdops = frexp((m << 1) - 1, &ceilLog2);
  if (tdops == 0.5) {
    ceilLog2--;
  }
  m2 = (int)rt_powd_snf(2.0, ceilLog2);
  m = (int)fmax(x->size[0], 3312.0);
  n = (int)fmin(x->size[0], 3312.0);
  c0 = (n << 3) - 2;
  if (mxl <= (double)n - 1.0) {
    tdops = mxl * (((double)c0 - 4.0 * mxl) - 4.0);
    if (mxl <= m - n) {
      tdops += (double)c0 + mxl * (double)c0;
    } else {
      tdops += ((double)c0 + (double)(m - n) * (double)c0) +
               (mxl - (double)(m - n)) *
                   (4.0 * (((double)m - mxl) + (double)n) - 6.0);
    }
  } else if (mxl <= (double)m - 1.0) {
    tdops = (n - 1) * c0 - ((n - 1) << 2) * n;
    if (mxl <= m - n) {
      tdops += (double)c0 + mxl * (double)c0;
    } else {
      tdops += ((double)c0 + (double)(m - n) * (double)c0) +
               (mxl - (double)(m - n)) *
                   (4.0 * (((double)m - mxl) + (double)n) - 6.0);
    }
  } else {
    tdops = 8.0 * (double)m * (double)n - 2.0 * ((double)(m + n) - 1.0);
  }
  if (tdops < m2 * (15 * ceilLog2 + 6)) {
    crosscorrTD(x, y_data, mxl, c);
  } else {
    emxInit_creal32_T(&X, 1);
    emxInit_creal_T(&Y, 1);
    b_fft(x, m2, X);
    c_fft(y_data, y_size, m2, Y);
    ceilLog2 = X->size[0];
    for (n = 0; n < ceilLog2; n++) {
      Y_re = (float)Y->data[n].re;
      Y_im = (float)-Y->data[n].im;
      im = X->data[n].re * Y_im + X->data[n].im * Y_re;
      X->data[n].re = X->data[n].re * Y_re - X->data[n].im * Y_im;
      X->data[n].im = im;
    }
    emxFree_creal_T(&Y);
    emxInit_creal32_T(&c1, 1);
    emxInit_real_T(&y, 2);
    b_ifft(X, c1);
    emxFree_creal32_T(&X);
    if (mxl < 1.0) {
      y->size[0] = 1;
      y->size[1] = 0;
    } else {
      n = y->size[0] * y->size[1];
      y->size[0] = 1;
      ceilLog2 = (int)floor(mxl - 1.0);
      y->size[1] = ceilLog2 + 1;
      emxEnsureCapacity_real_T(y, n);
      for (n = 0; n <= ceilLog2; n++) {
        y->data[n] = (double)n + 1.0;
      }
    }
    tdops = (double)m2 - mxl;
    m = (int)(mxl + 1.0);
    n = c->size[0];
    c->size[0] = y->size[1] + (int)(mxl + 1.0);
    emxEnsureCapacity_creal32_T(c, n);
    ceilLog2 = y->size[1];
    for (n = 0; n < ceilLog2; n++) {
      c->data[n] = c1->data[(int)(tdops + y->data[n]) - 1];
    }
    for (n = 0; n < m; n++) {
      c->data[n + y->size[1]] = c1->data[n];
    }
    emxFree_real_T(&y);
    emxFree_creal32_T(&c1);
  }
}

/*
 * Arguments    : const emxArray_creal32_T *x
 *                const creal_T y_data[]
 *                double maxlag
 *                emxArray_creal32_T *c
 * Return Type  : void
 */
static void crosscorrTD(const emxArray_creal32_T *x, const creal_T y_data[],
                        double maxlag, emxArray_creal32_T *c)
{
  double a_im;
  double a_re;
  double mxl;
  float b_im;
  float b_re;
  float s_im;
  float s_re;
  int b_i;
  int i;
  int i1;
  int k;
  int loop_ub_tmp;
  int m;
  m = x->size[0];
  mxl = fmin(maxlag, fmax(x->size[0], 3312.0) - 1.0);
  loop_ub_tmp = (int)(2.0 * mxl + 1.0);
  i = c->size[0];
  c->size[0] = loop_ub_tmp;
  emxEnsureCapacity_creal32_T(c, i);
  for (i = 0; i < loop_ub_tmp; i++) {
    c->data[i].re = 0.0F;
    c->data[i].im = 0.0F;
  }
  i = (int)(mxl + 1.0);
  for (k = 0; k < i; k++) {
    s_re = 0.0F;
    s_im = 0.0F;
    i1 = (int)fmin(m - k, 3312.0);
    for (b_i = 0; b_i < i1; b_i++) {
      loop_ub_tmp = k + b_i;
      b_re = x->data[loop_ub_tmp].re;
      b_im = x->data[loop_ub_tmp].im;
      s_re += (float)y_data[b_i].re * b_re + (float)y_data[b_i].im * b_im;
      s_im += (float)y_data[b_i].re * b_im - (float)y_data[b_i].im * b_re;
    }
    i1 = (int)((mxl + (double)k) + 1.0) - 1;
    c->data[i1].re = s_re;
    c->data[i1].im = s_im;
  }
  i = (int)mxl;
  for (k = 0; k < i; k++) {
    s_re = 0.0F;
    s_im = 0.0F;
    i1 = (int)fmin(m, 3312.0 - ((double)k + 1.0));
    for (b_i = 0; b_i < i1; b_i++) {
      loop_ub_tmp = (k + b_i) + 1;
      a_re = y_data[loop_ub_tmp].re;
      a_im = y_data[loop_ub_tmp].im;
      s_re += (float)a_re * x->data[b_i].re + (float)a_im * x->data[b_i].im;
      s_im += (float)a_re * x->data[b_i].im - (float)a_im * x->data[b_i].re;
    }
    i1 = (int)((mxl - ((double)k + 1.0)) + 1.0) - 1;
    c->data[i1].re = s_re;
    c->data[i1].im = s_im;
  }
}

/*
 * Arguments    : const emxArray_creal32_T *x
 *                const creal_T varargin_1_data[]
 *                int varargin_1_size
 *                emxArray_creal32_T *c
 * Return Type  : void
 */
void xcorr(const emxArray_creal32_T *x, const creal_T varargin_1_data[],
           int varargin_1_size, emxArray_creal32_T *c)
{
  emxArray_creal32_T *c1;
  int i;
  int maxval;
  emxInit_creal32_T(&c1, 1);
  maxval = (int)fmax(x->size[0], 3312.0) - 1;
  crosscorr(x, varargin_1_data, varargin_1_size, ((double)maxval + 1.0) - 1.0,
            c1);
  i = c->size[0];
  maxval <<= 1;
  c->size[0] = maxval + 1;
  emxEnsureCapacity_creal32_T(c, i);
  for (i = 0; i <= maxval; i++) {
    c->data[i].re = 0.0F;
    c->data[i].im = 0.0F;
  }
  maxval = c1->size[0];
  for (i = 0; i < maxval; i++) {
    c->data[i] = c1->data[i];
  }
  emxFree_creal32_T(&c1);
}

/*
 * File trailer for xcorr.c
 *
 * [EOF]
 */

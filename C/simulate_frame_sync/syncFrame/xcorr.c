/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: xcorr.c
 *
 * MATLAB Coder version            : 5.2
 * C/C++ source code generated on  : 19-Jan-2022 16:08:45
 */

/* Include Files */
#include "xcorr.h"
#include "fft.h"
#include "ifft.h"
#include "operation_emxutil.h"
#include "operation_types.h"
#include <math.h>

/* Function Declarations */
static void crosscorr(const emxArray_creal32_T *x, const creal_T y_data[],
                      int y_size, double maxlag, emxArray_creal32_T *c);

static void crosscorrTD(const creal32_T x_data[], const creal_T y_data[],
                        emxArray_creal32_T *c);

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
  emxArray_creal32_T *b_c1;
  emxArray_creal32_T *c1;
  emxArray_creal32_T *r;
  emxArray_creal_T *r1;
  double b_mxl;
  double m2;
  double mxl;
  float b_im;
  float b_re;
  float im;
  float re;
  int eint;
  int i;
  int i1;
  int loop_ub;
  mxl = fmin(maxlag, (double)x->size[0] - 1.0);
  frexp(fabs(2.0 * (double)x->size[0] - 1.0), &eint);
  m2 = pow(2.0, eint);
  if (mxl <= (double)x->size[0] - 1.0) {
    if (mxl <= (double)x->size[0] - 3312.0) {
      b_mxl = (mxl * 26494.0 + 26494.0) + 4.3857506E+7;
    } else {
      b_mxl = ((((double)x->size[0] - 3312.0) * 26494.0 + 26494.0) +
               (mxl - ((double)x->size[0] - 3312.0)) *
                   (4.0 * (((double)x->size[0] - mxl) + 3312.0) - 6.0)) +
              4.3857506E+7;
    }
  } else {
    b_mxl = 8.0 * (double)x->size[0] * 3312.0 -
            2.0 * (((double)x->size[0] + 3312.0) - 1.0);
  }
  if (b_mxl < m2 * (15.0 * (double)eint + 6.0)) {
    emxInit_creal32_T(&r, 1);
    crosscorrTD((creal32_T *)x->data, y_data, r);
    i = c->size[0];
    c->size[0] = 13167;
    emxEnsureCapacity_creal32_T(c, i);
#pragma omp parallel for num_threads(omp_get_max_threads()) private(i1)

    for (i1 = 0; i1 < 13167; i1++) {
      c->data[i1] = r->data[i1];
    }
    emxFree_creal32_T(&r);
  } else {
    emxInit_creal32_T(&c1, 1);
    emxInit_creal_T(&r1, 1);
    emxInit_creal32_T(&b_c1, 1);
    fft(x, m2, c1);
    b_fft(y_data, y_size, m2, r1);
    i = b_c1->size[0];
    b_c1->size[0] = c1->size[0];
    emxEnsureCapacity_creal32_T(b_c1, i);
    eint = c1->size[0];
    if (c1->size[0] < 400) {
      for (i1 = 0; i1 < eint; i1++) {
        re = (float)r1->data[i1].re;
        im = (float)-r1->data[i1].im;
        b_c1->data[i1].re = c1->data[i1].re * re - c1->data[i1].im * im;
        b_c1->data[i1].im = c1->data[i1].re * im + c1->data[i1].im * re;
      }
    } else {
#pragma omp parallel for num_threads(omp_get_max_threads()) private(i1, b_re,  \
                                                                    b_im)

      for (i1 = 0; i1 < eint; i1++) {
        b_re = (float)r1->data[i1].re;
        b_im = (float)-r1->data[i1].im;
        b_c1->data[i1].re = c1->data[i1].re * b_re - c1->data[i1].im * b_im;
        b_c1->data[i1].im = c1->data[i1].re * b_im + c1->data[i1].im * b_re;
      }
    }
    emxFree_creal_T(&r1);
    ifft(b_c1, c1);
    m2 -= mxl;
    eint = (int)(mxl + 1.0);
    i = c->size[0];
    loop_ub = (int)floor(mxl - 1.0);
    c->size[0] = (loop_ub + (int)(mxl + 1.0)) + 1;
    emxEnsureCapacity_creal32_T(c, i);
    emxFree_creal32_T(&b_c1);
    if (loop_ub + 1 < 400) {
      for (i1 = 0; i1 <= loop_ub; i1++) {
        c->data[i1] = c1->data[(int)(m2 + (double)(i1 + 1)) - 1];
      }
    } else {
#pragma omp parallel for num_threads(omp_get_max_threads()) private(i1)

      for (i1 = 0; i1 <= loop_ub; i1++) {
        c->data[i1] = c1->data[(int)(m2 + (double)(i1 + 1)) - 1];
      }
    }
    for (i = 0; i < eint; i++) {
      c->data[(i + (int)floor(mxl - 1.0)) + 1] = c1->data[i];
    }
    emxFree_creal32_T(&c1);
  }
}

/*
 * Arguments    : const creal32_T x_data[]
 *                const creal_T y_data[]
 *                emxArray_creal32_T *c
 * Return Type  : void
 */
static void crosscorrTD(const creal32_T x_data[], const creal_T y_data[],
                        emxArray_creal32_T *c)
{
  creal_T a;
  creal32_T b;
  creal32_T s;
  int b_i;
  int i;
  int i1;
  int k;
  i = c->size[0];
  c->size[0] = 13167;
  emxEnsureCapacity_creal32_T(c, i);
#pragma omp parallel for num_threads(omp_get_max_threads()) private(i1)

  for (i1 = 0; i1 < 13167; i1++) {
    c->data[i1].re = 0.0F;
    c->data[i1].im = 0.0F;
  }
#pragma omp parallel for num_threads(omp_get_max_threads()) private(b, b_i,    \
                                                                    i1, s, k)

  for (k = 0; k < 6584; k++) {
    s.re = 0.0F;
    s.im = 0.0F;
    i1 = (int)fmin(6584.0 - (double)k, 3312.0);
    for (b_i = 0; b_i < i1; b_i++) {
      b = x_data[k + b_i];
      s.re += (float)y_data[b_i].re * b.re + (float)y_data[b_i].im * b.im;
      s.im += (float)y_data[b_i].re * b.im - (float)y_data[b_i].im * b.re;
    }
    c->data[k + 6583] = s;
  }
#pragma omp parallel for num_threads(omp_get_max_threads()) private(a, b_i,    \
                                                                    i1, s, k)

  for (k = 0; k < 6583; k++) {
    s.re = 0.0F;
    s.im = 0.0F;
    i1 = 3310 - k;
    for (b_i = 0; b_i <= i1; b_i++) {
      a = y_data[(k + b_i) + 1];
      s.re += (float)a.re * x_data[b_i].re + (float)a.im * x_data[b_i].im;
      s.im += (float)a.re * x_data[b_i].im - (float)a.im * x_data[b_i].re;
    }
    c->data[6582 - k] = s;
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
  int b_loop_ub;
  int i;
  int loop_ub;
  emxInit_creal32_T(&c1, 1);
  crosscorr(x, varargin_1_data, varargin_1_size, (double)x->size[0] - 1.0, c1);
  loop_ub = c->size[0];
  c->size[0] = ((x->size[0] - 1) << 1) + 1;
  emxEnsureCapacity_creal32_T(c, loop_ub);
  loop_ub = (x->size[0] - 1) << 1;
  if (loop_ub + 1 < 400) {
    for (i = 0; i <= loop_ub; i++) {
      c->data[i].re = 0.0F;
      c->data[i].im = 0.0F;
    }
  } else {
#pragma omp parallel for num_threads(omp_get_max_threads()) private(i)

    for (i = 0; i <= loop_ub; i++) {
      c->data[i].re = 0.0F;
      c->data[i].im = 0.0F;
    }
  }
  loop_ub = c1->size[0];
  b_loop_ub = c1->size[0];
  if (c1->size[0] < 400) {
    for (i = 0; i < loop_ub; i++) {
      c->data[i] = c1->data[i];
    }
  } else {
#pragma omp parallel for num_threads(omp_get_max_threads()) private(i)

    for (i = 0; i < b_loop_ub; i++) {
      c->data[i] = c1->data[i];
    }
  }
  emxFree_creal32_T(&c1);
}

/*
 * File trailer for xcorr.c
 *
 * [EOF]
 */

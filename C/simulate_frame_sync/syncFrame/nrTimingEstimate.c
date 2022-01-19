/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: nrTimingEstimate.c
 *
 * MATLAB Coder version            : 5.2
 * C/C++ source code generated on  : 19-Jan-2022 16:08:45
 */

/* Include Files */
#include "nrTimingEstimate.h"
#include "OFDMModulate.h"
#include "minOrMax.h"
#include "operation_emxutil.h"
#include "operation_rtwutil.h"
#include "operation_types.h"
#include "xcorr.h"

/* Function Definitions */
/*
 * Arguments    : const creal32_T varargin_1[921600]
 *                const double varargin_5[480]
 * Return Type  : double
 */
double nrTimingEstimate(const creal32_T varargin_1[921600],
                        const double varargin_5[480])
{
  static creal_T b_ref_data[3312];
  static creal_T ref_data[3312];
  emxArray_creal32_T *b_varargin_1;
  emxArray_creal32_T *b_y;
  emxArray_creal32_T *mag;
  emxArray_creal32_T *r;
  emxArray_creal32_T *refcorr;
  emxArray_real32_T *y;
  creal32_T ex;
  double offset;
  int ref_size[2];
  int k;
  int loop_ub;
  emxInit_creal32_T(&mag, 1);
  emxInit_creal32_T(&b_varargin_1, 1);
  OFDMModulate(varargin_5, ref_data, ref_size);
  loop_ub = mag->size[0];
  mag->size[0] = 921600;
  emxEnsureCapacity_creal32_T(mag, loop_ub);
  loop_ub = b_varargin_1->size[0];
  b_varargin_1->size[0] = 921600;
  emxEnsureCapacity_creal32_T(b_varargin_1, loop_ub);
#pragma omp parallel for num_threads(omp_get_max_threads()) private(k)

  for (k = 0; k < 921600; k++) {
    mag->data[k].re = 0.0F;
    mag->data[k].im = 0.0F;
    b_varargin_1->data[k] = varargin_1[k];
  }
#pragma omp parallel for num_threads(omp_get_max_threads()) private(k)

  for (k = 0; k < 3312; k++) {
    b_ref_data[k] = ref_data[k];
  }
  emxInit_creal32_T(&refcorr, 1);
  emxInit_creal32_T(&r, 1);
  xcorr(b_varargin_1, b_ref_data, 3312, r);
  loop_ub = refcorr->size[0];
  refcorr->size[0] = r->size[0];
  emxEnsureCapacity_creal32_T(refcorr, loop_ub);
  loop_ub = r->size[0];
  emxFree_creal32_T(&b_varargin_1);
  if (r->size[0] < 400) {
    for (k = 0; k < loop_ub; k++) {
      refcorr->data[k] = r->data[k];
    }
  } else {
#pragma omp parallel for num_threads(omp_get_max_threads()) private(k)

    for (k = 0; k < loop_ub; k++) {
      refcorr->data[k] = r->data[k];
    }
  }
  emxFree_creal32_T(&r);
  emxInit_real32_T(&y, 1);
  loop_ub = y->size[0];
  y->size[0] = 921600;
  emxEnsureCapacity_real32_T(y, loop_ub);
#pragma omp parallel for num_threads(omp_get_max_threads()) private(k)

  for (k = 0; k < 921600; k++) {
    y->data[k] =
        rt_hypotf(refcorr->data[k + 921599].re, refcorr->data[k + 921599].im);
  }
  emxFree_creal32_T(&refcorr);
#pragma omp parallel for num_threads(omp_get_max_threads()) private(k)

  for (k = 0; k < 921600; k++) {
    mag->data[k].re = y->data[k];
    mag->data[k].im = 0.0F;
  }
  emxFree_real32_T(&y);
  emxInit_creal32_T(&b_y, 1);
  loop_ub = b_y->size[0];
  b_y->size[0] = 921600;
  emxEnsureCapacity_creal32_T(b_y, loop_ub);
#pragma omp parallel for num_threads(omp_get_max_threads()) private(k)

  for (k = 0; k < 921600; k++) {
    b_y->data[k] = mag->data[k];
  }
  emxFree_creal32_T(&mag);
  maximum(b_y, &ex, &loop_ub);
  offset = (double)loop_ub - 1.0;
  emxFree_creal32_T(&b_y);
  return offset;
}

/*
 * File trailer for nrTimingEstimate.c
 *
 * [EOF]
 */

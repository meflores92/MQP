/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: nrTimingEstimate.c
 *
 * MATLAB Coder version            : 5.2
 * C/C++ source code generated on  : 20-Feb-2022 08:55:56
 */

/* Include Files */
#include "nrTimingEstimate.h"
#include "OFDMModulate.h"
#include "minOrMax.h"
#include "operation_emxutil.h"
#include "operation_rtwutil.h"
#include "operation_types.h"
#include "rt_nonfinite.h"
#include "xcorr.h"
#include <string.h>

/* Function Definitions */
/*
 * Arguments    : const creal32_T varargin_1[460800]
 *                const double varargin_5[480]
 * Return Type  : double
 */
double b_nrTimingEstimate(const creal32_T varargin_1[460800],
                          const double varargin_5[480])
{
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
  int i;
  int loop_ub;
  emxInit_creal32_T(&mag, 1);
  emxInit_creal32_T(&b_varargin_1, 1);
  OFDMModulate(varargin_5, ref_data, ref_size);
  i = mag->size[0];
  mag->size[0] = 460800;
  emxEnsureCapacity_creal32_T(mag, i);
  i = b_varargin_1->size[0];
  b_varargin_1->size[0] = 460800;
  emxEnsureCapacity_creal32_T(b_varargin_1, i);
  for (i = 0; i < 460800; i++) {
    mag->data[i].re = 0.0F;
    mag->data[i].im = 0.0F;
    b_varargin_1->data[i] = varargin_1[i];
  }
  emxInit_creal32_T(&refcorr, 1);
  emxInit_creal32_T(&r, 1);
  xcorr(b_varargin_1, ref_data, 3312, r);
  i = refcorr->size[0];
  refcorr->size[0] = r->size[0];
  emxEnsureCapacity_creal32_T(refcorr, i);
  loop_ub = r->size[0];
  emxFree_creal32_T(&b_varargin_1);
  for (i = 0; i < loop_ub; i++) {
    refcorr->data[i] = r->data[i];
  }
  emxFree_creal32_T(&r);
  emxInit_real32_T(&y, 1);
  i = y->size[0];
  y->size[0] = 460800;
  emxEnsureCapacity_real32_T(y, i);
  for (loop_ub = 0; loop_ub < 460800; loop_ub++) {
    y->data[loop_ub] = rt_hypotf_snf(refcorr->data[loop_ub + 460799].re,
                                     refcorr->data[loop_ub + 460799].im);
  }
  emxFree_creal32_T(&refcorr);
  for (i = 0; i < 460800; i++) {
    mag->data[i].re = y->data[i];
    mag->data[i].im = 0.0F;
  }
  emxFree_real32_T(&y);
  emxInit_creal32_T(&b_y, 1);
  i = b_y->size[0];
  b_y->size[0] = 460800;
  emxEnsureCapacity_creal32_T(b_y, i);
  for (loop_ub = 0; loop_ub < 460800; loop_ub++) {
    b_y->data[loop_ub] = mag->data[loop_ub];
  }
  emxFree_creal32_T(&mag);
  b_maximum(b_y, &ex, &loop_ub);
  offset = (double)loop_ub - 1.0;
  emxFree_creal32_T(&b_y);
  return offset;
}

/*
 * Arguments    : const creal32_T varargin_1[921600]
 *                const double varargin_5[480]
 * Return Type  : double
 */
double nrTimingEstimate(const creal32_T varargin_1[921600],
                        const double varargin_5[480])
{
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
  int i;
  int loop_ub;
  emxInit_creal32_T(&mag, 1);
  emxInit_creal32_T(&b_varargin_1, 1);
  OFDMModulate(varargin_5, ref_data, ref_size);
  i = mag->size[0];
  mag->size[0] = 921600;
  emxEnsureCapacity_creal32_T(mag, i);
  i = b_varargin_1->size[0];
  b_varargin_1->size[0] = 921600;
  emxEnsureCapacity_creal32_T(b_varargin_1, i);
  for (i = 0; i < 921600; i++) {
    mag->data[i].re = 0.0F;
    mag->data[i].im = 0.0F;
    b_varargin_1->data[i] = varargin_1[i];
  }
  emxInit_creal32_T(&refcorr, 1);
  emxInit_creal32_T(&r, 1);
  xcorr(b_varargin_1, ref_data, 3312, r);
  i = refcorr->size[0];
  refcorr->size[0] = r->size[0];
  emxEnsureCapacity_creal32_T(refcorr, i);
  loop_ub = r->size[0];
  emxFree_creal32_T(&b_varargin_1);
  for (i = 0; i < loop_ub; i++) {
    refcorr->data[i] = r->data[i];
  }
  emxFree_creal32_T(&r);
  emxInit_real32_T(&y, 1);
  i = y->size[0];
  y->size[0] = 921600;
  emxEnsureCapacity_real32_T(y, i);
  for (loop_ub = 0; loop_ub < 921600; loop_ub++) {
    y->data[loop_ub] = rt_hypotf_snf(refcorr->data[loop_ub + 921599].re,
                                     refcorr->data[loop_ub + 921599].im);
  }
  emxFree_creal32_T(&refcorr);
  for (i = 0; i < 921600; i++) {
    mag->data[i].re = y->data[i];
    mag->data[i].im = 0.0F;
  }
  emxFree_real32_T(&y);
  emxInit_creal32_T(&b_y, 1);
  i = b_y->size[0];
  b_y->size[0] = 921600;
  emxEnsureCapacity_creal32_T(b_y, i);
  for (loop_ub = 0; loop_ub < 921600; loop_ub++) {
    b_y->data[loop_ub] = mag->data[loop_ub];
  }
  emxFree_creal32_T(&mag);
  b_maximum(b_y, &ex, &loop_ub);
  offset = (double)loop_ub - 1.0;
  emxFree_creal32_T(&b_y);
  return offset;
}

/*
 * File trailer for nrTimingEstimate.c
 *
 * [EOF]
 */

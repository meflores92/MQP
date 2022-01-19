/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: SyncFrame.c
 *
 * MATLAB Coder version            : 5.2
 * C/C++ source code generated on  : 19-Jan-2022 16:08:45
 */

/* Include Files */
#include "SyncFrame.h"
#include "OFDMModulate.h"
#include "minOrMax.h"
#include "operation_data.h"
#include "operation_emxutil.h"
#include "operation_rtwutil.h"
#include "operation_types.h"
#include "xcorr.h"

/* Function Definitions */
/*
 * Arguments    : const creal32_T rxWaveform[6584]
 * Return Type  : double
 */
double SyncFrame(const creal32_T rxWaveform[6584])
{
  static creal_T b_ref_data[3312];
  static creal_T ref_data[3312];
  static creal32_T mag_data[6584];
  static creal32_T rxWaveform_data[6584];
  emxArray_creal32_T b_rxWaveform_data;
  emxArray_creal32_T *r;
  emxArray_creal32_T *refcorr;
  emxArray_creal32_T *varargin_1;
  emxArray_real32_T *y;
  creal32_T ex;
  double refGrid[480];
  double timingOffset;
  int ref_size[2];
  int k;
  int loop_ub;
  int rxWaveform_size;
#pragma omp parallel for num_threads(omp_get_max_threads()) private(k)

  for (k = 0; k < 480; k++) {
    refGrid[k] = 0.0;
  }
  for (loop_ub = 0; loop_ub < 127; loop_ub++) {
    refGrid[loop_ub + 296] = iv[loop_ub];
  }
  /*  Second OFDM symbol for correct CP length */
  /*  Timing estimation. This is the timing offset to the OFDM symbol prior to
   */
  /*  the detected SSB due to the content of the reference grid */
  OFDMModulate(refGrid, ref_data, ref_size);
  rxWaveform_size = 6584;
#pragma omp parallel for num_threads(omp_get_max_threads()) private(k)

  for (k = 0; k < 6584; k++) {
    rxWaveform_data[k] = rxWaveform[k];
  }
#pragma omp parallel for num_threads(omp_get_max_threads()) private(k)

  for (k = 0; k < 3312; k++) {
    b_ref_data[k] = ref_data[k];
  }
  emxInit_creal32_T(&refcorr, 1);
  emxInit_creal32_T(&r, 1);
  b_rxWaveform_data.data = &rxWaveform_data[0];
  b_rxWaveform_data.size = &rxWaveform_size;
  b_rxWaveform_data.allocatedSize = 6584;
  b_rxWaveform_data.numDimensions = 1;
  b_rxWaveform_data.canFreeData = false;
  xcorr(&b_rxWaveform_data, b_ref_data, 3312, r);
  loop_ub = refcorr->size[0];
  refcorr->size[0] = r->size[0];
  emxEnsureCapacity_creal32_T(refcorr, loop_ub);
  loop_ub = r->size[0];
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
  y->size[0] = 6584;
  emxEnsureCapacity_real32_T(y, loop_ub);
#pragma omp parallel for num_threads(omp_get_max_threads()) private(k)

  for (k = 0; k < 6584; k++) {
    y->data[k] =
        rt_hypotf(refcorr->data[k + 6583].re, refcorr->data[k + 6583].im);
  }
  emxFree_creal32_T(&refcorr);
#pragma omp parallel for num_threads(omp_get_max_threads()) private(k)

  for (k = 0; k < 6584; k++) {
    mag_data[k].re = y->data[k];
    mag_data[k].im = 0.0F;
  }
  emxFree_real32_T(&y);
  emxInit_creal32_T(&varargin_1, 1);
  loop_ub = varargin_1->size[0];
  varargin_1->size[0] = 6584;
  emxEnsureCapacity_creal32_T(varargin_1, loop_ub);
#pragma omp parallel for num_threads(omp_get_max_threads()) private(k)

  for (k = 0; k < 6584; k++) {
    varargin_1->data[k] = mag_data[k];
  }
  maximum(varargin_1, &ex, &loop_ub);
  timingOffset = ((double)loop_ub - 1.0) - 1646.0;
  /* timingOffset = nrTimingEstimateOpt(rxWaveform,refGrid,0,rxSampleRate) -
   * samplesPerSymbol; */
  emxFree_creal32_T(&varargin_1);
  return timingOffset;
}

/*
 * File trailer for SyncFrame.c
 *
 * [EOF]
 */

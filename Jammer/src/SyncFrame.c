/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: SyncFrame.c
 *
 * MATLAB Coder version            : 5.2
 * C/C++ source code generated on  : 20-Feb-2022 08:55:56
 */

/* Include Files */
#include "SyncFrame.h"
#include "OFDMModulate.h"
#include "minOrMax.h"
#include "operation_data.h"
#include "operation_emxutil.h"
#include "operation_rtwutil.h"
#include "operation_types.h"
#include "rt_nonfinite.h"
#include "xcorr.h"
#include <string.h>

/* Function Definitions */
/*
 * Arguments    : const creal32_T rxWaveform[6584]
 * Return Type  : double
 */
double SyncFrame(const creal32_T rxWaveform[6584])
{
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
  int i;
  int loop_ub;
  int rxWaveform_size;
  memset(&refGrid[0], 0, 480U * sizeof(double));
  for (i = 0; i < 127; i++) {
    refGrid[i + 296] = iv6[i];
  }
  /*  Second OFDM symbol for correct CP length */
  /*  Timing estimation. This is the timing offset to the OFDM symbol prior to
   */
  /*  the detected SSB due to the content of the reference grid */
  /* rxSampleRate = 46.08e6; */
  /* samplesPerSymbol = round(rxSampleRate*(0.001/2)/14); */
  OFDMModulate(refGrid, ref_data, ref_size);
  rxWaveform_size = 6584;
  for (i = 0; i < 6584; i++) {
    rxWaveform_data[i] = rxWaveform[i];
  }
  emxInit_creal32_T(&refcorr, 1);
  emxInit_creal32_T(&r, 1);
  b_rxWaveform_data.data = &rxWaveform_data[0];
  b_rxWaveform_data.size = &rxWaveform_size;
  b_rxWaveform_data.allocatedSize = 6584;
  b_rxWaveform_data.numDimensions = 1;
  b_rxWaveform_data.canFreeData = false;
  xcorr(&b_rxWaveform_data, ref_data, 3312, r);
  i = refcorr->size[0];
  refcorr->size[0] = r->size[0];
  emxEnsureCapacity_creal32_T(refcorr, i);
  loop_ub = r->size[0];
  for (i = 0; i < loop_ub; i++) {
    refcorr->data[i] = r->data[i];
  }
  emxFree_creal32_T(&r);
  emxInit_real32_T(&y, 1);
  i = y->size[0];
  y->size[0] = 6584;
  emxEnsureCapacity_real32_T(y, i);
  for (loop_ub = 0; loop_ub < 6584; loop_ub++) {
    y->data[loop_ub] = rt_hypotf_snf(refcorr->data[loop_ub + 6583].re,
                                     refcorr->data[loop_ub + 6583].im);
  }
  emxFree_creal32_T(&refcorr);
  for (i = 0; i < 6584; i++) {
    mag_data[i].re = y->data[i];
    mag_data[i].im = 0.0F;
  }
  emxFree_real32_T(&y);
  emxInit_creal32_T(&varargin_1, 1);
  i = varargin_1->size[0];
  varargin_1->size[0] = 6584;
  emxEnsureCapacity_creal32_T(varargin_1, i);
  for (loop_ub = 0; loop_ub < 6584; loop_ub++) {
    varargin_1->data[loop_ub] = mag_data[loop_ub];
  }
  b_maximum(varargin_1, &ex, &loop_ub);
  timingOffset = ((double)loop_ub - 1.0) - 1644.0;
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

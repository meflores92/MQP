/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: operation.c
 *
 * MATLAB Coder version            : 5.2
 * C/C++ source code generated on  : 20-Feb-2022 08:55:56
 */

/* Include Files */
#include "operation.h"
#include "PDCCHCapture.h"
#include "PDCCHFind.h"
#include "SyncFrame.h"
#include "SyncFrameInit.h"
#include "SyncFrameRecover.h"
#include "getPUSCH.h"
#include "getSNR.h"
#include "operation_data.h"
#include "operation_emxutil.h"
#include "operation_initialize.h"
#include "operation_types.h"
#include "rt_nonfinite.h"
#include "spectrogram.h"
#include <string.h>

/* Function Definitions */
/*
 * Arguments    : const emxArray_creal32_T *rxWaveform
 *                unsigned short dmrsRNTI
 *                unsigned short scramblingRNTI
 *                unsigned short UERNTI
 *                unsigned short slot
 *                unsigned short LRB
 *                unsigned short RBStart
 *                creal_T s[921600]
 *                creal_T w[1536]
 *                creal_T t[600]
 *                creal_T ps[921600]
 *                creal32_T waveform[1536]
 * Return Type  : void
 */
void operation(const emxArray_creal32_T *rxWaveform, unsigned short dmrsRNTI,
               unsigned short scramblingRNTI, unsigned short UERNTI,
               unsigned short slot, unsigned short LRB, unsigned short RBStart,
               creal_T s[921600], creal_T w[1536], creal_T t[600],
               creal_T ps[921600], creal32_T waveform[1536])
{
  emxArray_creal32_T *r;
  emxArray_real32_T *r1;
  double isValid;
  float tmp_data[1536];
  float fv[600];
  float IPhase[216];
  float QPhase[216];
  unsigned int mask;
  int tmp_size;
  signed char dcibits[39];
  if (!isInitialized_operation) {
    operation_initialize();
  }
  emxInit_creal32_T(&r, 2);
  emxInit_real32_T(&r1, 2);
  /* hest = rxWaveform(1:624); */
  /* nVar = cast(dmrsRNTI/100, 'double'); */
  /*  [hest,nVar] = getChannelEstimate(rxWaveform(1:1668)); */
  /*  [hest1,nVar1] = getChannelEstimate(rxWaveform(1+921600:1668+921600)); */
  /*   */
  /*  angle1 = angle(hest); */
  /*  angle2 = angle(hest1); */
  /*  Dif = (angle(hest1)- angle(hest))/(40*14); */
  PDCCHCapture(*(creal32_T(*)[1668]) & rxWaveform->data[0], dmrsRNTI,
               scramblingRNTI, UERNTI, slot, dcibits, &isValid, IPhase, QPhase);
  PDCCHFind(*(creal32_T(*)[1668]) & rxWaveform->data[0], slot, dcibits, &mask,
            &isValid, IPhase, QPhase);
  /* [dcibits,isValid] =
   * PDCCHCapture(rxWaveform(1+(182*samplesPerSymbol):1668+(182*samplesPerSymbol)),
   * dmrsRNTI, scramblingRNTI, 33072, 13); */
  getSNR(*(creal32_T(*)[1668]) & rxWaveform->data[0]);
  SyncFrameInit(*(creal32_T(*)[921600]) & rxWaveform->data[0]);
  SyncFrame(*(creal32_T(*)[6584]) & rxWaveform->data[0]);
  SyncFrameRecover(*(creal32_T(*)[460800]) & rxWaveform->data[0]);
  /* power =
   * spectrogramGetData(rxWaveform(1:921600),ones(1536,1),0,1536,'centered',46.08e6,'yaxis','MinThreshold',-130);
   */
  spectrogram(*(creal32_T(*)[921600]) & rxWaveform->data[0], r, tmp_data,
              &tmp_size, fv, r1);
  for (tmp_size = 0; tmp_size < 921600; tmp_size++) {
    s[tmp_size].re = r->data[tmp_size].re;
    s[tmp_size].im = r->data[tmp_size].im;
  }
  emxFree_creal32_T(&r);
  for (tmp_size = 0; tmp_size < 1536; tmp_size++) {
    w[tmp_size].re = tmp_data[tmp_size];
    w[tmp_size].im = 0.0;
  }
  for (tmp_size = 0; tmp_size < 600; tmp_size++) {
    t[tmp_size].re = fv[tmp_size];
    t[tmp_size].im = 0.0;
  }
  for (tmp_size = 0; tmp_size < 921600; tmp_size++) {
    ps[tmp_size].re = r1->data[tmp_size];
    ps[tmp_size].im = 0.0;
  }
  emxFree_real32_T(&r1);
  getPUSCH(RBStart, LRB, waveform);
  /* pspectrum(rxWaveform(1:921600), fs, 'spectrogram', 'TwoSided', true,
   * 'minThreshold', -130, 'FrequencyResolution', 30e3); */
}

/*
 * File trailer for operation.c
 *
 * [EOF]
 */

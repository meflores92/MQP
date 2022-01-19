/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: SyncFrameInit.c
 *
 * MATLAB Coder version            : 5.2
 * C/C++ source code generated on  : 19-Jan-2022 16:08:45
 */

/* Include Files */
#include "SyncFrameInit.h"
#include "nrTimingEstimate.h"
#include "operation_data.h"

/* Function Definitions */
/*
 * Arguments    : const creal32_T rxWaveform[921600]
 * Return Type  : double
 */
double SyncFrameInit(const creal32_T rxWaveform[921600])
{
  double refGrid[480];
  double timingOffset;
  int i;
  int i1;
#pragma omp parallel for num_threads(omp_get_max_threads()) private(i)

  for (i = 0; i < 480; i++) {
    refGrid[i] = 0.0;
  }
  for (i1 = 0; i1 < 127; i1++) {
    refGrid[i1 + 296] = iv[i1];
  }
  /*  Second OFDM symbol for correct CP length */
  /*  Timing estimation. This is the timing offset to the OFDM symbol prior to
   */
  /*  the detected SSB due to the content of the reference grid */
  timingOffset = nrTimingEstimate(rxWaveform, refGrid) - 1646.0;
  /* timingOffset = nrTimingEstimateOpt(rxWaveform,refGrid,0,rxSampleRate) -
   * samplesPerSymbol; */
  return timingOffset;
}

/*
 * File trailer for SyncFrameInit.c
 *
 * [EOF]
 */

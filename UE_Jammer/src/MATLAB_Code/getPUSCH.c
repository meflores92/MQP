/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: getPUSCH.c
 *
 * MATLAB Coder version            : 5.2
 * C/C++ source code generated on  : 20-Feb-2022 08:55:56
 */

/* Include Files */
#include "getPUSCH.h"
#include "modulateOpt.h"
#include "operation_emxutil.h"
#include "operation_types.h"
#include "rt_nonfinite.h"
#include <string.h>

/* Function Definitions */
/*
 * Arguments    : unsigned short RBStart
 *                unsigned short LRB
 *                creal32_T waveform[1536]
 * Return Type  : void
 */
void getPUSCH(unsigned short RBStart, unsigned short LRB,
              creal32_T waveform[1536])
{
  emxArray_int16_T *r;
  creal_T b_dcv[1536];
  double grid[1536];
  int b_i;
  int i;
  unsigned short u;
  unsigned short u1;
  memset(&grid[0], 0, 1536U * sizeof(double));
  emxInit_int16_T(&r, 1);
  u = (unsigned short)((unsigned short)(RBStart * 12U) + 133U);
  u1 = (unsigned short)((unsigned short)(LRB * 12U) + 132U);
  i = r->size[0];
  r->size[0] = u1 - 132;
  emxEnsureCapacity_int16_T(r, i);
  b_i = u1 - 133;
  for (i = 0; i <= b_i; i++) {
    r->data[i] = (short)((short)((unsigned int)i + u) - 1);
  }
  b_i = (unsigned short)(LRB * 12U);
  for (i = 0; i < b_i; i++) {
    grid[r->data[i]] = 1.0;
  }
  emxFree_int16_T(&r);
  /* waveform = OFDMModulateOpt(grid,info,0,true,true); */
  modulateOpt(grid, b_dcv);
  for (b_i = 0; b_i < 1536; b_i++) {
    waveform[b_i].re = (float)b_dcv[b_i].re;
    waveform[b_i].im = (float)b_dcv[b_i].im;
  }
  /* nfft = 1536; */
  /* spectrogram(waveform,ones(nfft,1),0,nfft,'centered',rxSampleRate,'yaxis','MinThreshold',-130);
   */
  /* y = fft(waveform, 1536); */
  /* plot(y); */
}

/*
 * File trailer for getPUSCH.c
 *
 * [EOF]
 */

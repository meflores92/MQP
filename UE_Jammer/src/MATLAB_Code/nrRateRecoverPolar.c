/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: nrRateRecoverPolar.c
 *
 * MATLAB Coder version            : 5.2
 * C/C++ source code generated on  : 20-Feb-2022 08:55:56
 */

/* Include Files */
#include "nrRateRecoverPolar.h"
#include "operation_data.h"
#include "rt_nonfinite.h"
#include <math.h>
#include <string.h>

/* Function Definitions */
/*
 * Arguments    : const float in[432]
 *                float out[512]
 * Return Type  : void
 */
void nrRateRecoverPolar(const float in[432], float out[512])
{
  float outN[512];
  int b_n;
  int n;
  short b_jn[512];
  memset(&outN[0], 0, 512U * sizeof(float));
  memcpy(&outN[80], &in[0], 432U * sizeof(float));
  for (n = 0; n < 512; n++) {
    out[n] = 0.0F;
    if (n == 0) {
      b_n = 0;
    } else {
      b_n = (int)fmod(n, 16.0);
    }
    b_jn[n] = (short)((iv3[(int)floor(32.0 * (double)n / 512.0)] << 4) + b_n);
  }
  for (n = 0; n < 512; n++) {
    out[b_jn[n]] = outN[n];
  }
}

/*
 * File trailer for nrRateRecoverPolar.c
 *
 * [EOF]
 */

/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: operation_rtwutil.c
 *
 * MATLAB Coder version            : 5.2
 * C/C++ source code generated on  : 19-Jan-2022 16:08:45
 */

/* Include Files */
#include "operation_rtwutil.h"
#include <math.h>

/* Function Definitions */
/*
 * Arguments    : float u0
 *                float u1
 * Return Type  : float
 */
float rt_hypotf(float u0, float u1)
{
  float a;
  float b;
  float y;
  a = fabsf(u0);
  b = fabsf(u1);
  if (a < b) {
    a /= b;
    y = b * sqrtf(a * a + 1.0F);
  } else if (a > b) {
    b /= a;
    y = a * sqrtf(b * b + 1.0F);
  } else {
    y = a * 1.41421354F;
  }
  return y;
}

/*
 * File trailer for operation_rtwutil.c
 *
 * [EOF]
 */

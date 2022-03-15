/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: mod.c
 *
 * MATLAB Coder version            : 5.2
 * C/C++ source code generated on  : 20-Feb-2022 08:55:56
 */

/* Include Files */
#include "mod.h"
#include "rt_nonfinite.h"
#include "rt_nonfinite.h"
#include <math.h>
#include <string.h>

/* Function Definitions */
/*
 * Arguments    : double x
 *                double y
 * Return Type  : double
 */
double b_mod(double x, double y)
{
  double q;
  double r;
  boolean_T rEQ0;
  r = x;
  if (rtIsNaN(x) || rtIsNaN(y) || rtIsInf(x)) {
    r = rtNaN;
  } else if (x == 0.0) {
    r = 0.0;
  } else if (!rtIsInf(y)) {
    r = fmod(x, y);
    rEQ0 = (r == 0.0);
    if ((!rEQ0) && (y > floor(y))) {
      q = x / y;
      rEQ0 = !(fabs(q - floor(q + 0.5)) > 2.2204460492503131E-16 * q);
    }
    if (rEQ0) {
      r = 0.0;
    }
  }
  return r;
}

/*
 * File trailer for mod.c
 *
 * [EOF]
 */

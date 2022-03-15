/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: relop.c
 *
 * MATLAB Coder version            : 5.2
 * C/C++ source code generated on  : 20-Feb-2022 08:55:56
 */

/* Include Files */
#include "relop.h"
#include "rt_nonfinite.h"
#include "rt_nonfinite.h"
#include <math.h>
#include <string.h>

/* Function Definitions */
/*
 * Arguments    : float x
 *                float y
 * Return Type  : boolean_T
 */
boolean_T iseq(float x, float y)
{
  float absx;
  int exponent;
  boolean_T p;
  absx = fabsf(y / 2.0F);
  if ((!rtIsInfF(absx)) && (!rtIsNaNF(absx))) {
    if (absx <= 1.17549435E-38F) {
      absx = 1.4013E-45F;
    } else {
      frexpf(absx, &exponent);
      absx = ldexpf(1.0F, exponent - 24);
    }
  } else {
    absx = rtNaNF;
  }
  if ((fabsf(y - x) < absx) ||
      (rtIsInfF(x) && rtIsInfF(y) && ((x > 0.0F) == (y > 0.0F)))) {
    p = true;
  } else {
    p = false;
  }
  return p;
}

/*
 * File trailer for relop.c
 *
 * [EOF]
 */

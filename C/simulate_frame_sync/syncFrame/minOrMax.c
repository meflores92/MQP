/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: minOrMax.c
 *
 * MATLAB Coder version            : 5.2
 * C/C++ source code generated on  : 19-Jan-2022 16:08:45
 */

/* Include Files */
#include "minOrMax.h"
#include "operation_rtwutil.h"
#include "operation_types.h"
#include <math.h>

/* Function Definitions */
/*
 * Arguments    : const emxArray_creal32_T *x
 *                creal32_T *ex
 *                int *idx
 * Return Type  : void
 */
void maximum(const emxArray_creal32_T *x, creal32_T *ex, int *idx)
{
  creal32_T fc;
  float absx;
  float b_x;
  float br;
  int b_exponent;
  int c_exponent;
  int exponent;
  int istop;
  int k;
  boolean_T SCALEA;
  boolean_T SCALEB;
  istop = x->size[0];
  *idx = 1;
  *ex = x->data[0];
  for (k = 2; k <= istop; k++) {
    if ((fabsf(ex->re) > 1.70141173E+38F) ||
        (fabsf(ex->im) > 1.70141173E+38F)) {
      SCALEA = true;
    } else {
      SCALEA = false;
    }
    fc = x->data[k - 1];
    if ((fabsf(fc.re) > 1.70141173E+38F) ||
        (fabsf(x->data[k - 1].im) > 1.70141173E+38F)) {
      SCALEB = true;
    } else {
      SCALEB = false;
    }
    if (SCALEA || SCALEB) {
      b_x = rt_hypotf(ex->re / 2.0F, ex->im / 2.0F);
      br = rt_hypotf(x->data[k - 1].re / 2.0F, x->data[k - 1].im / 2.0F);
    } else {
      b_x = rt_hypotf(ex->re, ex->im);
      br = rt_hypotf(x->data[k - 1].re, x->data[k - 1].im);
    }
    absx = br / 2.0F;
    if (absx <= 1.17549435E-38F) {
      absx = 1.4013E-45F;
    } else {
      frexpf(absx, &exponent);
      absx = ldexpf(1.0F, exponent - 24);
    }
    if (fabsf(br - b_x) < absx) {
      b_x = atan2f(ex->im, ex->re);
      br = atan2f(x->data[k - 1].im, x->data[k - 1].re);
      absx = fabsf(br / 2.0F);
      if (absx <= 1.17549435E-38F) {
        absx = 1.4013E-45F;
      } else {
        frexpf(absx, &b_exponent);
        absx = ldexpf(1.0F, b_exponent - 24);
      }
      if (fabsf(br - b_x) < absx) {
        br = x->data[k - 1].re;
        if (ex->re != br) {
          if (b_x >= 0.0F) {
            b_x = br;
            br = ex->re;
          } else {
            b_x = ex->re;
          }
        } else {
          b_x = ex->im;
          br = x->data[k - 1].im;
        }
        absx = fabsf(br / 2.0F);
        if (absx <= 1.17549435E-38F) {
          absx = 1.4013E-45F;
        } else {
          frexpf(absx, &c_exponent);
          absx = ldexpf(1.0F, c_exponent - 24);
        }
        if (fabsf(br - b_x) < absx) {
          b_x = 0.0F;
          br = 0.0F;
        }
      }
    }
    if (b_x < br) {
      *ex = fc;
      *idx = k;
    }
  }
}

/*
 * File trailer for minOrMax.c
 *
 * [EOF]
 */

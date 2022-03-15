/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: minOrMax.c
 *
 * MATLAB Coder version            : 5.2
 * C/C++ source code generated on  : 20-Feb-2022 08:55:56
 */

/* Include Files */
#include "minOrMax.h"
#include "operation_rtwutil.h"
#include "operation_types.h"
#include "relop.h"
#include "rt_nonfinite.h"
#include "rt_nonfinite.h"
#include <math.h>
#include <string.h>

/* Function Definitions */
/*
 * Arguments    : const emxArray_creal32_T *x
 *                creal32_T *ex
 *                int *idx
 * Return Type  : void
 */
void b_maximum(const emxArray_creal32_T *x, creal32_T *ex, int *idx)
{
  creal32_T fc;
  float b_x;
  float br;
  int istop;
  int k;
  boolean_T SCALEA;
  boolean_T SCALEB;
  istop = x->size[0];
  *idx = 1;
  *ex = x->data[0];
  for (k = 2; k <= istop; k++) {
    fc = x->data[k - 1];
    if (rtIsNaNF(fc.re) || rtIsNaNF(x->data[k - 1].im)) {
      SCALEA = false;
    } else if (rtIsNaNF(ex->re) || rtIsNaNF(ex->im)) {
      SCALEA = true;
    } else {
      if ((fabsf(ex->re) > 1.70141173E+38F) ||
          (fabsf(ex->im) > 1.70141173E+38F)) {
        SCALEA = true;
      } else {
        SCALEA = false;
      }
      if ((fabsf(x->data[k - 1].re) > 1.70141173E+38F) ||
          (fabsf(x->data[k - 1].im) > 1.70141173E+38F)) {
        SCALEB = true;
      } else {
        SCALEB = false;
      }
      if (SCALEA || SCALEB) {
        b_x = rt_hypotf_snf(ex->re / 2.0F, ex->im / 2.0F);
        br = rt_hypotf_snf(x->data[k - 1].re / 2.0F, x->data[k - 1].im / 2.0F);
      } else {
        b_x = rt_hypotf_snf(ex->re, ex->im);
        br = rt_hypotf_snf(x->data[k - 1].re, x->data[k - 1].im);
      }
      if (iseq(b_x, br)) {
        b_x = rt_atan2f_snf(ex->im, ex->re);
        br = rt_atan2f_snf(x->data[k - 1].im, x->data[k - 1].re);
        if (iseq(b_x, br)) {
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
          if (iseq(b_x, br)) {
            b_x = 0.0F;
            br = 0.0F;
          }
        }
      }
      SCALEA = (b_x < br);
    }
    if (SCALEA) {
      *ex = fc;
      *idx = k;
    }
  }
}

/*
 * Arguments    : const double x_data[]
 *                const int x_size[2]
 * Return Type  : double
 */
double b_minimum(const double x_data[], const int x_size[2])
{
  double d;
  double ex;
  int idx;
  int k;
  int last;
  boolean_T exitg1;
  last = x_size[1];
  if (x_size[1] <= 2) {
    if (x_size[1] == 1) {
      ex = x_data[0];
    } else {
      ex = x_data[x_size[1] - 1];
      if ((!(x_data[0] > ex)) && ((!rtIsNaN(x_data[0])) || rtIsNaN(ex))) {
        ex = x_data[0];
      }
    }
  } else {
    if (!rtIsNaN(x_data[0])) {
      idx = 1;
    } else {
      idx = 0;
      k = 2;
      exitg1 = false;
      while ((!exitg1) && (k <= last)) {
        if (!rtIsNaN(x_data[k - 1])) {
          idx = k;
          exitg1 = true;
        } else {
          k++;
        }
      }
    }
    if (idx == 0) {
      ex = x_data[0];
    } else {
      ex = x_data[idx - 1];
      idx++;
      for (k = idx; k <= last; k++) {
        d = x_data[k - 1];
        if (ex > d) {
          ex = d;
        }
      }
    }
  }
  return ex;
}

/*
 * Arguments    : const emxArray_real_T *x
 * Return Type  : double
 */
double maximum(const emxArray_real_T *x)
{
  double d;
  double ex;
  int idx;
  int k;
  int last;
  boolean_T exitg1;
  last = x->size[0];
  if (x->size[0] <= 2) {
    if (x->size[0] == 1) {
      ex = x->data[0];
    } else if ((x->data[0] < x->data[x->size[0] - 1]) ||
               (rtIsNaN(x->data[0]) && (!rtIsNaN(x->data[x->size[0] - 1])))) {
      ex = x->data[x->size[0] - 1];
    } else {
      ex = x->data[0];
    }
  } else {
    if (!rtIsNaN(x->data[0])) {
      idx = 1;
    } else {
      idx = 0;
      k = 2;
      exitg1 = false;
      while ((!exitg1) && (k <= last)) {
        if (!rtIsNaN(x->data[k - 1])) {
          idx = k;
          exitg1 = true;
        } else {
          k++;
        }
      }
    }
    if (idx == 0) {
      ex = x->data[0];
    } else {
      ex = x->data[idx - 1];
      idx++;
      for (k = idx; k <= last; k++) {
        d = x->data[k - 1];
        if (ex < d) {
          ex = d;
        }
      }
    }
  }
  return ex;
}

/*
 * Arguments    : const emxArray_real_T *x
 * Return Type  : double
 */
double minimum(const emxArray_real_T *x)
{
  double d;
  double ex;
  int idx;
  int k;
  int last;
  boolean_T exitg1;
  last = x->size[0];
  if (x->size[0] <= 2) {
    if (x->size[0] == 1) {
      ex = x->data[0];
    } else if ((x->data[0] > x->data[x->size[0] - 1]) ||
               (rtIsNaN(x->data[0]) && (!rtIsNaN(x->data[x->size[0] - 1])))) {
      ex = x->data[x->size[0] - 1];
    } else {
      ex = x->data[0];
    }
  } else {
    if (!rtIsNaN(x->data[0])) {
      idx = 1;
    } else {
      idx = 0;
      k = 2;
      exitg1 = false;
      while ((!exitg1) && (k <= last)) {
        if (!rtIsNaN(x->data[k - 1])) {
          idx = k;
          exitg1 = true;
        } else {
          k++;
        }
      }
    }
    if (idx == 0) {
      ex = x->data[0];
    } else {
      ex = x->data[idx - 1];
      idx++;
      for (k = idx; k <= last; k++) {
        d = x->data[k - 1];
        if (ex > d) {
          ex = d;
        }
      }
    }
  }
  return ex;
}

/*
 * File trailer for minOrMax.c
 *
 * [EOF]
 */

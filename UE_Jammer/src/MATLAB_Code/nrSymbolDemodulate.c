/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: nrSymbolDemodulate.c
 *
 * MATLAB Coder version            : 5.2
 * C/C++ source code generated on  : 20-Feb-2022 08:55:56
 */

/* Include Files */
#include "nrSymbolDemodulate.h"
#include "operation_emxutil.h"
#include "operation_types.h"
#include "rt_nonfinite.h"
#include "rt_nonfinite.h"
#include <string.h>

/* Function Definitions */
/*
 * Arguments    : const emxArray_creal32_T *in
 *                emxArray_real32_T *out
 * Return Type  : void
 */
void nrSymbolDemodulate(const emxArray_creal32_T *in, emxArray_real32_T *out)
{
  static const creal32_T constellation[4] = {{
                                                 -0.707106769F, /* re */
                                                 0.707106769F   /* im */
                                             },
                                             {
                                                 -0.707106769F, /* re */
                                                 -0.707106769F  /* im */
                                             },
                                             {
                                                 0.707106769F, /* re */
                                                 0.707106769F  /* im */
                                             },
                                             {
                                                 0.707106769F, /* re */
                                                 -0.707106769F /* im */
                                             }};
  float b_im;
  float b_re;
  float b_y_idx_0;
  float c_y_idx_0;
  float im;
  float re;
  float y_idx_0;
  int i;
  int inpIdx;
  int loop_ub;
  if ((in->size[0] == 0) || (in->size[1] == 0) || (in->size[2] == 0)) {
    i = out->size[0] * out->size[1] * out->size[2];
    out->size[0] = (short)in->size[0];
    out->size[1] = (short)in->size[1];
    out->size[2] = (short)in->size[2];
    emxEnsureCapacity_real32_T(out, i);
    loop_ub = (short)in->size[0] * (short)in->size[1] * (short)in->size[2];
    for (i = 0; i < loop_ub; i++) {
      out->data[i] = 0.0F;
    }
  } else {
    i = out->size[0] * out->size[1] * out->size[2];
    out->size[0] = (unsigned char)in->size[0] << 1;
    out->size[1] = (short)in->size[1];
    out->size[2] = (short)in->size[2];
    emxEnsureCapacity_real32_T(out, i);
    i = in->size[0] * in->size[1] * in->size[2];
    for (inpIdx = 0; inpIdx < i; inpIdx++) {
      re = constellation[2].re - in->data[inpIdx].re;
      im = constellation[2].im - in->data[inpIdx].im;
      y_idx_0 = re * re;
      b_y_idx_0 = im * im;
      re = constellation[3].re - in->data[inpIdx].re;
      im = constellation[3].im - in->data[inpIdx].im;
      b_re = constellation[0].re - in->data[inpIdx].re;
      b_im = constellation[0].im - in->data[inpIdx].im;
      c_y_idx_0 = b_re * b_re + b_im * b_im;
      b_re = constellation[1].re - in->data[inpIdx].re;
      b_im = constellation[1].im - in->data[inpIdx].im;
      b_re = b_re * b_re + b_im * b_im;
      if ((c_y_idx_0 > b_re) || (rtIsNaNF(c_y_idx_0) && (!rtIsNaNF(b_re)))) {
        c_y_idx_0 = b_re;
      }
      y_idx_0 += b_y_idx_0;
      b_re = re * re + im * im;
      if ((y_idx_0 > b_re) || (rtIsNaNF(y_idx_0) && (!rtIsNaNF(b_re)))) {
        y_idx_0 = b_re;
      }
      loop_ub = inpIdx << 1;
      out->data[loop_ub] = c_y_idx_0 - y_idx_0;
      re = constellation[0].re - in->data[inpIdx].re;
      im = constellation[0].im - in->data[inpIdx].im;
      y_idx_0 = re * re;
      b_y_idx_0 = im * im;
      re = constellation[2].re - in->data[inpIdx].re;
      im = constellation[2].im - in->data[inpIdx].im;
      b_re = constellation[1].re - in->data[inpIdx].re;
      b_im = constellation[1].im - in->data[inpIdx].im;
      c_y_idx_0 = b_re * b_re + b_im * b_im;
      b_re = constellation[3].re - in->data[inpIdx].re;
      b_im = constellation[3].im - in->data[inpIdx].im;
      b_re = b_re * b_re + b_im * b_im;
      if ((c_y_idx_0 > b_re) || (rtIsNaNF(c_y_idx_0) && (!rtIsNaNF(b_re)))) {
        c_y_idx_0 = b_re;
      }
      y_idx_0 += b_y_idx_0;
      b_re = re * re + im * im;
      if ((y_idx_0 > b_re) || (rtIsNaNF(y_idx_0) && (!rtIsNaNF(b_re)))) {
        y_idx_0 = b_re;
      }
      out->data[loop_ub + 1] = c_y_idx_0 - y_idx_0;
    }
    loop_ub = out->size[0] * out->size[1] * out->size[2];
    for (i = 0; i < loop_ub; i++) {
      out->data[i] /= 1.0E-10F;
    }
  }
}

/*
 * File trailer for nrSymbolDemodulate.c
 *
 * [EOF]
 */

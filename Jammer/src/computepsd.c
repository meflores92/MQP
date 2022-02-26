/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: computepsd.c
 *
 * MATLAB Coder version            : 5.2
 * C/C++ source code generated on  : 20-Feb-2022 08:55:56
 */

/* Include Files */
#include "computepsd.h"
#include "operation_emxutil.h"
#include "operation_types.h"
#include "rt_nonfinite.h"
#include "strcmp.h"
#include <string.h>

/* Function Definitions */
/*
 * Arguments    : const emxArray_real32_T *Sxx1
 *                const double w2_data[]
 *                int w2_size
 *                const char range[8]
 *                emxArray_real32_T *varargout_1
 *                double varargout_2_data[]
 *                int *varargout_2_size
 * Return Type  : void
 */
void computepsd(const emxArray_real32_T *Sxx1, const double w2_data[],
                int w2_size, const char range[8],
                emxArray_real32_T *varargout_1, double varargout_2_data[],
                int *varargout_2_size)
{
  emxArray_real32_T *Sxx_unscaled;
  int i;
  int loop_ub;
  if (b_strcmp(range)) {
    emxInit_real32_T(&Sxx_unscaled, 2);
    i = Sxx_unscaled->size[0] * Sxx_unscaled->size[1];
    Sxx_unscaled->size[0] = 769;
    Sxx_unscaled->size[1] = 600;
    emxEnsureCapacity_real32_T(Sxx_unscaled, i);
    i = varargout_1->size[0] * varargout_1->size[1];
    varargout_1->size[0] = 769;
    varargout_1->size[1] = 600;
    emxEnsureCapacity_real32_T(varargout_1, i);
    for (i = 0; i < 600; i++) {
      for (loop_ub = 0; loop_ub < 769; loop_ub++) {
        Sxx_unscaled->data[loop_ub + Sxx_unscaled->size[0] * i] =
            Sxx1->data[loop_ub + Sxx1->size[0] * i];
      }
      varargout_1->data[varargout_1->size[0] * i] =
          Sxx_unscaled->data[Sxx_unscaled->size[0] * i];
      for (loop_ub = 0; loop_ub < 767; loop_ub++) {
        varargout_1->data[(loop_ub + varargout_1->size[0] * i) + 1] =
            2.0F *
            Sxx_unscaled->data[(loop_ub + Sxx_unscaled->size[0] * i) + 1];
      }
      varargout_1->data[varargout_1->size[0] * i + 768] =
          Sxx_unscaled->data[Sxx_unscaled->size[0] * i + 768];
    }
    emxFree_real32_T(&Sxx_unscaled);
    *varargout_2_size = 769;
    memcpy(&varargout_2_data[0], &w2_data[0], 769U * sizeof(double));
  } else {
    i = varargout_1->size[0] * varargout_1->size[1];
    varargout_1->size[0] = Sxx1->size[0];
    varargout_1->size[1] = 600;
    emxEnsureCapacity_real32_T(varargout_1, i);
    loop_ub = Sxx1->size[0] * 600;
    for (i = 0; i < loop_ub; i++) {
      varargout_1->data[i] = Sxx1->data[i];
    }
    *varargout_2_size = w2_size;
    if (0 <= w2_size - 1) {
      memcpy(&varargout_2_data[0], &w2_data[0], w2_size * sizeof(double));
    }
  }
  loop_ub = varargout_1->size[0] * 600;
  i = varargout_1->size[0] * varargout_1->size[1];
  varargout_1->size[1] = 600;
  emxEnsureCapacity_real32_T(varargout_1, i);
  for (i = 0; i < loop_ub; i++) {
    varargout_1->data[i] /= 4.608E+7F;
  }
}

/*
 * File trailer for computepsd.c
 *
 * [EOF]
 */

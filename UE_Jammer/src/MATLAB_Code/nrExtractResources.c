/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: nrExtractResources.c
 *
 * MATLAB Coder version            : 5.2
 * C/C++ source code generated on  : 20-Feb-2022 08:55:56
 */

/* Include Files */
#include "nrExtractResources.h"
#include "operation_emxutil.h"
#include "operation_rtwutil.h"
#include "operation_types.h"
#include "rt_nonfinite.h"
#include "sortrows.h"
#include "unique.h"
#include <string.h>

/* Function Definitions */
/*
 * Arguments    : const double ind[216]
 *                const creal32_T varargin_1[624]
 *                emxArray_creal32_T *varargout_1
 * Return Type  : void
 */
void nrExtractResources(const double ind[216], const creal32_T varargin_1[624],
                        emxArray_creal32_T *varargout_1)
{
  double a__1_data[3024];
  double a_data[864];
  double subIndOut_data[864];
  double subIndIn_data[432];
  double v1_data[432];
  double sortIdx_data[216];
  int b_v1_data[648];
  int a__1_size[2];
  int subIndIn_size[2];
  int a_size_idx_1;
  int cpsiz_idx_1;
  int i;
  int i1;
  int jcol;
  int loop_ub;
  int result;
  int subIndOut_size_idx_0;
  signed char input_sizes_idx_1;
  boolean_T empty_non_axis_sizes;
  for (i = 0; i < 216; i++) {
    i1 = (int)ind[i] - 1;
    cpsiz_idx_1 = div_s32(i1, 624);
    jcol = cpsiz_idx_1 + 1;
    i1 -= cpsiz_idx_1 * 624;
    cpsiz_idx_1 = div_s32(i1, 624);
    i1 -= cpsiz_idx_1 * 624;
    b_v1_data[i] = i1 + 1;
    b_v1_data[i + 216] = cpsiz_idx_1 + 1;
    b_v1_data[i + 432] = jcol;
  }
  for (i = 0; i < 2; i++) {
    for (i1 = 0; i1 < 216; i1++) {
      cpsiz_idx_1 = i1 + 216 * i;
      v1_data[cpsiz_idx_1] = b_v1_data[cpsiz_idx_1];
    }
  }
  unique_rows(v1_data, subIndIn_data, subIndIn_size);
  if (subIndIn_size[0] != 0) {
    result = subIndIn_size[0];
  } else {
    result = 0;
  }
  empty_non_axis_sizes = (result == 0);
  if (empty_non_axis_sizes || (subIndIn_size[0] != 0)) {
    input_sizes_idx_1 = 2;
  } else {
    input_sizes_idx_1 = 0;
  }
  if (empty_non_axis_sizes || (subIndIn_size[0] != 0)) {
    cpsiz_idx_1 = 2;
  } else {
    cpsiz_idx_1 = 0;
  }
  a_size_idx_1 = input_sizes_idx_1 + cpsiz_idx_1;
  loop_ub = input_sizes_idx_1;
  for (i = 0; i < loop_ub; i++) {
    for (i1 = 0; i1 < result; i1++) {
      a_data[i1 + result * i] = subIndIn_data[i1 + result * i];
    }
  }
  for (i = 0; i < cpsiz_idx_1; i++) {
    for (i1 = 0; i1 < result; i1++) {
      a_data[i1 + result * (i + input_sizes_idx_1)] = 0.0;
    }
  }
  subIndOut_size_idx_0 = (unsigned char)result;
  for (jcol = 0; jcol < a_size_idx_1; jcol++) {
    cpsiz_idx_1 = jcol * result;
    if (0 <= result - 1) {
      memcpy(&subIndOut_data[cpsiz_idx_1], &a_data[cpsiz_idx_1],
             ((result + cpsiz_idx_1) - cpsiz_idx_1) * sizeof(double));
    }
  }
  cpsiz_idx_1 = (unsigned char)subIndIn_size[0];
  if (3 > a_size_idx_1) {
    i = 1;
  } else {
    i = 3;
  }
  for (i1 = 0; i1 < cpsiz_idx_1; i1++) {
    subIndOut_data[i1 + (unsigned char)result * (i - 1)] = 1.0;
  }
  for (i1 = 0; i1 < cpsiz_idx_1; i1++) {
    subIndOut_data[i1 + (unsigned char)result * i] = 1.0;
  }
  cpsiz_idx_1 = (unsigned char)result - 1;
  for (i = 0; i < 2; i++) {
    for (i1 = 0; i1 <= cpsiz_idx_1; i1++) {
      subIndIn_data[i1 + (unsigned char)result * i] =
          subIndOut_data[i1 + (unsigned char)result * (3 - i)];
    }
  }
  loop_ub = (unsigned char)result;
  for (i = 0; i < 2; i++) {
    for (i1 = 0; i1 < loop_ub; i1++) {
      subIndOut_data[i1 + (unsigned char)result * (i + 2)] =
          subIndIn_data[i1 + (unsigned char)result * i];
    }
  }
  if (3 > a_size_idx_1) {
    i = 0;
    i1 = 1;
    cpsiz_idx_1 = -1;
  } else {
    i = a_size_idx_1 - 1;
    i1 = -1;
    cpsiz_idx_1 = 2;
  }
  a__1_size[0] = (unsigned char)result;
  loop_ub = div_s32_floor(cpsiz_idx_1 - i, i1);
  a__1_size[1] = loop_ub + 1;
  for (cpsiz_idx_1 = 0; cpsiz_idx_1 <= loop_ub; cpsiz_idx_1++) {
    for (jcol = 0; jcol < subIndOut_size_idx_0; jcol++) {
      a__1_data[jcol + (unsigned char)result * cpsiz_idx_1] =
          subIndOut_data[jcol + (unsigned char)result * (i + i1 * cpsiz_idx_1)];
    }
  }
  sortrows(a__1_data, a__1_size, sortIdx_data, &jcol);
  cpsiz_idx_1 = a_size_idx_1 - 1;
  for (i = 0; i <= cpsiz_idx_1; i++) {
    for (i1 = 0; i1 < jcol; i1++) {
      a_data[i1 + jcol * i] =
          subIndOut_data[((int)sortIdx_data[i1] + (unsigned char)result * i) -
                         1];
    }
  }
  loop_ub = jcol * a_size_idx_1;
  if (0 <= loop_ub - 1) {
    memcpy(&subIndOut_data[0], &a_data[0], loop_ub * sizeof(double));
  }
  for (i = 0; i < jcol; i++) {
    sortIdx_data[i] =
        (int)subIndOut_data[i] + 624 * ((int)subIndOut_data[i + jcol] - 1);
  }
  i = varargout_1->size[0] * varargout_1->size[1] * varargout_1->size[2];
  varargout_1->size[0] = (unsigned char)subIndIn_size[0];
  varargout_1->size[1] = 1;
  varargout_1->size[2] = 1;
  emxEnsureCapacity_creal32_T(varargout_1, i);
  loop_ub = (unsigned char)subIndIn_size[0];
  for (i = 0; i < loop_ub; i++) {
    varargout_1->data[i] = varargin_1[(int)sortIdx_data[i] - 1];
  }
}

/*
 * File trailer for nrExtractResources.c
 *
 * [EOF]
 */

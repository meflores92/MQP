/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: nrPRBS.c
 *
 * MATLAB Coder version            : 5.2
 * C/C++ source code generated on  : 20-Feb-2022 08:55:56
 */

/* Include Files */
#include "nrPRBS.h"
#include "bsxfun.h"
#include "operation_emxutil.h"
#include "operation_rtwutil.h"
#include "operation_types.h"
#include "rt_nonfinite.h"
#include <string.h>

/* Variable Definitions */
static const unsigned int uv[16] = {
    0U, 1073741824U, 1073741824U, 0U, 1073741824U, 0U,
    0U, 1073741824U, 1073741824U, 0U, 0U,          1073741824U,
    0U, 1073741824U, 1073741824U, 0U};

static const signed char iv2[256] = {
    0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0,
    0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1,
    0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0,
    0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0,
    0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0,
    0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1,
    0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1,
    1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1,
    0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0,
    0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1,
    0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0};

/* Function Definitions */
/*
 * Arguments    : const emxArray_real_T *cinit
 *                double n
 *                float seq_data[]
 *                int *seq_size
 * Return Type  : void
 */
void b_nrPRBS(const emxArray_real_T *cinit, double n, float seq_data[],
              int *seq_size)
{
  emxArray_uint32_T *reg;
  unsigned int seqpair_data[864];
  unsigned int tmp_data[864];
  unsigned int ts_data[864];
  int b_ts_size[2];
  int seqpair_size[2];
  int tmp_size[2];
  int ts_size[2];
  int b_i;
  unsigned int b_varargin_1;
  unsigned int c_varargin_1;
  unsigned int d_varargin_1;
  int i;
  int i1;
  int loop_ub;
  unsigned int varargin_1;
  emxInit_uint32_T(&reg, 2);
  i = reg->size[0] * reg->size[1];
  reg->size[0] = 1;
  reg->size[1] = cinit->size[1] + 1;
  emxEnsureCapacity_uint32_T(reg, i);
  reg->data[0] = 1U;
  loop_ub = cinit->size[1];
  for (i = 0; i < loop_ub; i++) {
    reg->data[i + 1] = (unsigned int)rt_roundd_snf(cinit->data[i]);
  }
  i = (int)n;
  seqpair_size[0] = (int)n;
  seqpair_size[1] = 2;
  for (b_i = 0; b_i < i; b_i++) {
    varargin_1 = reg->data[0];
    seqpair_data[b_i] = varargin_1 & 35263098U;
    varargin_1 = reg->data[0];
    b_varargin_1 = reg->data[1];
    seqpair_data[b_i + (int)n] = b_varargin_1 & 10031374U;
    b_varargin_1 = reg->data[1];
    i1 = reg->size[0] * reg->size[1];
    reg->size[0] = 1;
    emxEnsureCapacity_uint32_T(reg, i1);
    loop_ub = reg->size[1] - 1;
    for (i1 = 0; i1 <= loop_ub; i1++) {
      c_varargin_1 = reg->data[i1];
      reg->data[i1] = c_varargin_1 >> 1;
    }
    c_varargin_1 = reg->data[0];
    d_varargin_1 = reg->data[1];
    i1 = reg->size[0] * reg->size[1];
    reg->size[0] = 1;
    reg->size[1] = 2;
    emxEnsureCapacity_uint32_T(reg, i1);
    reg->data[0] = c_varargin_1 ^ uv[(int)(varargin_1 & 9U)];
    reg->data[1] = d_varargin_1 ^ uv[(int)(b_varargin_1 & 15U)];
  }
  emxFree_uint32_T(&reg);
  tmp_size[0] = (int)n;
  tmp_size[1] = 2;
  loop_ub = (int)n * 2;
  for (i = 0; i < loop_ub; i++) {
    tmp_data[i] = seqpair_data[i] >> 16;
  }
  bsxfun(seqpair_data, seqpair_size, tmp_data, tmp_size, ts_data, ts_size);
  tmp_size[0] = ts_size[0];
  tmp_size[1] = 2;
  loop_ub = ts_size[0] * 2;
  for (i = 0; i < loop_ub; i++) {
    tmp_data[i] = ts_data[i] >> 8;
  }
  b_ts_size[0] = ts_size[0];
  b_ts_size[1] = 2;
  loop_ub = ts_size[0] * ts_size[1] - 1;
  if (0 <= loop_ub) {
    memcpy(&seqpair_data[0], &ts_data[0], (loop_ub + 1) * sizeof(unsigned int));
  }
  bsxfun(seqpair_data, b_ts_size, tmp_data, tmp_size, ts_data, ts_size);
  loop_ub = ts_size[0] * 2;
  for (i = 0; i < loop_ub; i++) {
    ts_data[i] &= 255U;
  }
  seqpair_size[0] = ts_size[0];
  loop_ub = ts_size[0] * 2;
  for (i = 0; i < loop_ub; i++) {
    seqpair_data[i] = (unsigned int)iv2[(int)ts_data[i]];
  }
  loop_ub = ts_size[0];
  *seq_size = ts_size[0];
  for (i = 0; i < loop_ub; i++) {
    seq_data[i] =
        1.0F - 2.0F * (float)((seqpair_data[i] != 0U) !=
                              (seqpair_data[i + seqpair_size[0]] != 0U));
  }
}

/*
 * Arguments    : double cinit
 *                double n
 *                float seq_data[]
 *                int *seq_size
 * Return Type  : void
 */
void nrPRBS(double cinit, double n, float seq_data[], int *seq_size)
{
  unsigned int seqpair_data[864];
  unsigned int tmp_data[864];
  unsigned int ts_data[864];
  int b_ts_size[2];
  int seqpair_size[2];
  int tmp_size[2];
  int ts_size[2];
  int b_i;
  int c_idx_0;
  int c_idx_1;
  int i;
  unsigned int reg_idx_0;
  unsigned int reg_idx_1;
  reg_idx_0 = 1U;
  reg_idx_1 = (unsigned int)rt_roundd_snf(cinit);
  i = (int)n;
  seqpair_size[0] = (int)n;
  seqpair_size[1] = 2;
  for (b_i = 0; b_i < i; b_i++) {
    seqpair_data[b_i] = reg_idx_0 & 35263098U;
    c_idx_0 = (int)(reg_idx_0 & 9U);
    reg_idx_0 >>= 1;
    seqpair_data[b_i + (int)n] = reg_idx_1 & 10031374U;
    c_idx_1 = (int)(reg_idx_1 & 15U);
    reg_idx_1 >>= 1;
    reg_idx_0 ^= uv[c_idx_0];
    reg_idx_1 ^= uv[c_idx_1];
  }
  tmp_size[0] = (int)n;
  tmp_size[1] = 2;
  b_i = (int)n * 2;
  for (i = 0; i < b_i; i++) {
    tmp_data[i] = seqpair_data[i] >> 16;
  }
  bsxfun(seqpair_data, seqpair_size, tmp_data, tmp_size, ts_data, ts_size);
  tmp_size[0] = ts_size[0];
  tmp_size[1] = 2;
  b_i = ts_size[0] * 2;
  for (i = 0; i < b_i; i++) {
    tmp_data[i] = ts_data[i] >> 8;
  }
  b_ts_size[0] = ts_size[0];
  b_ts_size[1] = 2;
  b_i = ts_size[0] * ts_size[1] - 1;
  if (0 <= b_i) {
    memcpy(&seqpair_data[0], &ts_data[0], (b_i + 1) * sizeof(unsigned int));
  }
  bsxfun(seqpair_data, b_ts_size, tmp_data, tmp_size, ts_data, ts_size);
  b_i = ts_size[0] * 2;
  for (i = 0; i < b_i; i++) {
    ts_data[i] &= 255U;
  }
  seqpair_size[0] = ts_size[0];
  b_i = ts_size[0] * 2;
  for (i = 0; i < b_i; i++) {
    seqpair_data[i] = (unsigned int)iv2[(int)ts_data[i]];
  }
  b_i = ts_size[0];
  *seq_size = ts_size[0];
  for (i = 0; i < b_i; i++) {
    seq_data[i] =
        1.0F - 2.0F * (float)((seqpair_data[i] != 0U) !=
                              (seqpair_data[i + seqpair_size[0]] != 0U));
  }
}

/*
 * File trailer for nrPRBS.c
 *
 * [EOF]
 */

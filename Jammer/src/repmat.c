/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: repmat.c
 *
 * MATLAB Coder version            : 5.2
 * C/C++ source code generated on  : 20-Feb-2022 08:55:56
 */

/* Include Files */
#include "repmat.h"
#include "operation_emxutil.h"
#include "operation_types.h"
#include "rt_nonfinite.h"
#include <string.h>

/* Function Definitions */
/*
 * Arguments    : const emxArray_creal32_T *a
 *                const double varargin_1[2]
 *                emxArray_creal32_T *b
 * Return Type  : void
 */
void b_repmat(const emxArray_creal32_T *a, const double varargin_1[2],
              emxArray_creal32_T *b)
{
  int iacol_tmp;
  int ibmat;
  int ibtile;
  int jcol;
  int jtilecol;
  int k;
  int ncols;
  int nrows;
  int ntilecols;
  nrows = b->size[0] * b->size[1];
  b->size[0] = a->size[0];
  b->size[1] = a->size[1] * (int)varargin_1[1];
  emxEnsureCapacity_creal32_T(b, nrows);
  nrows = a->size[0];
  ncols = a->size[1];
  ntilecols = (int)varargin_1[1];
  for (jtilecol = 0; jtilecol < ntilecols; jtilecol++) {
    ibtile = jtilecol * (nrows * ncols) - 1;
    for (jcol = 0; jcol < ncols; jcol++) {
      iacol_tmp = jcol * nrows;
      ibmat = ibtile + iacol_tmp;
      for (k = 0; k < nrows; k++) {
        b->data[(ibmat + k) + 1] = a->data[iacol_tmp + k];
      }
    }
  }
}

/*
 * Arguments    : double a
 *                const double varargin_1[2]
 *                emxArray_real_T *b
 * Return Type  : void
 */
void repmat(double a, const double varargin_1[2], emxArray_real_T *b)
{
  int i;
  int loop_ub;
  i = b->size[0] * b->size[1];
  b->size[0] = (short)(int)varargin_1[0];
  b->size[1] = (short)(int)varargin_1[1];
  emxEnsureCapacity_real_T(b, i);
  loop_ub = (short)(int)varargin_1[0] * (short)(int)varargin_1[1];
  for (i = 0; i < loop_ub; i++) {
    b->data[i] = a;
  }
}

/*
 * File trailer for repmat.c
 *
 * [EOF]
 */

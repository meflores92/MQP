/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: bsxfun.c
 *
 * MATLAB Coder version            : 5.2
 * C/C++ source code generated on  : 20-Feb-2022 08:55:56
 */

/* Include Files */
#include "bsxfun.h"
#include "operation_emxutil.h"
#include "operation_types.h"
#include "rt_nonfinite.h"
#include <string.h>

/* Function Definitions */
/*
 * Arguments    : const emxArray_real_T *a
 *                emxArray_real_T *c
 * Return Type  : void
 */
void b_bsxfun(const emxArray_real_T *a, emxArray_real_T *c)
{
  int acoef;
  int b_k;
  int i;
  int k;
  i = c->size[0] * c->size[1];
  c->size[0] = (short)a->size[0];
  c->size[1] = 12;
  emxEnsureCapacity_real_T(c, i);
  acoef = (a->size[0] != 1);
  for (k = 0; k < 12; k++) {
    i = c->size[0] - 1;
    for (b_k = 0; b_k <= i; b_k++) {
      c->data[b_k + c->size[0] * k] = a->data[acoef * b_k] + ((double)k + 1.0);
    }
  }
}

/*
 * Arguments    : const unsigned int a_data[]
 *                const int a_size[2]
 *                const unsigned int b_data[]
 *                const int b_size[2]
 *                unsigned int c_data[]
 *                int c_size[2]
 * Return Type  : void
 */
void bsxfun(const unsigned int a_data[], const int a_size[2],
            const unsigned int b_data[], const int b_size[2],
            unsigned int c_data[], int c_size[2])
{
  int acoef;
  int b_k;
  int bcoef;
  int i;
  int k;
  if (b_size[0] == 1) {
    c_size[0] = (short)a_size[0];
  } else if (a_size[0] == 1) {
    c_size[0] = (short)b_size[0];
  } else if (a_size[0] == b_size[0]) {
    c_size[0] = (short)a_size[0];
  } else if (b_size[0] < a_size[0]) {
    c_size[0] = (short)b_size[0];
  } else {
    c_size[0] = (short)a_size[0];
  }
  c_size[1] = 2;
  if (b_size[0] == 1) {
    acoef = (short)a_size[0];
  } else if (a_size[0] == 1) {
    acoef = (short)b_size[0];
  } else if (a_size[0] == b_size[0]) {
    acoef = (short)a_size[0];
  } else if (b_size[0] < a_size[0]) {
    acoef = (short)b_size[0];
  } else {
    acoef = (short)a_size[0];
  }
  if (acoef != 0) {
    acoef = (a_size[0] != 1);
    bcoef = (b_size[0] != 1);
    i = c_size[0] - 1;
    for (k = 0; k < 2; k++) {
      for (b_k = 0; b_k <= i; b_k++) {
        c_data[b_k + c_size[0] * k] = a_data[acoef * b_k + a_size[0] * k] ^
                                      b_data[bcoef * b_k + b_size[0] * k];
      }
    }
  }
}

/*
 * File trailer for bsxfun.c
 *
 * [EOF]
 */

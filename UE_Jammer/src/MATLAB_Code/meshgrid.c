/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: meshgrid.c
 *
 * MATLAB Coder version            : 5.2
 * C/C++ source code generated on  : 20-Feb-2022 08:55:56
 */

/* Include Files */
#include "meshgrid.h"
#include "operation_emxutil.h"
#include "operation_types.h"
#include "rt_nonfinite.h"
#include <string.h>

/* Function Definitions */
/*
 * Arguments    : const double x_data[]
 *                const int x_size[2]
 *                const emxArray_real_T *y
 *                emxArray_real_T *xx
 *                emxArray_real_T *yy
 * Return Type  : void
 */
void meshgrid(const double x_data[], const int x_size[2],
              const emxArray_real_T *y, emxArray_real_T *xx,
              emxArray_real_T *yy)
{
  int i;
  int j;
  int nx;
  int ny;
  nx = x_size[1];
  ny = y->size[0];
  j = xx->size[0] * xx->size[1];
  xx->size[0] = y->size[0];
  xx->size[1] = x_size[1];
  emxEnsureCapacity_real_T(xx, j);
  j = yy->size[0] * yy->size[1];
  yy->size[0] = y->size[0];
  yy->size[1] = x_size[1];
  emxEnsureCapacity_real_T(yy, j);
  if ((x_size[1] != 0) && (y->size[0] != 0)) {
    for (j = 0; j < nx; j++) {
      for (i = 0; i < ny; i++) {
        xx->data[i + xx->size[0] * j] = x_data[j];
        yy->data[i + yy->size[0] * j] = y->data[i];
      }
    }
  }
}

/*
 * File trailer for meshgrid.c
 *
 * [EOF]
 */

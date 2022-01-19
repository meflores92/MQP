/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: ifft.c
 *
 * MATLAB Coder version            : 5.2
 * C/C++ source code generated on  : 19-Jan-2022 16:08:45
 */

/* Include Files */
#include "ifft.h"
#include "FFTImplementationCallback.h"
#include "operation_emxutil.h"
#include "operation_types.h"

/* Function Definitions */
/*
 * Arguments    : const emxArray_creal32_T *x
 *                emxArray_creal32_T *y
 * Return Type  : void
 */
void ifft(const emxArray_creal32_T *x, emxArray_creal32_T *y)
{
  emxArray_real32_T *costab;
  emxArray_real32_T *sintab;
  int sintabinv_size[2];
  int N2blue;
  int nRows;
  emxInit_real32_T(&costab, 2);
  emxInit_real32_T(&sintab, 2);
  c_FFTImplementationCallback_get(x->size[0], true, &N2blue, &nRows);
  e_FFTImplementationCallback_gen(nRows, costab, sintab, sintabinv_size);
  e_FFTImplementationCallback_r2b(x, x->size[0], costab, sintab, y);
  emxFree_real32_T(&sintab);
  emxFree_real32_T(&costab);
}

/*
 * File trailer for ifft.c
 *
 * [EOF]
 */

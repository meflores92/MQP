/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: fft.c
 *
 * MATLAB Coder version            : 5.2
 * C/C++ source code generated on  : 19-Jan-2022 16:08:45
 */

/* Include Files */
#include "fft.h"
#include "FFTImplementationCallback.h"
#include "operation_emxutil.h"
#include "operation_types.h"

/* Function Definitions */
/*
 * Arguments    : const creal_T x_data[]
 *                int x_size
 *                double varargin_1
 *                emxArray_creal_T *y
 * Return Type  : void
 */
void b_fft(const creal_T x_data[], int x_size, double varargin_1,
           emxArray_creal_T *y)
{
  emxArray_real_T *costab;
  emxArray_real_T *sintab;
  int sintabinv_size[2];
  int N2blue;
  int nRows;
  emxInit_real_T(&costab, 2);
  emxInit_real_T(&sintab, 2);
  c_FFTImplementationCallback_get((int)varargin_1, true, &N2blue, &nRows);
  d_FFTImplementationCallback_gen(nRows, costab, sintab, sintabinv_size);
  c_FFTImplementationCallback_r2b(x_data, x_size, (int)varargin_1, costab,
                                  sintab, y);
  emxFree_real_T(&sintab);
  emxFree_real_T(&costab);
}

/*
 * Arguments    : const emxArray_creal32_T *x
 *                double varargin_1
 *                emxArray_creal32_T *y
 * Return Type  : void
 */
void fft(const emxArray_creal32_T *x, double varargin_1, emxArray_creal32_T *y)
{
  emxArray_real32_T *costab;
  emxArray_real32_T *sintab;
  int sintabinv_size[2];
  int N2blue;
  int nRows;
  emxInit_real32_T(&costab, 2);
  emxInit_real32_T(&sintab, 2);
  c_FFTImplementationCallback_get((int)varargin_1, true, &N2blue, &nRows);
  c_FFTImplementationCallback_gen(nRows, costab, sintab, sintabinv_size);
  d_FFTImplementationCallback_r2b(x, (int)varargin_1, costab, sintab, y);
  emxFree_real32_T(&sintab);
  emxFree_real32_T(&costab);
}

/*
 * File trailer for fft.c
 *
 * [EOF]
 */

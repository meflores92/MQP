/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: fft.h
 *
 * MATLAB Coder version            : 5.2
 * C/C++ source code generated on  : 19-Jan-2022 16:08:45
 */

#ifndef FFT_H
#define FFT_H

/* Include Files */
#include "operation_types.h"
#include "rtwtypes.h"
#include "omp.h"
#include <stddef.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Function Declarations */
void b_fft(const creal_T x_data[], int x_size, double varargin_1,
           emxArray_creal_T *y);

void fft(const emxArray_creal32_T *x, double varargin_1, emxArray_creal32_T *y);

#ifdef __cplusplus
}
#endif

#endif
/*
 * File trailer for fft.h
 *
 * [EOF]
 */

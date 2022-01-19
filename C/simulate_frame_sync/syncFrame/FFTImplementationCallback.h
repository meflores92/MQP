/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: FFTImplementationCallback.h
 *
 * MATLAB Coder version            : 5.2
 * C/C++ source code generated on  : 19-Jan-2022 16:08:45
 */

#ifndef FFTIMPLEMENTATIONCALLBACK_H
#define FFTIMPLEMENTATIONCALLBACK_H

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
void c_FFTImplementationCallback_dob(const creal_T x_data[], int n2blue,
                                     const double costab_data[],
                                     const int costab_size[2],
                                     const double sintab_data[],
                                     const int sintab_size[2],
                                     const double sintabinv_data[],
                                     creal_T y_data[], int y_size[3]);

void c_FFTImplementationCallback_gen(int nRows, emxArray_real32_T *costab,
                                     emxArray_real32_T *sintab,
                                     int sintabinv_size[2]);

void c_FFTImplementationCallback_get(int nfft, boolean_T useRadix2, int *n2blue,
                                     int *nRows);

void c_FFTImplementationCallback_r2b(const creal_T x_data[], int x_size,
                                     int n1_unsigned,
                                     const emxArray_real_T *costab,
                                     const emxArray_real_T *sintab,
                                     emxArray_creal_T *y);

void d_FFTImplementationCallback_gen(int nRows, emxArray_real_T *costab,
                                     emxArray_real_T *sintab,
                                     int sintabinv_size[2]);

void d_FFTImplementationCallback_r2b(const emxArray_creal32_T *x,
                                     int n1_unsigned,
                                     const emxArray_real32_T *costab,
                                     const emxArray_real32_T *sintab,
                                     emxArray_creal32_T *y);

void e_FFTImplementationCallback_gen(int nRows, emxArray_real32_T *costab,
                                     emxArray_real32_T *sintab,
                                     int sintabinv_size[2]);

void e_FFTImplementationCallback_r2b(const emxArray_creal32_T *x,
                                     int n1_unsigned,
                                     const emxArray_real32_T *costab,
                                     const emxArray_real32_T *sintab,
                                     emxArray_creal32_T *y);

#ifdef __cplusplus
}
#endif

#endif
/*
 * File trailer for FFTImplementationCallback.h
 *
 * [EOF]
 */

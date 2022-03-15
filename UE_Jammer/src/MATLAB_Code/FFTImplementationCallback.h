/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: FFTImplementationCallback.h
 *
 * MATLAB Coder version            : 5.2
 * C/C++ source code generated on  : 20-Feb-2022 08:55:56
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
void c_FFTImplementationCallback_dob(const emxArray_creal32_T *x, int n2blue,
                                     int nfft, const emxArray_real32_T *costab,
                                     const emxArray_real32_T *sintab,
                                     const emxArray_real32_T *sintabinv,
                                     emxArray_creal32_T *y);

void c_FFTImplementationCallback_gen(int nRows, boolean_T useRadix2,
                                     emxArray_real32_T *costab,
                                     emxArray_real32_T *sintab,
                                     emxArray_real32_T *sintabinv);

void c_FFTImplementationCallback_get(int nfft, boolean_T useRadix2, int *n2blue,
                                     int *nRows);

void c_FFTImplementationCallback_r2b(const emxArray_creal32_T *x,
                                     int n1_unsigned,
                                     const emxArray_real32_T *costab,
                                     const emxArray_real32_T *sintab,
                                     emxArray_creal32_T *y);

void d_FFTImplementationCallback_dob(const emxArray_creal32_T *x, int n2blue,
                                     int nfft, const emxArray_real32_T *costab,
                                     const emxArray_real32_T *sintab,
                                     const emxArray_real32_T *sintabinv,
                                     emxArray_creal32_T *y);

void d_FFTImplementationCallback_gen(int nRows, boolean_T useRadix2,
                                     emxArray_real32_T *costab,
                                     emxArray_real32_T *sintab,
                                     emxArray_real32_T *sintabinv);

void d_FFTImplementationCallback_r2b(const emxArray_creal32_T *x,
                                     int unsigned_nRows,
                                     const emxArray_real32_T *costab,
                                     const emxArray_real32_T *sintab,
                                     emxArray_creal32_T *y);

void e_FFTImplementationCallback_dob(
    const creal_T x_data[], int n2blue, const double costab_data[],
    const int costab_size[2], const double sintab_data[],
    const int sintab_size[2], const double sintabinv_data[],
    const int sintabinv_size[2], creal_T y_data[], int y_size[3]);

void e_FFTImplementationCallback_gen(int nRows, double costab_data[],
                                     int costab_size[2], double sintab_data[],
                                     int sintab_size[2],
                                     double sintabinv_data[],
                                     int sintabinv_size[2]);

void e_FFTImplementationCallback_r2b(const emxArray_creal_T *x,
                                     int unsigned_nRows,
                                     const emxArray_real_T *costab,
                                     const emxArray_real_T *sintab,
                                     emxArray_creal_T *y);

void f_FFTImplementationCallback_dob(const creal_T x_data[], int n2blue,
                                     int nfft, const emxArray_real_T *costab,
                                     const emxArray_real_T *sintab,
                                     const emxArray_real_T *sintabinv,
                                     emxArray_creal_T *y);

void f_FFTImplementationCallback_gen(int nRows, boolean_T useRadix2,
                                     emxArray_real_T *costab,
                                     emxArray_real_T *sintab,
                                     emxArray_real_T *sintabinv);

void g_FFTImplementationCallback_dob(const emxArray_creal32_T *x, int n2blue,
                                     const emxArray_real32_T *costab,
                                     const emxArray_real32_T *sintab,
                                     const emxArray_real32_T *sintabinv,
                                     emxArray_creal32_T *y);

void g_FFTImplementationCallback_gen(int nRows, emxArray_real32_T *costab,
                                     emxArray_real32_T *sintab,
                                     emxArray_real32_T *sintabinv);

#ifdef __cplusplus
}
#endif

#endif
/*
 * File trailer for FFTImplementationCallback.h
 *
 * [EOF]
 */

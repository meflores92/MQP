/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: operation_rtwutil.h
 *
 * MATLAB Coder version            : 5.2
 * C/C++ source code generated on  : 20-Feb-2022 08:55:56
 */

#ifndef OPERATION_RTWUTIL_H
#define OPERATION_RTWUTIL_H

/* Include Files */
#include "rtwtypes.h"
#include "omp.h"
#include <stddef.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Function Declarations */
extern int div_s32(int numerator, int denominator);

extern int div_s32_floor(int numerator, int denominator);

extern double rt_atan2d_snf(double u0, double u1);

extern float rt_atan2f_snf(float u0, float u1);

extern float rt_hypotf_snf(float u0, float u1);

extern double rt_powd_snf(double u0, double u1);

extern double rt_remd_snf(double u0, double u1);

extern double rt_roundd_snf(double u);

#ifdef __cplusplus
}
#endif

#endif
/*
 * File trailer for operation_rtwutil.h
 *
 * [EOF]
 */

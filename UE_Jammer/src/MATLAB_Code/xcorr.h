/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: xcorr.h
 *
 * MATLAB Coder version            : 5.2
 * C/C++ source code generated on  : 20-Feb-2022 08:55:56
 */

#ifndef XCORR_H
#define XCORR_H

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
void xcorr(const emxArray_creal32_T *x, const creal_T varargin_1_data[],
           int varargin_1_size, emxArray_creal32_T *c);

#ifdef __cplusplus
}
#endif

#endif
/*
 * File trailer for xcorr.h
 *
 * [EOF]
 */

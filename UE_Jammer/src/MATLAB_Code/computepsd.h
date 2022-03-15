/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: computepsd.h
 *
 * MATLAB Coder version            : 5.2
 * C/C++ source code generated on  : 20-Feb-2022 08:55:56
 */

#ifndef COMPUTEPSD_H
#define COMPUTEPSD_H

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
void computepsd(const emxArray_real32_T *Sxx1, const double w2_data[],
                int w2_size, const char range[8],
                emxArray_real32_T *varargout_1, double varargout_2_data[],
                int *varargout_2_size);

#ifdef __cplusplus
}
#endif

#endif
/*
 * File trailer for computepsd.h
 *
 * [EOF]
 */

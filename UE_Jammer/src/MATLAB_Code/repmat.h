/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: repmat.h
 *
 * MATLAB Coder version            : 5.2
 * C/C++ source code generated on  : 20-Feb-2022 08:55:56
 */

#ifndef REPMAT_H
#define REPMAT_H

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
void b_repmat(const emxArray_creal32_T *a, const double varargin_1[2],
              emxArray_creal32_T *b);

void repmat(double a, const double varargin_1[2], emxArray_real_T *b);

#ifdef __cplusplus
}
#endif

#endif
/*
 * File trailer for repmat.h
 *
 * [EOF]
 */

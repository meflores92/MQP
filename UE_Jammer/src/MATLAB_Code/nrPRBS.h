/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: nrPRBS.h
 *
 * MATLAB Coder version            : 5.2
 * C/C++ source code generated on  : 20-Feb-2022 08:55:56
 */

#ifndef NRPRBS_H
#define NRPRBS_H

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
void b_nrPRBS(const emxArray_real_T *cinit, double n, float seq_data[],
              int *seq_size);

void nrPRBS(double cinit, double n, float seq_data[], int *seq_size);

#ifdef __cplusplus
}
#endif

#endif
/*
 * File trailer for nrPRBS.h
 *
 * [EOF]
 */

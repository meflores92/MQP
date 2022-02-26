/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: computeDFT.h
 *
 * MATLAB Coder version            : 5.2
 * C/C++ source code generated on  : 20-Feb-2022 08:55:56
 */

#ifndef COMPUTEDFT_H
#define COMPUTEDFT_H

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
void computeDFT(const creal32_T xin[921600], emxArray_creal32_T *Xx,
                double f_data[], int *f_size);

#ifdef __cplusplus
}
#endif

#endif
/*
 * File trailer for computeDFT.h
 *
 * [EOF]
 */

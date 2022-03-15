/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: ifft.h
 *
 * MATLAB Coder version            : 5.2
 * C/C++ source code generated on  : 20-Feb-2022 08:55:56
 */

#ifndef IFFT_H
#define IFFT_H

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
void b_ifft(const emxArray_creal32_T *x, emxArray_creal32_T *y);

void ifft(const creal32_T x_data[], int x_size, creal32_T y_data[],
          int *y_size);

#ifdef __cplusplus
}
#endif

#endif
/*
 * File trailer for ifft.h
 *
 * [EOF]
 */

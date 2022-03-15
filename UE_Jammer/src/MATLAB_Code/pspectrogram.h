/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: pspectrogram.h
 *
 * MATLAB Coder version            : 5.2
 * C/C++ source code generated on  : 20-Feb-2022 08:55:56
 */

#ifndef PSPECTROGRAM_H
#define PSPECTROGRAM_H

/* Include Files */
#include "operation_internal_types.h"
#include "operation_types.h"
#include "rtwtypes.h"
#include "omp.h"
#include <stddef.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Function Declarations */
void centerOutputs(emxArray_creal32_T *y, double f_data[], const int *f_size,
                   emxArray_real32_T *Pxx);

void compute_PSD(const emxArray_creal32_T *y, double f_data[], int *f_size,
                 const b_struct_T options, emxArray_real32_T *Pxx);

void formatSpectrogram(const emxArray_creal32_T *y, double f_data[],
                       int *f_size, const b_struct_T options,
                       emxArray_creal32_T *yout);

#ifdef __cplusplus
}
#endif

#endif
/*
 * File trailer for pspectrogram.h
 *
 * [EOF]
 */

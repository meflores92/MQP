/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: spectrogram.h
 *
 * MATLAB Coder version            : 5.2
 * C/C++ source code generated on  : 20-Feb-2022 08:55:56
 */

#ifndef SPECTROGRAM_H
#define SPECTROGRAM_H

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
void spectrogram(const creal32_T x[921600], emxArray_creal32_T *varargout_1,
                 float varargout_2_data[], int *varargout_2_size,
                 float varargout_3[600], emxArray_real32_T *varargout_4);

#ifdef __cplusplus
}
#endif

#endif
/*
 * File trailer for spectrogram.h
 *
 * [EOF]
 */

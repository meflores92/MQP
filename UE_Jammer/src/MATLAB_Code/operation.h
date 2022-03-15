/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: operation.h
 *
 * MATLAB Coder version            : 5.2
 * C/C++ source code generated on  : 20-Feb-2022 08:55:56
 */

#ifndef OPERATION_H
#define OPERATION_H

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
extern void operation(const emxArray_creal32_T *rxWaveform,
                      unsigned short dmrsRNTI, unsigned short scramblingRNTI,
                      unsigned short UERNTI, unsigned short slot,
                      unsigned short LRB, unsigned short RBStart,
                      creal_T s[921600], creal_T w[1536], creal_T t[600],
                      creal_T ps[921600], creal32_T waveform[1536]);

#ifdef __cplusplus
}
#endif

#endif
/*
 * File trailer for operation.h
 *
 * [EOF]
 */

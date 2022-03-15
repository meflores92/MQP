/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: nrDCIEncode.h
 *
 * MATLAB Coder version            : 5.2
 * C/C++ source code generated on  : 20-Feb-2022 08:55:56
 */

#ifndef NRDCIENCODE_H
#define NRDCIENCODE_H

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
void nrDCIEncode(const signed char dciBits[39], unsigned int rnti,
                 emxArray_int8_T *dciCW);

#ifdef __cplusplus
}
#endif

#endif
/*
 * File trailer for nrDCIEncode.h
 *
 * [EOF]
 */

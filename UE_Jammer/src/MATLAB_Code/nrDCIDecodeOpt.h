/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: nrDCIDecodeOpt.h
 *
 * MATLAB Coder version            : 5.2
 * C/C++ source code generated on  : 20-Feb-2022 08:55:56
 */

#ifndef NRDCIDECODEOPT_H
#define NRDCIDECODEOPT_H

/* Include Files */
#include "rtwtypes.h"
#include "omp.h"
#include <stddef.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Function Declarations */
void nrDCIDecodeOpt(const float dciCW_data[], int dciCW_size,
                    signed char dciBits[39], unsigned int *mask);

#ifdef __cplusplus
}
#endif

#endif
/*
 * File trailer for nrDCIDecodeOpt.h
 *
 * [EOF]
 */

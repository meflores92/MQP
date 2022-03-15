/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: nrChannelEstimateOpt.h
 *
 * MATLAB Coder version            : 5.2
 * C/C++ source code generated on  : 20-Feb-2022 08:55:56
 */

#ifndef NRCHANNELESTIMATEOPT_H
#define NRCHANNELESTIMATEOPT_H

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
void nrChannelEstimateOpt(const creal32_T rxGrid[624], emxArray_creal32_T *H,
                          float *nVar);

#ifdef __cplusplus
}
#endif

#endif
/*
 * File trailer for nrChannelEstimateOpt.h
 *
 * [EOF]
 */

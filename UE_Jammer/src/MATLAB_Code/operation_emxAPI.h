/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: operation_emxAPI.h
 *
 * MATLAB Coder version            : 5.2
 * C/C++ source code generated on  : 20-Feb-2022 08:55:56
 */

#ifndef OPERATION_EMXAPI_H
#define OPERATION_EMXAPI_H

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
extern emxArray_creal32_T *emxCreateND_creal32_T(int numDimensions,
                                                 const int *size);

extern emxArray_creal32_T *emxCreateWrapperND_creal32_T(creal32_T *data,
                                                        int numDimensions,
                                                        const int *size);

extern emxArray_creal32_T *emxCreateWrapper_creal32_T(creal32_T *data, int rows,
                                                      int cols);

extern emxArray_creal32_T *emxCreate_creal32_T(int rows, int cols);

extern void emxDestroyArray_creal32_T(emxArray_creal32_T *emxArray);

extern void emxInitArray_creal32_T(emxArray_creal32_T **pEmxArray,
                                   int numDimensions);

#ifdef __cplusplus
}
#endif

#endif
/*
 * File trailer for operation_emxAPI.h
 *
 * [EOF]
 */

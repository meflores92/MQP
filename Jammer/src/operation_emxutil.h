/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: operation_emxutil.h
 *
 * MATLAB Coder version            : 5.2
 * C/C++ source code generated on  : 20-Feb-2022 08:55:56
 */

#ifndef OPERATION_EMXUTIL_H
#define OPERATION_EMXUTIL_H

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
extern void emxEnsureCapacity_boolean_T(emxArray_boolean_T *emxArray,
                                        int oldNumel);

extern void emxEnsureCapacity_cell_wrap_8(emxArray_cell_wrap_8 *emxArray,
                                          int oldNumel);

extern void emxEnsureCapacity_creal32_T(emxArray_creal32_T *emxArray,
                                        int oldNumel);

extern void emxEnsureCapacity_creal_T(emxArray_creal_T *emxArray, int oldNumel);

extern void emxEnsureCapacity_int16_T(emxArray_int16_T *emxArray, int oldNumel);

extern void emxEnsureCapacity_int32_T(emxArray_int32_T *emxArray, int oldNumel);

extern void emxEnsureCapacity_int8_T(emxArray_int8_T *emxArray, int oldNumel);

extern void emxEnsureCapacity_real32_T(emxArray_real32_T *emxArray,
                                       int oldNumel);

extern void emxEnsureCapacity_real_T(emxArray_real_T *emxArray, int oldNumel);

extern void emxEnsureCapacity_uint16_T(emxArray_uint16_T *emxArray,
                                       int oldNumel);

extern void emxEnsureCapacity_uint32_T(emxArray_uint32_T *emxArray,
                                       int oldNumel);

extern void emxExpand_cell_wrap_8(emxArray_cell_wrap_8 *emxArray, int fromIndex,
                                  int toIndex);

extern void emxFreeMatrix_cell_wrap_8(cell_wrap_8 pMatrix[9]);

extern void emxFreeStruct_cell_wrap_8(cell_wrap_8 *pStruct);

extern void emxFreeStruct_struct_T(c_struct_T *pStruct);

extern void emxFree_boolean_T(emxArray_boolean_T **pEmxArray);

extern void emxFree_cell_wrap_8(emxArray_cell_wrap_8 **pEmxArray);

extern void emxFree_creal32_T(emxArray_creal32_T **pEmxArray);

extern void emxFree_creal_T(emxArray_creal_T **pEmxArray);

extern void emxFree_int16_T(emxArray_int16_T **pEmxArray);

extern void emxFree_int32_T(emxArray_int32_T **pEmxArray);

extern void emxFree_int8_T(emxArray_int8_T **pEmxArray);

extern void emxFree_real32_T(emxArray_real32_T **pEmxArray);

extern void emxFree_real_T(emxArray_real_T **pEmxArray);

extern void emxFree_uint16_T(emxArray_uint16_T **pEmxArray);

extern void emxFree_uint32_T(emxArray_uint32_T **pEmxArray);

extern void emxInitMatrix_cell_wrap_8(cell_wrap_8 pMatrix[9]);

extern void emxInitStruct_cell_wrap_8(cell_wrap_8 *pStruct);

extern void emxInitStruct_struct_T(c_struct_T *pStruct);

extern void emxInit_boolean_T(emxArray_boolean_T **pEmxArray,
                              int numDimensions);

extern void emxInit_cell_wrap_8(emxArray_cell_wrap_8 **pEmxArray,
                                int numDimensions);

extern void emxInit_creal32_T(emxArray_creal32_T **pEmxArray,
                              int numDimensions);

extern void emxInit_creal_T(emxArray_creal_T **pEmxArray, int numDimensions);

extern void emxInit_int16_T(emxArray_int16_T **pEmxArray, int numDimensions);

extern void emxInit_int32_T(emxArray_int32_T **pEmxArray, int numDimensions);

extern void emxInit_int8_T(emxArray_int8_T **pEmxArray, int numDimensions);

extern void emxInit_real32_T(emxArray_real32_T **pEmxArray, int numDimensions);

extern void emxInit_real_T(emxArray_real_T **pEmxArray, int numDimensions);

extern void emxInit_uint16_T(emxArray_uint16_T **pEmxArray, int numDimensions);

extern void emxInit_uint32_T(emxArray_uint32_T **pEmxArray, int numDimensions);

extern void emxTrim_cell_wrap_8(emxArray_cell_wrap_8 *emxArray, int fromIndex,
                                int toIndex);

#ifdef __cplusplus
}
#endif

#endif
/*
 * File trailer for operation_emxutil.h
 *
 * [EOF]
 */

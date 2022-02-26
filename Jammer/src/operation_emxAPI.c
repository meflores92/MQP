/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: operation_emxAPI.c
 *
 * MATLAB Coder version            : 5.2
 * C/C++ source code generated on  : 20-Feb-2022 08:55:56
 */

/* Include Files */
#include "operation_emxAPI.h"
#include "operation_emxutil.h"
#include "operation_types.h"
#include "rt_nonfinite.h"
#include <stdlib.h>
#include <string.h>

/* Function Definitions */
/*
 * Arguments    : int numDimensions
 *                const int *size
 * Return Type  : emxArray_creal32_T *
 */
emxArray_creal32_T *emxCreateND_creal32_T(int numDimensions, const int *size)
{
  emxArray_creal32_T *emx;
  int i;
  int numEl;
  emxInit_creal32_T(&emx, numDimensions);
  numEl = 1;
  for (i = 0; i < numDimensions; i++) {
    numEl *= size[i];
    emx->size[i] = size[i];
  }
  emx->data = (creal32_T *)calloc((unsigned int)numEl, sizeof(creal32_T));
  emx->numDimensions = numDimensions;
  emx->allocatedSize = numEl;
  return emx;
}

/*
 * Arguments    : creal32_T *data
 *                int numDimensions
 *                const int *size
 * Return Type  : emxArray_creal32_T *
 */
emxArray_creal32_T *emxCreateWrapperND_creal32_T(creal32_T *data,
                                                 int numDimensions,
                                                 const int *size)
{
  emxArray_creal32_T *emx;
  int i;
  int numEl;
  emxInit_creal32_T(&emx, numDimensions);
  numEl = 1;
  for (i = 0; i < numDimensions; i++) {
    numEl *= size[i];
    emx->size[i] = size[i];
  }
  emx->data = data;
  emx->numDimensions = numDimensions;
  emx->allocatedSize = numEl;
  emx->canFreeData = false;
  return emx;
}

/*
 * Arguments    : creal32_T *data
 *                int rows
 *                int cols
 * Return Type  : emxArray_creal32_T *
 */
emxArray_creal32_T *emxCreateWrapper_creal32_T(creal32_T *data, int rows,
                                               int cols)
{
  emxArray_creal32_T *emx;
  emxInit_creal32_T(&emx, 2);
  emx->size[0] = rows;
  emx->size[1] = cols;
  emx->data = data;
  emx->numDimensions = 2;
  emx->allocatedSize = rows * cols;
  emx->canFreeData = false;
  return emx;
}

/*
 * Arguments    : int rows
 *                int cols
 * Return Type  : emxArray_creal32_T *
 */
emxArray_creal32_T *emxCreate_creal32_T(int rows, int cols)
{
  emxArray_creal32_T *emx;
  int numEl;
  emxInit_creal32_T(&emx, 2);
  emx->size[0] = rows;
  numEl = rows * cols;
  emx->size[1] = cols;
  emx->data = (creal32_T *)calloc((unsigned int)numEl, sizeof(creal32_T));
  emx->numDimensions = 2;
  emx->allocatedSize = numEl;
  return emx;
}

/*
 * Arguments    : emxArray_creal32_T *emxArray
 * Return Type  : void
 */
void emxDestroyArray_creal32_T(emxArray_creal32_T *emxArray)
{
  emxFree_creal32_T(&emxArray);
}

/*
 * Arguments    : emxArray_creal32_T **pEmxArray
 *                int numDimensions
 * Return Type  : void
 */
void emxInitArray_creal32_T(emxArray_creal32_T **pEmxArray, int numDimensions)
{
  emxInit_creal32_T(pEmxArray, numDimensions);
}

/*
 * File trailer for operation_emxAPI.c
 *
 * [EOF]
 */

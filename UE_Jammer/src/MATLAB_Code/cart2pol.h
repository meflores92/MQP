/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: cart2pol.h
 *
 * MATLAB Coder version            : 5.2
 * C/C++ source code generated on  : 20-Feb-2022 08:55:56
 */

#ifndef CART2POL_H
#define CART2POL_H

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
void cart2pol(const emxArray_real32_T *x, const emxArray_real32_T *y,
              emxArray_real32_T *th, emxArray_real32_T *r);

#ifdef __cplusplus
}
#endif

#endif
/*
 * File trailer for cart2pol.h
 *
 * [EOF]
 */

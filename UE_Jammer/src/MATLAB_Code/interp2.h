/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: interp2.h
 *
 * MATLAB Coder version            : 5.2
 * C/C++ source code generated on  : 20-Feb-2022 08:55:56
 */

#ifndef INTERP2_H
#define INTERP2_H

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
void interp2_local(const emxArray_creal32_T *V, const emxArray_real_T *Xq,
                   const emxArray_real_T *Yq, const double X_data[],
                   const int X_size[2], const emxArray_real_T *Y,
                   emxArray_creal32_T *Vq);

#ifdef __cplusplus
}
#endif

#endif
/*
 * File trailer for interp2.h
 *
 * [EOF]
 */

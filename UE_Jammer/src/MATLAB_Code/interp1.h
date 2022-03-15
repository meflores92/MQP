/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: interp1.h
 *
 * MATLAB Coder version            : 5.2
 * C/C++ source code generated on  : 20-Feb-2022 08:55:56
 */

#ifndef INTERP1_H
#define INTERP1_H

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
void b_interp1(const emxArray_real_T *varargin_1,
               const emxArray_real32_T *varargin_2,
               const emxArray_real_T *varargin_3, emxArray_real32_T *Vq);

void b_interp1Linear(const float y_data[], int nyrows,
                     const emxArray_real_T *xi, emxArray_real32_T *yi,
                     const emxArray_real_T *varargin_1);

creal32_T c_interp1(const double varargin_1_data[],
                    const int varargin_1_size[2],
                    const creal32_T varargin_2_data[],
                    const int varargin_2_size[2], double varargin_3);

void interp1(const emxArray_real_T *varargin_1,
             const emxArray_creal32_T *varargin_2,
             const emxArray_real_T *varargin_3, emxArray_creal32_T *Vq);

void interp1Linear(const float y_data[], int nyrows, const emxArray_real_T *xi,
                   emxArray_real32_T *yi, const emxArray_real_T *varargin_1);

#ifdef __cplusplus
}
#endif

#endif
/*
 * File trailer for interp1.h
 *
 * [EOF]
 */

/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: meshgrid.h
 *
 * MATLAB Coder version            : 5.2
 * C/C++ source code generated on  : 20-Feb-2022 08:55:56
 */

#ifndef MESHGRID_H
#define MESHGRID_H

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
void meshgrid(const double x_data[], const int x_size[2],
              const emxArray_real_T *y, emxArray_real_T *xx,
              emxArray_real_T *yy);

#ifdef __cplusplus
}
#endif

#endif
/*
 * File trailer for meshgrid.h
 *
 * [EOF]
 */

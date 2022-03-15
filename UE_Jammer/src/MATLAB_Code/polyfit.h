/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: polyfit.h
 *
 * MATLAB Coder version            : 5.2
 * C/C++ source code generated on  : 20-Feb-2022 08:55:56
 */

#ifndef POLYFIT_H
#define POLYFIT_H

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
void polyfit(const emxArray_real_T *x, const float y_data[], int y_size,
             float p[2]);

#ifdef __cplusplus
}
#endif

#endif
/*
 * File trailer for polyfit.h
 *
 * [EOF]
 */

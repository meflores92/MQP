/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: sortLE.h
 *
 * MATLAB Coder version            : 5.2
 * C/C++ source code generated on  : 20-Feb-2022 08:55:56
 */

#ifndef SORTLE_H
#define SORTLE_H

/* Include Files */
#include "rtwtypes.h"
#include "omp.h"
#include <stddef.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Function Declarations */
boolean_T sortLE(const double v_data[], const int v_size[2],
                 const int dir_data[], const int dir_size[2], int idx1,
                 int idx2);

#ifdef __cplusplus
}
#endif

#endif
/*
 * File trailer for sortLE.h
 *
 * [EOF]
 */

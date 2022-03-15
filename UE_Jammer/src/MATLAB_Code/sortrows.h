/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: sortrows.h
 *
 * MATLAB Coder version            : 5.2
 * C/C++ source code generated on  : 20-Feb-2022 08:55:56
 */

#ifndef SORTROWS_H
#define SORTROWS_H

/* Include Files */
#include "rtwtypes.h"
#include "omp.h"
#include <stddef.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Function Declarations */
void sortrows(double y_data[], const int y_size[2], double ndx_data[],
              int *ndx_size);

#ifdef __cplusplus
}
#endif

#endif
/*
 * File trailer for sortrows.h
 *
 * [EOF]
 */

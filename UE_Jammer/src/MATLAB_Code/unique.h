/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: unique.h
 *
 * MATLAB Coder version            : 5.2
 * C/C++ source code generated on  : 20-Feb-2022 08:55:56
 */

#ifndef UNIQUE_H
#define UNIQUE_H

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
void b_unique_rows(const double a_data[], const int a_size[2], double b_data[],
                   int b_size[2], int ndx_data[], int *ndx_size, int pos_data[],
                   int *pos_size);

void count_nonfinites(const emxArray_real_T *b, int nb, int *nMInf,
                      int *nFinite, int *nPInf, int *nNaN);

void unique_rows(const double a_data[], double b_data[], int b_size[2]);

void unique_vector(const emxArray_real_T *a, emxArray_real_T *b);

#ifdef __cplusplus
}
#endif

#endif
/*
 * File trailer for unique.h
 *
 * [EOF]
 */

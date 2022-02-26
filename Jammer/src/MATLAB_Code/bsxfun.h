/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: bsxfun.h
 *
 * MATLAB Coder version            : 5.2
 * C/C++ source code generated on  : 20-Feb-2022 08:55:56
 */

#ifndef BSXFUN_H
#define BSXFUN_H

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
void b_bsxfun(const emxArray_real_T *a, emxArray_real_T *c);

void bsxfun(const unsigned int a_data[], const int a_size[2],
            const unsigned int b_data[], const int b_size[2],
            unsigned int c_data[], int c_size[2]);

#ifdef __cplusplus
}
#endif

#endif
/*
 * File trailer for bsxfun.h
 *
 * [EOF]
 */

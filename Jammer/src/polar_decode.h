/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: polar_decode.h
 *
 * MATLAB Coder version            : 5.2
 * C/C++ source code generated on  : 20-Feb-2022 08:55:56
 */

#ifndef POLAR_DECODE_H
#define POLAR_DECODE_H

/* Include Files */
#include "rtwtypes.h"
#include "omp.h"
#include <stddef.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Function Declarations */
void f_polar_decode(const double y[128],
                    const double frozen_bits_indicator[128], double u_data[],
                    int u_size[2], double v[128]);

void polar_decode(double u_data[], int u_size[2], double v[64]);

#ifdef __cplusplus
}
#endif

#endif
/*
 * File trailer for polar_decode.h
 *
 * [EOF]
 */

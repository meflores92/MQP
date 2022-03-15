/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: operation_internal_types.h
 *
 * MATLAB Coder version            : 5.2
 * C/C++ source code generated on  : 20-Feb-2022 08:55:56
 */

#ifndef OPERATION_INTERNAL_TYPES_H
#define OPERATION_INTERNAL_TYPES_H

/* Include Files */
#include "operation_types.h"
#include "rtwtypes.h"

/* Type Definitions */
#ifndef typedef_b_struct_T
#define typedef_b_struct_T
typedef struct {
  double nfft;
  double Fs;
  double conflevel;
  boolean_T average;
  boolean_T maxhold;
  boolean_T minhold;
  boolean_T MIMO;
  boolean_T isNFFTSingle;
  boolean_T centerdc;
  char range[8];
} b_struct_T;
#endif /* typedef_b_struct_T */

#ifndef typedef_cell_wrap_66
#define typedef_cell_wrap_66
typedef struct {
  creal32_T f1[921600];
} cell_wrap_66;
#endif /* typedef_cell_wrap_66 */

#endif
/*
 * File trailer for operation_internal_types.h
 *
 * [EOF]
 */

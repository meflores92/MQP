/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: eml_setop.h
 *
 * MATLAB Coder version            : 5.2
 * C/C++ source code generated on  : 20-Feb-2022 08:55:56
 */

#ifndef EML_SETOP_H
#define EML_SETOP_H

/* Include Files */
#include "rtwtypes.h"
#include "omp.h"
#include <stddef.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Function Declarations */
void b_do_vectors(double c_data[], int *c_size, int ia_data[], int *ia_size,
                  int *ib_size);

void do_vectors(double c_data[], int *c_size, int ia_data[], int *ia_size,
                int *ib_size);

#ifdef __cplusplus
}
#endif

#endif
/*
 * File trailer for eml_setop.h
 *
 * [EOF]
 */

/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: modulate.h
 *
 * MATLAB Coder version            : 5.2
 * C/C++ source code generated on  : 19-Jan-2022 16:08:45
 */

#ifndef MODULATE_H
#define MODULATE_H

/* Include Files */
#include "rtwtypes.h"
#include "omp.h"
#include <stddef.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Function Declarations */
void modulate(const creal_T gridIn_data[],
              const double prmStr_CyclicPrefixLength[2], creal_T yout_data[],
              int yout_size[2]);

#ifdef __cplusplus
}
#endif

#endif
/*
 * File trailer for modulate.h
 *
 * [EOF]
 */

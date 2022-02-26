/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: OFDMModulate.h
 *
 * MATLAB Coder version            : 5.2
 * C/C++ source code generated on  : 20-Feb-2022 08:55:56
 */

#ifndef OFDMMODULATE_H
#define OFDMMODULATE_H

/* Include Files */
#include "rtwtypes.h"
#include "omp.h"
#include <stddef.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Function Declarations */
void OFDMModulate(const double grid[480], creal_T waveform_data[],
                  int waveform_size[2]);

#ifdef __cplusplus
}
#endif

#endif
/*
 * File trailer for OFDMModulate.h
 *
 * [EOF]
 */

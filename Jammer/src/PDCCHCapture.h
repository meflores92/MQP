/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: PDCCHCapture.h
 *
 * MATLAB Coder version            : 5.2
 * C/C++ source code generated on  : 20-Feb-2022 08:55:56
 */

#ifndef PDCCHCAPTURE_H
#define PDCCHCAPTURE_H

/* Include Files */
#include "rtwtypes.h"
#include "omp.h"
#include <stddef.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Function Declarations */
void PDCCHCapture(const creal32_T rxWaveN[1668], unsigned short dmrsRNTI,
                  unsigned short scramblingRNTI, unsigned short UERNTI,
                  unsigned short nSlot, signed char dciBits[39],
                  double *isValid, float IPhase[216], float QPhase[216]);

#ifdef __cplusplus
}
#endif

#endif
/*
 * File trailer for PDCCHCapture.h
 *
 * [EOF]
 */

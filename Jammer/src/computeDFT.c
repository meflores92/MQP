/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: computeDFT.c
 *
 * MATLAB Coder version            : 5.2
 * C/C++ source code generated on  : 20-Feb-2022 08:55:56
 */

/* Include Files */
#include "computeDFT.h"
#include "FFTImplementationCallback.h"
#include "operation_emxutil.h"
#include "operation_types.h"
#include "psdfreqvec.h"
#include "rt_nonfinite.h"
#include <string.h>

/* Function Definitions */
/*
 * Arguments    : const creal32_T xin[921600]
 *                emxArray_creal32_T *Xx
 *                double f_data[]
 *                int *f_size
 * Return Type  : void
 */
void computeDFT(const creal32_T xin[921600], emxArray_creal32_T *Xx,
                double f_data[], int *f_size)
{
  emxArray_creal32_T b_xin;
  emxArray_real32_T *costab;
  emxArray_real32_T *sintab;
  emxArray_real32_T *sintabinv;
  int b_iv[2];
  int N2blue;
  int nRows;
  emxInit_real32_T(&costab, 2);
  emxInit_real32_T(&sintab, 2);
  emxInit_real32_T(&sintabinv, 2);
  c_FFTImplementationCallback_get(1536, false, &N2blue, &nRows);
  g_FFTImplementationCallback_gen(nRows, costab, sintab, sintabinv);
  b_xin.data = (creal32_T *)&xin[0];
  b_iv[0] = 1536;
  b_iv[1] = 600;
  b_xin.size = &b_iv[0];
  b_xin.allocatedSize = 921600;
  b_xin.numDimensions = 2;
  b_xin.canFreeData = false;
  g_FFTImplementationCallback_dob(&b_xin, N2blue, costab, sintab, sintabinv,
                                  Xx);
  psdfreqvec(f_data, f_size);
  emxFree_real32_T(&sintabinv);
  emxFree_real32_T(&sintab);
  emxFree_real32_T(&costab);
}

/*
 * File trailer for computeDFT.c
 *
 * [EOF]
 */

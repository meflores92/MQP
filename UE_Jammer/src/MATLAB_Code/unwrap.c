/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: unwrap.c
 *
 * MATLAB Coder version            : 5.2
 * C/C++ source code generated on  : 20-Feb-2022 08:55:56
 */

/* Include Files */
#include "unwrap.h"
#include "operation_emxutil.h"
#include "operation_types.h"
#include "rt_nonfinite.h"
#include "rt_nonfinite.h"
#include <float.h>
#include <math.h>
#include <string.h>

/* Function Declarations */
static float rt_remf_snf(float u0, float u1);

/* Function Definitions */
/*
 * Arguments    : float u0
 *                float u1
 * Return Type  : float
 */
static float rt_remf_snf(float u0, float u1)
{
  float q;
  float y;
  if (rtIsNaNF(u0) || rtIsNaNF(u1) || rtIsInfF(u0)) {
    y = rtNaNF;
  } else if (rtIsInfF(u1)) {
    y = u0;
  } else if ((u1 != 0.0F) && (u1 != truncf(u1))) {
    q = fabsf(u0 / u1);
    if (!(fabsf(q - floorf(q + 0.5F)) > FLT_EPSILON * q)) {
      y = 0.0F * u0;
    } else {
      y = fmodf(u0, u1);
    }
  } else {
    y = fmodf(u0, u1);
  }
  return y;
}

/*
 * Arguments    : emxArray_real32_T *p
 * Return Type  : void
 */
void unwrap(emxArray_real32_T *p)
{
  emxArray_real32_T *vwork;
  float cumsum_dp_corr;
  float dp;
  float dp_tmp;
  float pkm1;
  int dim;
  int exitg2;
  int j;
  unsigned int k;
  int m;
  int vlen;
  int vstride;
  boolean_T exitg1;
  dim = 0;
  if (p->size[0] != 1) {
    dim = -1;
  }
  emxInit_real32_T(&vwork, 1);
  if (dim + 2 <= 1) {
    vlen = p->size[0] - 1;
  } else {
    vlen = 0;
  }
  m = vwork->size[0];
  vwork->size[0] = vlen + 1;
  emxEnsureCapacity_real32_T(vwork, m);
  vstride = 1;
  for (m = 0; m <= dim; m++) {
    vstride *= p->size[0];
  }
  dim = -1;
  for (j = 0; j < vstride; j++) {
    dim++;
    for (m = 0; m <= vlen; m++) {
      vwork->data[m] = p->data[dim + m * vstride];
    }
    m = vwork->size[0];
    cumsum_dp_corr = 0.0F;
    k = 1U;
    exitg1 = false;
    while ((!exitg1) && ((int)k < m)) {
      dp = vwork->data[(int)k - 1];
      if (rtIsInfF(dp) || rtIsNaNF(dp)) {
        k = (unsigned int)((int)k + 1);
      } else {
        exitg1 = true;
      }
    }
    if ((int)k < vwork->size[0]) {
      pkm1 = vwork->data[(int)k - 1];
      do {
        exitg2 = 0;
        k++;
        exitg1 = false;
        while ((!exitg1) && (k <= (unsigned int)m)) {
          dp = vwork->data[(int)k - 1];
          if (rtIsInfF(dp) || rtIsNaNF(dp)) {
            k++;
          } else {
            exitg1 = true;
          }
        }
        if (k > (unsigned int)m) {
          exitg2 = 1;
        } else {
          dp_tmp = vwork->data[(int)k - 1];
          dp = dp_tmp - pkm1;
          pkm1 = dp / 6.28318548F;
          if (fabsf(rt_remf_snf(pkm1, 1.0F)) <= 0.5F) {
            pkm1 = truncf(pkm1);
          } else {
            pkm1 = roundf(pkm1);
          }
          if (fabsf(dp) >= 3.14159274F) {
            cumsum_dp_corr += pkm1;
          }
          pkm1 = dp_tmp;
          vwork->data[(int)k - 1] = dp_tmp - 6.28318548F * cumsum_dp_corr;
        }
      } while (exitg2 == 0);
    }
    for (m = 0; m <= vlen; m++) {
      p->data[dim + m * vstride] = vwork->data[m];
    }
  }
  emxFree_real32_T(&vwork);
}

/*
 * File trailer for unwrap.c
 *
 * [EOF]
 */

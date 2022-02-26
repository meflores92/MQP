/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: polyfit.c
 *
 * MATLAB Coder version            : 5.2
 * C/C++ source code generated on  : 20-Feb-2022 08:55:56
 */

/* Include Files */
#include "polyfit.h"
#include "operation_emxutil.h"
#include "operation_types.h"
#include "rt_nonfinite.h"
#include "xnrm2.h"
#include "rt_nonfinite.h"
#include <math.h>
#include <string.h>

/* Function Declarations */
static double rt_hypotd_snf(double u0, double u1);

/* Function Definitions */
/*
 * Arguments    : double u0
 *                double u1
 * Return Type  : double
 */
static double rt_hypotd_snf(double u0, double u1)
{
  double a;
  double y;
  a = fabs(u0);
  y = fabs(u1);
  if (a < y) {
    a /= y;
    y *= sqrt(a * a + 1.0);
  } else if (a > y) {
    y /= a;
    y = a * sqrt(y * y + 1.0);
  } else if (!rtIsNaN(y)) {
    y = a * 1.4142135623730951;
  }
  return y;
}

/*
 * Arguments    : const emxArray_real_T *x
 *                const float y_data[]
 *                int y_size
 *                float p[2]
 * Return Type  : void
 */
void polyfit(const emxArray_real_T *x, const float y_data[], int y_size,
             float p[2])
{
  static float B_data[8736];
  emxArray_real_T *V;
  double tau_data[2];
  double vn1[2];
  double vn2[2];
  double work[2];
  double atmp;
  double beta1;
  double temp;
  float wj;
  int b_i;
  int exitg1;
  int i;
  int ii;
  int ip1;
  int jA;
  int k;
  int knt;
  int lastv;
  int m;
  int ma;
  int minmana;
  int minmn;
  int mmi;
  int pvt;
  signed char jpvt[2];
  emxInit_real_T(&V, 2);
  i = V->size[0] * V->size[1];
  V->size[0] = x->size[0];
  V->size[1] = 2;
  emxEnsureCapacity_real_T(V, i);
  if (x->size[0] != 0) {
    i = x->size[0];
    for (k = 0; k < i; k++) {
      V->data[k + V->size[0]] = 1.0;
    }
    i = x->size[0];
    for (k = 0; k < i; k++) {
      V->data[k] = x->data[k];
    }
  }
  m = V->size[0];
  minmana = V->size[0];
  if (minmana >= 2) {
    minmana = 2;
  }
  if (0 <= minmana - 1) {
    memset(&tau_data[0], 0, minmana * sizeof(double));
  }
  if (V->size[0] == 0) {
    jpvt[0] = 1;
    jpvt[1] = 2;
  } else {
    ma = V->size[0];
    minmn = (V->size[0] >= 2);
    jpvt[0] = 1;
    work[0] = 0.0;
    temp = xnrm2(V->size[0], V, 1);
    vn1[0] = temp;
    vn2[0] = temp;
    jpvt[1] = 2;
    work[1] = 0.0;
    temp = xnrm2(V->size[0], V, V->size[0] + 1);
    vn1[1] = temp;
    vn2[1] = temp;
    for (b_i = 0; b_i <= minmn; b_i++) {
      ip1 = b_i + 2;
      lastv = b_i * ma;
      ii = lastv + b_i;
      mmi = m - b_i;
      minmana = 0;
      if ((2 - b_i > 1) && (fabs(vn1[b_i + 1]) > fabs(vn1[b_i]))) {
        minmana = 1;
      }
      pvt = b_i + minmana;
      if (pvt != b_i) {
        minmana = pvt * ma;
        for (k = 0; k < m; k++) {
          knt = minmana + k;
          temp = V->data[knt];
          i = lastv + k;
          V->data[knt] = V->data[i];
          V->data[i] = temp;
        }
        minmana = jpvt[pvt];
        jpvt[pvt] = jpvt[b_i];
        jpvt[b_i] = (signed char)minmana;
        vn1[pvt] = vn1[b_i];
        vn2[pvt] = vn2[b_i];
      }
      if (b_i + 1 < m) {
        atmp = V->data[ii];
        minmana = ii + 2;
        tau_data[b_i] = 0.0;
        if (mmi > 0) {
          temp = xnrm2(mmi - 1, V, ii + 2);
          if (temp != 0.0) {
            beta1 = rt_hypotd_snf(V->data[ii], temp);
            if (V->data[ii] >= 0.0) {
              beta1 = -beta1;
            }
            if (fabs(beta1) < 1.0020841800044864E-292) {
              knt = -1;
              i = ii + mmi;
              do {
                knt++;
                for (k = minmana; k <= i; k++) {
                  V->data[k - 1] *= 9.9792015476736E+291;
                }
                beta1 *= 9.9792015476736E+291;
                atmp *= 9.9792015476736E+291;
              } while (!(fabs(beta1) >= 1.0020841800044864E-292));
              beta1 = rt_hypotd_snf(atmp, xnrm2(mmi - 1, V, ii + 2));
              if (atmp >= 0.0) {
                beta1 = -beta1;
              }
              tau_data[b_i] = (beta1 - atmp) / beta1;
              temp = 1.0 / (atmp - beta1);
              for (k = minmana; k <= i; k++) {
                V->data[k - 1] *= temp;
              }
              for (k = 0; k <= knt; k++) {
                beta1 *= 1.0020841800044864E-292;
              }
              atmp = beta1;
            } else {
              tau_data[b_i] = (beta1 - V->data[ii]) / beta1;
              temp = 1.0 / (V->data[ii] - beta1);
              i = ii + mmi;
              for (k = minmana; k <= i; k++) {
                V->data[k - 1] *= temp;
              }
              atmp = beta1;
            }
          }
        }
        V->data[ii] = atmp;
      } else {
        tau_data[b_i] = 0.0;
      }
      if (b_i + 1 < 2) {
        atmp = V->data[ii];
        V->data[ii] = 1.0;
        jA = (ii + ma) + 1;
        if (tau_data[0] != 0.0) {
          lastv = mmi - 1;
          minmana = (ii + mmi) - 1;
          while ((lastv + 1 > 0) && (V->data[minmana] == 0.0)) {
            lastv--;
            minmana--;
          }
          knt = 1;
          pvt = jA;
          do {
            exitg1 = 0;
            if (pvt <= jA + lastv) {
              if (V->data[pvt - 1] != 0.0) {
                exitg1 = 1;
              } else {
                pvt++;
              }
            } else {
              knt = 0;
              exitg1 = 1;
            }
          } while (exitg1 == 0);
        } else {
          lastv = -1;
          knt = 0;
        }
        if (lastv + 1 > 0) {
          if (knt != 0) {
            work[0] = 0.0;
            minmana = 0;
            for (k = jA; ma < 0 ? k >= jA : k <= jA; k += ma) {
              temp = 0.0;
              i = k + lastv;
              for (pvt = k; pvt <= i; pvt++) {
                temp += V->data[pvt - 1] * V->data[(ii + pvt) - k];
              }
              work[minmana] += temp;
              minmana++;
            }
          }
          if (!(-tau_data[0] == 0.0)) {
            for (pvt = 0; pvt < knt; pvt++) {
              if (work[0] != 0.0) {
                temp = work[0] * -tau_data[0];
                i = lastv + jA;
                for (minmana = jA; minmana <= i; minmana++) {
                  V->data[minmana - 1] += V->data[(ii + minmana) - jA] * temp;
                }
              }
              jA += ma;
            }
          }
        }
        V->data[ii] = atmp;
      }
      for (pvt = ip1; pvt < 3; pvt++) {
        minmana = b_i + ma;
        if (vn1[1] != 0.0) {
          temp = fabs(V->data[minmana]) / vn1[1];
          temp = 1.0 - temp * temp;
          if (temp < 0.0) {
            temp = 0.0;
          }
          beta1 = vn1[1] / vn2[1];
          beta1 = temp * (beta1 * beta1);
          if (beta1 <= 1.4901161193847656E-8) {
            if (b_i + 1 < m) {
              temp = xnrm2(mmi - 1, V, minmana + 2);
              vn1[1] = temp;
              vn2[1] = temp;
            } else {
              vn1[1] = 0.0;
              vn2[1] = 0.0;
            }
          } else {
            vn1[1] *= sqrt(temp);
          }
        }
      }
    }
  }
  knt = 0;
  minmana = V->size[0];
  if (minmana >= 2) {
    minmana = 2;
  }
  if (minmana > 0) {
    for (k = 0; k < minmana; k++) {
      if (V->data[k + V->size[0] * k] != 0.0) {
        knt++;
      }
    }
  }
  if (0 <= y_size - 1) {
    memcpy(&B_data[0], &y_data[0], y_size * sizeof(float));
  }
  p[0] = 0.0F;
  p[1] = 0.0F;
  m = V->size[0];
  minmana = V->size[0];
  if (minmana >= 2) {
    minmana = 2;
  }
  for (pvt = 0; pvt < minmana; pvt++) {
    if (tau_data[pvt] != 0.0) {
      wj = B_data[pvt];
      i = pvt + 2;
      for (b_i = i; b_i <= m; b_i++) {
        wj += (float)V->data[(b_i + V->size[0] * pvt) - 1] * B_data[b_i - 1];
      }
      wj *= (float)tau_data[pvt];
      if (wj != 0.0F) {
        B_data[pvt] -= wj;
        for (b_i = i; b_i <= m; b_i++) {
          B_data[b_i - 1] -= (float)V->data[(b_i + V->size[0] * pvt) - 1] * wj;
        }
      }
    }
  }
  for (b_i = 0; b_i < knt; b_i++) {
    p[jpvt[b_i] - 1] = B_data[b_i];
  }
  for (pvt = knt; pvt >= 1; pvt--) {
    minmana = jpvt[pvt - 1] - 1;
    p[minmana] /= (float)V->data[(pvt + V->size[0] * (pvt - 1)) - 1];
    for (b_i = 0; b_i <= pvt - 2; b_i++) {
      p[jpvt[0] - 1] -= p[minmana] * (float)V->data[V->size[0] * (pvt - 1)];
    }
  }
  emxFree_real_T(&V);
}

/*
 * File trailer for polyfit.c
 *
 * [EOF]
 */

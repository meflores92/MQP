/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: unique.c
 *
 * MATLAB Coder version            : 5.2
 * C/C++ source code generated on  : 20-Feb-2022 08:55:56
 */

/* Include Files */
#include "unique.h"
#include "operation_emxutil.h"
#include "operation_types.h"
#include "rt_nonfinite.h"
#include "sortIdx.h"
#include "sortrows.h"
#include "rt_nonfinite.h"
#include <math.h>
#include <string.h>

/* Function Definitions */
/*
 * Arguments    : const double a_data[]
 *                const int a_size[2]
 *                double b_data[]
 *                int b_size[2]
 *                int ndx_data[]
 *                int *ndx_size
 *                int pos_data[]
 *                int *pos_size
 * Return Type  : void
 */
void b_unique_rows(const double a_data[], const int a_size[2], double b_data[],
                   int b_size[2], int ndx_data[], int *ndx_size, int pos_data[],
                   int *pos_size)
{
  static double b_b_data[3024];
  double c_b_data[742];
  double idx_data[216];
  double b_idx_data[53];
  double a;
  double absx;
  double b;
  int tmp_data[216];
  int invr_data[53];
  int r_data[53];
  int b_b_size[2];
  int exitg1;
  int exponent;
  int i;
  int idx_size;
  int k;
  int k0;
  int khi;
  int loop_ub;
  int nb;
  boolean_T exitg2;
  boolean_T p;
  *pos_size = a_size[0];
  if (a_size[0] == 0) {
    b_size[0] = 0;
    b_size[1] = a_size[1];
    *ndx_size = 0;
  } else {
    b_b_size[0] = a_size[0];
    b_b_size[1] = a_size[1];
    loop_ub = a_size[0] * a_size[1];
    if (0 <= loop_ub - 1) {
      memcpy(&b_b_data[0], &a_data[0], loop_ub * sizeof(double));
    }
    sortrows(b_b_data, b_b_size, idx_data, &idx_size);
    b_size[0] = b_b_size[0];
    b_size[1] = b_b_size[1];
    loop_ub = b_b_size[0] * b_b_size[1];
    if (0 <= loop_ub - 1) {
      memcpy(&b_data[0], &b_b_data[0], loop_ub * sizeof(double));
    }
    if (0 <= idx_size - 1) {
      memcpy(&b_idx_data[0], &idx_data[0], idx_size * sizeof(double));
    }
    nb = -1;
    khi = a_size[0];
    k = 1;
    while (k <= khi) {
      k0 = k - 1;
      do {
        exitg1 = 0;
        k++;
        if (k > khi) {
          exitg1 = 1;
        } else {
          p = false;
          loop_ub = 0;
          exitg2 = false;
          while ((!exitg2) && (loop_ub <= b_size[1] - 1)) {
            idx_size = b_size[0] * loop_ub;
            a = b_data[k0 + idx_size];
            b = b_data[(k + idx_size) - 1];
            absx = fabs(b / 2.0);
            if ((!rtIsInf(absx)) && (!rtIsNaN(absx))) {
              if (absx <= 2.2250738585072014E-308) {
                absx = 4.94065645841247E-324;
              } else {
                frexp(absx, &exponent);
                absx = ldexp(1.0, exponent - 53);
              }
            } else {
              absx = rtNaN;
            }
            if ((fabs(b - a) < absx) ||
                (rtIsInf(a) && rtIsInf(b) && ((a > 0.0) == (b > 0.0)))) {
              loop_ub++;
            } else {
              p = true;
              exitg2 = true;
            }
          }
          if (p) {
            exitg1 = 1;
          }
        }
      } while (exitg1 == 0);
      nb++;
      i = b_size[1];
      for (loop_ub = 0; loop_ub < i; loop_ub++) {
        idx_size = b_size[0] * loop_ub;
        b_data[nb + idx_size] = b_data[k0 + idx_size];
      }
      i = k - 1;
      for (loop_ub = k0 + 1; loop_ub <= i; loop_ub++) {
        pos_data[(int)b_idx_data[loop_ub - 1] - 1] = nb + 1;
      }
      b_idx_data[nb] = b_idx_data[k0];
    }
    if (1 > nb + 1) {
      loop_ub = 0;
    } else {
      loop_ub = nb + 1;
    }
    khi = b_b_size[1] - 1;
    for (i = 0; i <= khi; i++) {
      for (k0 = 0; k0 < loop_ub; k0++) {
        b_data[k0 + loop_ub * i] = b_data[k0 + b_size[0] * i];
      }
    }
    *ndx_size = nb + 1;
    for (k = 0; k <= nb; k++) {
      ndx_data[k] = (int)b_idx_data[k];
    }
    sortIdx(ndx_data, nb + 1, tmp_data, &idx_size);
    if (0 <= idx_size - 1) {
      memcpy(&r_data[0], &tmp_data[0], idx_size * sizeof(int));
    }
    for (i = 0; i <= khi; i++) {
      for (k0 = 0; k0 < idx_size; k0++) {
        c_b_data[k0 + idx_size * i] = b_data[(r_data[k0] + loop_ub * i) - 1];
      }
    }
    b_size[0] = idx_size;
    b_size[1] = b_b_size[1];
    loop_ub = idx_size * b_b_size[1];
    if (0 <= loop_ub - 1) {
      memcpy(&b_data[0], &c_b_data[0], loop_ub * sizeof(double));
    }
    for (k = 0; k <= nb; k++) {
      i = r_data[k];
      ndx_data[k] = (int)b_idx_data[i - 1];
      invr_data[i - 1] = k + 1;
    }
    for (i = 0; i < *pos_size; i++) {
      pos_data[i] = invr_data[pos_data[i] - 1];
    }
  }
}

/*
 * Arguments    : const emxArray_real_T *b
 *                int nb
 *                int *nMInf
 *                int *nFinite
 *                int *nPInf
 *                int *nNaN
 * Return Type  : void
 */
void count_nonfinites(const emxArray_real_T *b, int nb, int *nMInf,
                      int *nFinite, int *nPInf, int *nNaN)
{
  double d;
  int k;
  boolean_T exitg1;
  k = 0;
  while ((k + 1 <= nb) && rtIsInf(b->data[k]) && (b->data[k] < 0.0)) {
    k++;
  }
  *nMInf = k;
  k = nb;
  while ((k >= 1) && rtIsNaN(b->data[k - 1])) {
    k--;
  }
  *nNaN = nb - k;
  exitg1 = false;
  while ((!exitg1) && (k >= 1)) {
    d = b->data[k - 1];
    if (rtIsInf(d) && (d > 0.0)) {
      k--;
    } else {
      exitg1 = true;
    }
  }
  *nPInf = (nb - k) - *nNaN;
  *nFinite = k - *nMInf;
}

/*
 * Arguments    : const double a_data[]
 *                double b_data[]
 *                int b_size[2]
 * Return Type  : void
 */
void unique_rows(const double a_data[], double b_data[], int b_size[2])
{
  double b_b_data[432];
  double ycol_data[216];
  double absx;
  double absx_tmp;
  double d;
  int idx_data[216];
  int iwork_data[216];
  int b_k;
  int b_p;
  int exitg2;
  int exponent;
  int i;
  int idx1_tmp_tmp;
  int idx2_tmp_tmp;
  int j;
  int k;
  int k0;
  int kEnd;
  int nb;
  int pEnd;
  int q;
  boolean_T exitg1;
  boolean_T p;
  memcpy(&b_b_data[0], &a_data[0], 432U * sizeof(double));
  for (k = 0; k <= 214; k += 2) {
    p = true;
    b_k = 0;
    exitg1 = false;
    while ((!exitg1) && (b_k < 2)) {
      q = k + 216 * b_k;
      d = a_data[q + 1];
      absx = a_data[q];
      if (absx == d) {
        b_k++;
      } else {
        if ((!(absx <= d)) && (!rtIsNaN(d))) {
          p = false;
        }
        exitg1 = true;
      }
    }
    if (p) {
      idx_data[k] = k + 1;
      idx_data[k + 1] = k + 2;
    } else {
      idx_data[k] = k + 2;
      idx_data[k + 1] = k + 1;
    }
  }
  i = 2;
  while (i < 216) {
    k0 = i << 1;
    j = 1;
    for (pEnd = i + 1; pEnd < 217; pEnd = nb + i) {
      b_p = j;
      q = pEnd;
      nb = j + k0;
      if (nb > 217) {
        nb = 217;
      }
      k = 0;
      kEnd = nb - j;
      while (k + 1 <= kEnd) {
        idx1_tmp_tmp = idx_data[b_p - 1];
        idx2_tmp_tmp = idx_data[q - 1];
        p = true;
        b_k = 0;
        exitg1 = false;
        while ((!exitg1) && (b_k < 2)) {
          d = a_data[(idx1_tmp_tmp + 216 * b_k) - 1];
          absx = a_data[(idx2_tmp_tmp + 216 * b_k) - 1];
          if (d == absx) {
            b_k++;
          } else {
            if ((!(d <= absx)) && (!rtIsNaN(absx))) {
              p = false;
            }
            exitg1 = true;
          }
        }
        if (p) {
          iwork_data[k] = idx1_tmp_tmp;
          b_p++;
          if (b_p == pEnd) {
            while (q < nb) {
              k++;
              iwork_data[k] = idx_data[q - 1];
              q++;
            }
          }
        } else {
          iwork_data[k] = idx2_tmp_tmp;
          q++;
          if (q == nb) {
            while (b_p < pEnd) {
              k++;
              iwork_data[k] = idx_data[b_p - 1];
              b_p++;
            }
          }
        }
        k++;
      }
      for (k = 0; k < kEnd; k++) {
        idx_data[(j + k) - 1] = iwork_data[k];
      }
      j = nb;
    }
    i = k0;
  }
  for (j = 0; j < 2; j++) {
    for (i = 0; i < 216; i++) {
      ycol_data[i] = b_b_data[(idx_data[i] + 216 * j) - 1];
    }
    memcpy(&b_b_data[j * 216], &ycol_data[0], 216U * sizeof(double));
  }
  for (q = 0; q < 216; q++) {
    ycol_data[q] = idx_data[q];
  }
  memcpy(&b_data[0], &b_b_data[0], 432U * sizeof(double));
  nb = -1;
  k = 0;
  while (k + 1 <= 216) {
    k0 = k;
    do {
      exitg2 = 0;
      k++;
      if (k + 1 > 216) {
        exitg2 = 1;
      } else {
        p = false;
        j = 0;
        exitg1 = false;
        while ((!exitg1) && (j < 2)) {
          absx_tmp = b_data[k + 216 * j];
          absx = fabs(absx_tmp / 2.0);
          if ((!rtIsInf(absx)) && (!rtIsNaN(absx))) {
            if (absx <= 2.2250738585072014E-308) {
              absx = 4.94065645841247E-324;
            } else {
              frexp(absx, &exponent);
              absx = ldexp(1.0, exponent - 53);
            }
          } else {
            absx = rtNaN;
          }
          d = b_data[k0 + 216 * j];
          if ((fabs(absx_tmp - d) < absx) ||
              (rtIsInf(d) && rtIsInf(absx_tmp) &&
               ((d > 0.0) == (absx_tmp > 0.0)))) {
            j++;
          } else {
            p = true;
            exitg1 = true;
          }
        }
        if (p) {
          exitg2 = 1;
        }
      }
    } while (exitg2 == 0);
    nb++;
    b_data[nb] = b_data[k0];
    b_data[nb + 216] = b_data[k0 + 216];
    ycol_data[nb] = ycol_data[k0];
  }
  if (1 > nb + 1) {
    pEnd = 0;
  } else {
    pEnd = nb + 1;
  }
  for (q = 0; q < 2; q++) {
    for (b_p = 0; b_p < pEnd; b_p++) {
      b_data[b_p + pEnd * q] = b_data[b_p + 216 * q];
    }
  }
  for (k = 0; k <= nb; k++) {
    idx_data[k] = (int)ycol_data[k];
  }
  sortIdx(idx_data, nb + 1, iwork_data, &k0);
  for (q = 0; q < 2; q++) {
    for (b_p = 0; b_p < k0; b_p++) {
      b_b_data[b_p + k0 * q] = b_data[(iwork_data[b_p] + pEnd * q) - 1];
    }
  }
  b_size[0] = k0;
  b_size[1] = 2;
  pEnd = k0 * 2;
  if (0 <= pEnd - 1) {
    memcpy(&b_data[0], &b_b_data[0], pEnd * sizeof(double));
  }
}

/*
 * Arguments    : const emxArray_real_T *a
 *                emxArray_real_T *b
 * Return Type  : void
 */
void unique_vector(const emxArray_real_T *a, emxArray_real_T *b)
{
  emxArray_int32_T *idx;
  emxArray_int32_T *iwork;
  double absx;
  double x;
  int b_i;
  int exitg1;
  int exponent;
  int i;
  int i2;
  int j;
  int k;
  int kEnd;
  int n;
  int na;
  int p;
  int pEnd;
  int q;
  int qEnd;
  emxInit_int32_T(&idx, 1);
  na = a->size[0];
  n = a->size[0] + 1;
  i = idx->size[0];
  idx->size[0] = a->size[0];
  emxEnsureCapacity_int32_T(idx, i);
  b_i = a->size[0];
  for (i = 0; i < b_i; i++) {
    idx->data[i] = 0;
  }
  if (a->size[0] != 0) {
    emxInit_int32_T(&iwork, 1);
    i = iwork->size[0];
    iwork->size[0] = a->size[0];
    emxEnsureCapacity_int32_T(iwork, i);
    i = a->size[0] - 1;
    for (k = 1; k <= i; k += 2) {
      if ((a->data[k - 1] <= a->data[k]) || rtIsNaN(a->data[k])) {
        idx->data[k - 1] = k;
        idx->data[k] = k + 1;
      } else {
        idx->data[k - 1] = k + 1;
        idx->data[k] = k;
      }
    }
    if ((a->size[0] & 1) != 0) {
      idx->data[a->size[0] - 1] = a->size[0];
    }
    b_i = 2;
    while (b_i < n - 1) {
      i2 = b_i << 1;
      j = 1;
      for (pEnd = b_i + 1; pEnd < n; pEnd = qEnd + b_i) {
        p = j;
        q = pEnd - 1;
        qEnd = j + i2;
        if (qEnd > n) {
          qEnd = n;
        }
        k = 0;
        kEnd = qEnd - j;
        while (k + 1 <= kEnd) {
          absx = a->data[idx->data[q] - 1];
          i = idx->data[p - 1];
          if ((a->data[i - 1] <= absx) || rtIsNaN(absx)) {
            iwork->data[k] = i;
            p++;
            if (p == pEnd) {
              while (q + 1 < qEnd) {
                k++;
                iwork->data[k] = idx->data[q];
                q++;
              }
            }
          } else {
            iwork->data[k] = idx->data[q];
            q++;
            if (q + 1 == qEnd) {
              while (p < pEnd) {
                k++;
                iwork->data[k] = idx->data[p - 1];
                p++;
              }
            }
          }
          k++;
        }
        for (k = 0; k < kEnd; k++) {
          idx->data[(j + k) - 1] = iwork->data[k];
        }
        j = qEnd;
      }
      b_i = i2;
    }
    emxFree_int32_T(&iwork);
  }
  i = b->size[0];
  b->size[0] = a->size[0];
  emxEnsureCapacity_real_T(b, i);
  for (k = 0; k < na; k++) {
    b->data[k] = a->data[idx->data[k] - 1];
  }
  emxFree_int32_T(&idx);
  count_nonfinites(b, a->size[0], &pEnd, &b_i, &i2, &p);
  q = -1;
  if (pEnd > 0) {
    q = 0;
  }
  b_i += pEnd;
  while (pEnd + 1 <= b_i) {
    x = b->data[pEnd];
    do {
      exitg1 = 0;
      pEnd++;
      if (pEnd + 1 > b_i) {
        exitg1 = 1;
      } else {
        absx = fabs(x / 2.0);
        if ((!rtIsInf(absx)) && (!rtIsNaN(absx))) {
          if (absx <= 2.2250738585072014E-308) {
            absx = 4.94065645841247E-324;
          } else {
            frexp(absx, &exponent);
            absx = ldexp(1.0, exponent - 53);
          }
        } else {
          absx = rtNaN;
        }
        if ((!(fabs(x - b->data[pEnd]) < absx)) &&
            ((!rtIsInf(b->data[pEnd])) || (!rtIsInf(x)) ||
             ((b->data[pEnd] > 0.0) != (x > 0.0)))) {
          exitg1 = 1;
        }
      }
    } while (exitg1 == 0);
    q++;
    b->data[q] = x;
  }
  if (i2 > 0) {
    q++;
    b->data[q] = b->data[b_i];
  }
  pEnd = b_i + i2;
  for (j = 0; j < p; j++) {
    b->data[(q + j) + 1] = b->data[pEnd + j];
  }
  q += p;
  i = b->size[0];
  if (1 > q + 1) {
    b->size[0] = 0;
  } else {
    b->size[0] = q + 1;
  }
  emxEnsureCapacity_real_T(b, i);
}

/*
 * File trailer for unique.c
 *
 * [EOF]
 */

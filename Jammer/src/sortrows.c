/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: sortrows.c
 *
 * MATLAB Coder version            : 5.2
 * C/C++ source code generated on  : 20-Feb-2022 08:55:56
 */

/* Include Files */
#include "sortrows.h"
#include "rt_nonfinite.h"
#include "sortLE.h"
#include <string.h>

/* Function Definitions */
/*
 * Arguments    : double y_data[]
 *                const int y_size[2]
 *                double ndx_data[]
 *                int *ndx_size
 * Return Type  : void
 */
void sortrows(double y_data[], const int y_size[2], double ndx_data[],
              int *ndx_size)
{
  double ycol_data[216];
  int idx_data[216];
  int iwork_data[216];
  int col_data[14];
  int col_size[2];
  int b_i;
  int i;
  int i1;
  int i2;
  int j;
  int k;
  int kEnd;
  int n;
  int p;
  int pEnd;
  int q;
  int qEnd;
  n = y_size[1];
  col_size[0] = 1;
  col_size[1] = y_size[1];
  for (k = 0; k < n; k++) {
    col_data[k] = k + 1;
  }
  n = y_size[0] + 1;
  *ndx_size = y_size[0];
  i2 = y_size[0];
  if (0 <= i2 - 1) {
    memset(&idx_data[0], 0, i2 * sizeof(int));
  }
  if ((y_size[0] == 0) || (y_size[1] == 0)) {
    for (k = 0; k <= n - 2; k++) {
      idx_data[k] = k + 1;
    }
  } else {
    i = y_size[0] - 1;
    for (k = 1; k <= i; k += 2) {
      if (sortLE(y_data, y_size, col_data, col_size, k, k + 1)) {
        idx_data[k - 1] = k;
        idx_data[k] = k + 1;
      } else {
        idx_data[k - 1] = k + 1;
        idx_data[k] = k;
      }
    }
    if ((y_size[0] & 1) != 0) {
      idx_data[y_size[0] - 1] = y_size[0];
    }
    b_i = 2;
    while (b_i < n - 1) {
      i2 = b_i << 1;
      j = 1;
      for (pEnd = b_i + 1; pEnd < n; pEnd = qEnd + b_i) {
        p = j;
        q = pEnd;
        qEnd = j + i2;
        if (qEnd > n) {
          qEnd = n;
        }
        k = 0;
        kEnd = qEnd - j;
        while (k + 1 <= kEnd) {
          i = idx_data[q - 1];
          i1 = idx_data[p - 1];
          if (sortLE(y_data, y_size, col_data, col_size, i1, i)) {
            iwork_data[k] = i1;
            p++;
            if (p == pEnd) {
              while (q < qEnd) {
                k++;
                iwork_data[k] = idx_data[q - 1];
                q++;
              }
            }
          } else {
            iwork_data[k] = i;
            q++;
            if (q == qEnd) {
              while (p < pEnd) {
                k++;
                iwork_data[k] = idx_data[p - 1];
                p++;
              }
            }
          }
          k++;
        }
        for (k = 0; k < kEnd; k++) {
          idx_data[(j + k) - 1] = iwork_data[k];
        }
        j = qEnd;
      }
      b_i = i2;
    }
  }
  i2 = y_size[0] - 1;
  n = y_size[1];
  for (j = 0; j < n; j++) {
    for (b_i = 0; b_i <= i2; b_i++) {
      ycol_data[b_i] = y_data[(idx_data[b_i] + y_size[0] * j) - 1];
    }
    for (b_i = 0; b_i <= i2; b_i++) {
      y_data[b_i + y_size[0] * j] = ycol_data[b_i];
    }
  }
  for (i = 0; i < *ndx_size; i++) {
    ndx_data[i] = idx_data[i];
  }
}

/*
 * File trailer for sortrows.c
 *
 * [EOF]
 */

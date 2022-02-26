/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: sortIdx.c
 *
 * MATLAB Coder version            : 5.2
 * C/C++ source code generated on  : 20-Feb-2022 08:55:56
 */

/* Include Files */
#include "sortIdx.h"
#include "rt_nonfinite.h"
#include <string.h>

/* Function Definitions */
/*
 * Arguments    : const int x_data[]
 *                int x_size
 *                int idx_data[]
 *                int *idx_size
 * Return Type  : void
 */
void sortIdx(const int x_data[], int x_size, int idx_data[], int *idx_size)
{
  int iwork_data[216];
  int b_i;
  int i;
  int i2;
  int idx_size_tmp;
  int j;
  int k;
  int kEnd;
  int p;
  int pEnd;
  int q;
  int qEnd;
  idx_size_tmp = (unsigned char)x_size;
  *idx_size = (unsigned char)x_size;
  if (0 <= idx_size_tmp - 1) {
    memset(&idx_data[0], 0, idx_size_tmp * sizeof(int));
  }
  if (x_size != 0) {
    idx_size_tmp = x_size - 1;
    for (k = 1; k <= idx_size_tmp; k += 2) {
      if (x_data[k - 1] <= x_data[k]) {
        idx_data[k - 1] = k;
        idx_data[k] = k + 1;
      } else {
        idx_data[k - 1] = k + 1;
        idx_data[k] = k;
      }
    }
    if ((x_size & 1) != 0) {
      idx_data[x_size - 1] = x_size;
    }
    i = 2;
    while (i < x_size) {
      i2 = i << 1;
      j = 1;
      for (pEnd = i + 1; pEnd < x_size + 1; pEnd = qEnd + i) {
        p = j;
        q = pEnd;
        qEnd = j + i2;
        if (qEnd > x_size + 1) {
          qEnd = x_size + 1;
        }
        k = 0;
        kEnd = qEnd - j;
        while (k + 1 <= kEnd) {
          idx_size_tmp = idx_data[q - 1];
          b_i = idx_data[p - 1];
          if (x_data[b_i - 1] <= x_data[idx_size_tmp - 1]) {
            iwork_data[k] = b_i;
            p++;
            if (p == pEnd) {
              while (q < qEnd) {
                k++;
                iwork_data[k] = idx_data[q - 1];
                q++;
              }
            }
          } else {
            iwork_data[k] = idx_size_tmp;
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
      i = i2;
    }
  }
}

/*
 * File trailer for sortIdx.c
 *
 * [EOF]
 */

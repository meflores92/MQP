/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: OFDMModulate.c
 *
 * MATLAB Coder version            : 5.2
 * C/C++ source code generated on  : 19-Jan-2022 16:08:45
 */

/* Include Files */
#include "OFDMModulate.h"
#include "eml_setop.h"
#include "modulate.h"
#include "operation_data.h"
#include <math.h>

/* Function Definitions */
/*
 * Arguments    : const double grid[480]
 *                creal_T waveform_data[]
 *                int waveform_size[2]
 * Return Type  : void
 */
void OFDMModulate(const double grid[480], creal_T waveform_data[],
                  int waveform_size[2])
{
  static creal_T fullGrid_data[3072];
  creal_T gridc[480];
  double c_data[1536];
  double prmStr_CyclicPrefixLength[2];
  int ia_data[1536];
  int idx[1296];
  int iwork[1296];
  int b_i;
  int b_k;
  int c_k;
  int exitg1;
  int exponent;
  int i;
  int i2;
  int j;
  int k;
  int kEnd;
  int nb;
  int p;
  int pEnd;
  int q;
  int qEnd;
  int x;
  short b_data[1296];
#pragma omp parallel for num_threads(omp_get_max_threads()) private(c_k, k, b_i)

  for (k = 0; k < 2; k++) {
    for (c_k = 0; c_k < 240; c_k++) {
      b_i = c_k + 240 * k;
      gridc[b_i].re = grid[b_i];
      gridc[b_i].im = 0.0;
    }
    prmStr_CyclicPrefixLength[k] = -24.0 * (double)k + 132.0;
  }
  for (b_k = 0; b_k <= 1294; b_k += 2) {
    if (iv1[b_k] <= iv1[b_k + 1]) {
      idx[b_k] = b_k + 1;
      idx[b_k + 1] = b_k + 2;
    } else {
      idx[b_k] = b_k + 2;
      idx[b_k + 1] = b_k + 1;
    }
  }
  i = 2;
  while (i < 1296) {
    i2 = i << 1;
    j = 1;
    for (pEnd = i + 1; pEnd < 1297; pEnd = qEnd + i) {
      p = j;
      q = pEnd;
      qEnd = j + i2;
      if (qEnd > 1297) {
        qEnd = 1297;
      }
      b_k = 0;
      kEnd = qEnd - j;
      while (b_k + 1 <= kEnd) {
        nb = idx[q - 1];
        x = idx[p - 1];
        if (iv1[x - 1] <= iv1[nb - 1]) {
          iwork[b_k] = x;
          p++;
          if (p == pEnd) {
            while (q < qEnd) {
              b_k++;
              iwork[b_k] = idx[q - 1];
              q++;
            }
          }
        } else {
          iwork[b_k] = nb;
          q++;
          if (q == qEnd) {
            while (p < pEnd) {
              b_k++;
              iwork[b_k] = idx[p - 1];
              p++;
            }
          }
        }
        b_k++;
      }
      for (b_k = 0; b_k < kEnd; b_k++) {
        idx[(j + b_k) - 1] = iwork[b_k];
      }
      j = qEnd;
    }
    i = i2;
  }
#pragma omp parallel for num_threads(omp_get_max_threads()) private(k)

  for (k = 0; k < 1296; k++) {
    b_data[k] = iv1[idx[k] - 1];
  }
  nb = -1;
  b_k = 1;
  while (b_k <= 1296) {
    x = b_data[b_k - 1];
    do {
      exitg1 = 0;
      b_k++;
      if (b_k > 1296) {
        exitg1 = 1;
      } else {
        frexp((double)x / 2.0, &exponent);
        if (fabs((double)(x - b_data[b_k - 1])) >= ldexp(1.0, exponent - 53)) {
          exitg1 = 1;
        }
      }
    } while (exitg1 == 0);
    nb++;
    b_data[nb] = (short)x;
  }
  do_vectors(c_data, &nb, ia_data, &x, &i);
#pragma omp parallel for num_threads(omp_get_max_threads()) private(b_i)

  for (b_i = 0; b_i < 3072; b_i++) {
    fullGrid_data[b_i].re = 0.0;
    fullGrid_data[b_i].im = 0.0;
  }
  for (nb = 0; nb < 2; nb++) {
    for (x = 0; x < 240; x++) {
      fullGrid_data[((int)c_data[x] + 1536 * nb) - 1] = gridc[x + 240 * nb];
    }
  }
  modulate(fullGrid_data, prmStr_CyclicPrefixLength, waveform_data,
           waveform_size);
}

/*
 * File trailer for OFDMModulate.c
 *
 * [EOF]
 */

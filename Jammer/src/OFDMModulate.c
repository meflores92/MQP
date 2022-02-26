/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: OFDMModulate.c
 *
 * MATLAB Coder version            : 5.2
 * C/C++ source code generated on  : 20-Feb-2022 08:55:56
 */

/* Include Files */
#include "OFDMModulate.h"
#include "FFTImplementationCallback.h"
#include "eml_setop.h"
#include "operation_data.h"
#include "operation_emxutil.h"
#include "operation_types.h"
#include "rt_nonfinite.h"
#include "unique.h"
#include <math.h>
#include <string.h>

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
  static creal_T postIFFT_data[3072];
  emxArray_real_T b_data;
  emxArray_real_T *y;
  creal_T gridc[480];
  creal_T tmp_data[132];
  double costab_data[3073];
  double sintab_data[3073];
  double sintabinv_data[3073];
  double c_data[1536];
  double b_b_data[1296];
  double tmp_im;
  double tmp_re;
  int ia_data[1536];
  int idx[1296];
  int iwork[1296];
  int postIFFT_size[3];
  int costab_size[2];
  int sintab_size[2];
  int sintabinv_size[2];
  int b_i;
  int b_size;
  int exitg1;
  int exponent;
  int i;
  int i1;
  int i2;
  int k;
  int kEnd;
  int khi;
  int nb;
  int q;
  int qEnd;
  int x;
  short b_tmp_data[1668];
  for (k = 0; k < 2; k++) {
    for (khi = 0; khi < 240; khi++) {
      b_i = khi + 240 * k;
      tmp_re = grid[b_i];
      gridc[b_i].re = tmp_re;
      gridc[b_i].im = tmp_re * 0.0;
    }
  }
  for (k = 0; k <= 1294; k += 2) {
    if (iv7[k] <= iv7[k + 1]) {
      idx[k] = k + 1;
      idx[k + 1] = k + 2;
    } else {
      idx[k] = k + 2;
      idx[k + 1] = k + 1;
    }
  }
  i = 2;
  while (i < 1296) {
    i2 = i << 1;
    khi = 1;
    for (x = i + 1; x < 1297; x = qEnd + i) {
      nb = khi;
      q = x;
      qEnd = khi + i2;
      if (qEnd > 1297) {
        qEnd = 1297;
      }
      k = 0;
      kEnd = qEnd - khi;
      while (k + 1 <= kEnd) {
        b_i = idx[q - 1];
        i1 = idx[nb - 1];
        if (iv7[i1 - 1] <= iv7[b_i - 1]) {
          iwork[k] = i1;
          nb++;
          if (nb == x) {
            while (q < qEnd) {
              k++;
              iwork[k] = idx[q - 1];
              q++;
            }
          }
        } else {
          iwork[k] = b_i;
          q++;
          if (q == qEnd) {
            while (nb < x) {
              k++;
              iwork[k] = idx[nb - 1];
              nb++;
            }
          }
        }
        k++;
      }
      for (k = 0; k < kEnd; k++) {
        idx[(khi + k) - 1] = iwork[k];
      }
      khi = qEnd;
    }
    i = i2;
  }
  b_size = 1296;
  for (k = 0; k < 1296; k++) {
    b_b_data[k] = iv7[idx[k] - 1];
  }
  b_data.data = &b_b_data[0];
  b_data.size = &b_size;
  b_data.allocatedSize = 1296;
  b_data.numDimensions = 1;
  b_data.canFreeData = false;
  count_nonfinites(&b_data, 1296, &q, &khi, &nb, &x);
  nb = -1;
  if (q > 0) {
    nb = 0;
  }
  khi += q;
  while (q + 1 <= khi) {
    x = (int)b_b_data[q];
    do {
      exitg1 = 0;
      q++;
      if (q + 1 > khi) {
        exitg1 = 1;
      } else {
        frexp((double)x / 2.0, &exponent);
        if (!(fabs((double)x - b_b_data[q]) < ldexp(1.0, exponent - 53))) {
          exitg1 = 1;
        }
      }
    } while (exitg1 == 0);
    nb++;
    b_b_data[nb] = x;
  }
  b_do_vectors(c_data, &khi, ia_data, &nb, &x);
  memset(&fullGrid_data[0], 0, 3072U * sizeof(creal_T));
  for (b_i = 0; b_i < 2; b_i++) {
    for (i1 = 0; i1 < 240; i1++) {
      fullGrid_data[((int)c_data[i1] + 1536 * b_i) - 1] = gridc[i1 + 240 * b_i];
    }
  }
  i2 = 0;
  for (i = 0; i < 2; i++) {
    nb = i2 - 1;
    i2 += 1535;
    nb++;
    i2++;
    for (k = 0; k < 768; k++) {
      x = nb + k;
      tmp_re = fullGrid_data[x].re;
      tmp_im = fullGrid_data[x].im;
      fullGrid_data[x] = fullGrid_data[x + 768];
      fullGrid_data[x + 768].re = tmp_re;
      fullGrid_data[x + 768].im = tmp_im;
    }
  }
  c_FFTImplementationCallback_get(1536, false, &khi, &nb);
  e_FFTImplementationCallback_gen(nb, costab_data, costab_size, sintab_data,
                                  sintab_size, sintabinv_data, sintabinv_size);
  e_FFTImplementationCallback_dob(fullGrid_data, khi, costab_data, costab_size,
                                  sintab_data, sintab_size, sintabinv_data,
                                  sintabinv_size, postIFFT_data, postIFFT_size);
  waveform_size[0] = 3312;
  waveform_size[1] = 1;
  emxInit_real_T(&y, 2);
  for (q = 0; q < 2; q++) {
    nb = -24 * q + 132;
    b_i = y->size[0] * y->size[1];
    y->size[0] = 1;
    y->size[1] = nb;
    emxEnsureCapacity_real_T(y, b_i);
    x = nb - 1;
    for (b_i = 0; b_i <= x; b_i++) {
      y->data[b_i] = (double)b_i + 1.0;
    }
    x = y->size[1];
    for (b_i = 0; b_i < x; b_i++) {
      tmp_data[b_i] = postIFFT_data
          [((int)((1536.0 - (double)nb) + y->data[b_i]) + 1536 * q) - 1];
    }
    b_i = (1 <= q);
    i1 = y->size[0] * y->size[1];
    y->size[0] = 1;
    y->size[1] = nb + 1536;
    emxEnsureCapacity_real_T(y, i1);
    x = nb + 1535;
    for (i1 = 0; i1 <= x; i1++) {
      y->data[i1] = (double)i1 + 1.0;
    }
    if (b_i == 0) {
      khi = 0;
    } else {
      khi = 132;
    }
    b_i = 1536 * q + khi;
    x = y->size[1];
    for (i1 = 0; i1 < x; i1++) {
      b_tmp_data[i1] = (short)((short)((double)b_i + y->data[i1]) - 1);
    }
    for (b_i = 0; b_i < nb; b_i++) {
      waveform_data[b_tmp_data[b_i]] = tmp_data[b_i];
    }
    for (b_i = 0; b_i < 1536; b_i++) {
      waveform_data[b_tmp_data[b_i + nb]] = postIFFT_data[b_i + 1536 * q];
    }
  }
  emxFree_real_T(&y);
}

/*
 * File trailer for OFDMModulate.c
 *
 * [EOF]
 */

/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: modulate.c
 *
 * MATLAB Coder version            : 5.2
 * C/C++ source code generated on  : 19-Jan-2022 16:08:45
 */

/* Include Files */
#include "modulate.h"
#include "FFTImplementationCallback.h"
#include "operation_emxutil.h"
#include "operation_types.h"
#include <math.h>

/* Function Definitions */
/*
 * Arguments    : const creal_T gridIn_data[]
 *                const double prmStr_CyclicPrefixLength[2]
 *                creal_T yout_data[]
 *                int yout_size[2]
 * Return Type  : void
 */
void modulate(const creal_T gridIn_data[],
              const double prmStr_CyclicPrefixLength[2], creal_T yout_data[],
              int yout_size[2])
{
  static creal_T postIFFT_data[3072];
  static creal_T postShift_data[3072];
  emxArray_real_T *costab1q;
  creal_T tmp_data[132];
  double costab_data[3073];
  double sintab_data[3073];
  double sintabinv_data[3073];
  double tmp_im;
  double tmp_re;
  int postIFFT_size[3];
  int costab_size[2];
  int sintab_size[2];
  int b_k;
  int i;
  int i1;
  int i2;
  int k;
  int tmp_re_tmp;
  short b_tmp_data[1668];
#pragma omp parallel for num_threads(omp_get_max_threads()) private(k)

  for (k = 0; k < 3072; k++) {
    postShift_data[k] = gridIn_data[k];
  }
  i2 = 0;
  for (i = 0; i < 2; i++) {
    i1 = i2 - 1;
    i2 += 1535;
    i1++;
    i2++;
    for (b_k = 0; b_k < 768; b_k++) {
      tmp_re_tmp = i1 + b_k;
      tmp_re = postShift_data[tmp_re_tmp].re;
      tmp_im = postShift_data[tmp_re_tmp].im;
      postShift_data[tmp_re_tmp] = postShift_data[tmp_re_tmp + 768];
      postShift_data[tmp_re_tmp + 768].re = tmp_re;
      postShift_data[tmp_re_tmp + 768].im = tmp_im;
    }
  }
  emxInit_real_T(&costab1q, 2);
  c_FFTImplementationCallback_get(1536, false, &i, &i2);
  tmp_re = 6.2831853071795862 / (double)i2;
  tmp_re_tmp = i2 / 2 / 2;
  b_k = costab1q->size[0] * costab1q->size[1];
  costab1q->size[0] = 1;
  costab1q->size[1] = tmp_re_tmp + 1;
  emxEnsureCapacity_real_T(costab1q, b_k);
  costab1q->data[0] = 1.0;
  i2 = tmp_re_tmp / 2 - 1;
  if (i2 + 1 < 400) {
    for (k = 0; k <= i2; k++) {
      costab1q->data[k + 1] = cos(tmp_re * ((double)k + 1.0));
    }
  } else {
#pragma omp parallel for num_threads(omp_get_max_threads()) private(k)

    for (k = 0; k <= i2; k++) {
      costab1q->data[k + 1] = cos(tmp_re * ((double)k + 1.0));
    }
  }
  b_k = i2 + 2;
  i1 = tmp_re_tmp - 1;
  if ((tmp_re_tmp - i2) - 2 < 400) {
    for (k = b_k; k <= i1; k++) {
      costab1q->data[k] = sin(tmp_re * (double)(tmp_re_tmp - k));
    }
  } else {
#pragma omp parallel for num_threads(omp_get_max_threads()) private(k)

    for (k = b_k; k <= i1; k++) {
      costab1q->data[k] = sin(tmp_re * (double)(tmp_re_tmp - k));
    }
  }
  costab1q->data[tmp_re_tmp] = 0.0;
  tmp_re_tmp = costab1q->size[1] - 1;
  i2 = (costab1q->size[1] - 1) << 1;
  costab_size[0] = 1;
  costab_size[1] = i2 + 1;
  sintab_size[0] = 1;
  sintab_size[1] = i2 + 1;
  costab_data[0] = 1.0;
  sintab_data[0] = 0.0;
  if (costab1q->size[1] - 1 < 400) {
    for (k = 0; k < tmp_re_tmp; k++) {
      sintabinv_data[k + 1] = costab1q->data[(tmp_re_tmp - k) - 1];
    }
  } else {
#pragma omp parallel for num_threads(omp_get_max_threads()) private(k)

    for (k = 0; k < tmp_re_tmp; k++) {
      sintabinv_data[k + 1] = costab1q->data[(tmp_re_tmp - k) - 1];
    }
  }
  b_k = costab1q->size[1];
  if ((i2 - costab1q->size[1]) + 1 < 400) {
    for (k = b_k; k <= i2; k++) {
      sintabinv_data[k] = costab1q->data[k - tmp_re_tmp];
    }
  } else {
#pragma omp parallel for num_threads(omp_get_max_threads()) private(k)

    for (k = b_k; k <= i2; k++) {
      sintabinv_data[k] = costab1q->data[k - tmp_re_tmp];
    }
  }
  if (tmp_re_tmp < 400) {
    for (k = 0; k < tmp_re_tmp; k++) {
      costab_data[k + 1] = costab1q->data[k + 1];
      sintab_data[k + 1] = -costab1q->data[(tmp_re_tmp - k) - 1];
    }
  } else {
#pragma omp parallel for num_threads(omp_get_max_threads()) private(k)

    for (k = 0; k < tmp_re_tmp; k++) {
      costab_data[k + 1] = costab1q->data[k + 1];
      sintab_data[k + 1] = -costab1q->data[(tmp_re_tmp - k) - 1];
    }
  }
  b_k = costab1q->size[1];
  if ((i2 - costab1q->size[1]) + 1 < 400) {
    for (k = b_k; k <= i2; k++) {
      costab_data[k] = -costab1q->data[i2 - k];
      sintab_data[k] = -costab1q->data[k - tmp_re_tmp];
    }
  } else {
#pragma omp parallel for num_threads(omp_get_max_threads()) private(k)

    for (k = b_k; k <= i2; k++) {
      costab_data[k] = -costab1q->data[i2 - k];
      sintab_data[k] = -costab1q->data[k - tmp_re_tmp];
    }
  }
  emxFree_real_T(&costab1q);
  c_FFTImplementationCallback_dob(postShift_data, i, costab_data, costab_size,
                                  sintab_data, sintab_size, sintabinv_data,
                                  postIFFT_data, postIFFT_size);
  yout_size[0] = 3312;
  yout_size[1] = 1;
  for (i1 = 0; i1 < 2; i1++) {
    i2 = (int)floor(prmStr_CyclicPrefixLength[i1] - 1.0);
    for (b_k = 0; b_k <= i2; b_k++) {
      tmp_data[b_k] =
          postIFFT_data[((int)((1536.0 - prmStr_CyclicPrefixLength[i1]) +
                               (double)(b_k + 1)) +
                         1536 * i1) -
                        1];
    }
    b_k = (1 <= i1);
    if (b_k == 0) {
      tmp_re = 0.0;
    } else {
      tmp_re = prmStr_CyclicPrefixLength[0];
    }
    tmp_re += 1536.0 * (((double)i1 + 1.0) - 1.0);
    i2 = (int)floor((prmStr_CyclicPrefixLength[i1] + 1536.0) - 1.0);
    for (b_k = 0; b_k <= i2; b_k++) {
      b_tmp_data[b_k] = (short)((short)(int)(tmp_re + ((double)b_k + 1.0)) - 1);
    }
    i2 = (int)prmStr_CyclicPrefixLength[i1];
    for (b_k = 0; b_k < i2; b_k++) {
      yout_data[b_tmp_data[b_k]] = tmp_data[b_k];
    }
    for (b_k = 0; b_k < 1536; b_k++) {
      yout_data[b_tmp_data[b_k + i2]] = postIFFT_data[b_k + 1536 * i1];
    }
  }
}

/*
 * File trailer for modulate.c
 *
 * [EOF]
 */

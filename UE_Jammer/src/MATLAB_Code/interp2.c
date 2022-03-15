/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: interp2.c
 *
 * MATLAB Coder version            : 5.2
 * C/C++ source code generated on  : 20-Feb-2022 08:55:56
 */

/* Include Files */
#include "interp2.h"
#include "bsearch.h"
#include "operation_emxutil.h"
#include "operation_types.h"
#include "rt_nonfinite.h"
#include <string.h>

/* Function Definitions */
/*
 * Arguments    : const emxArray_creal32_T *V
 *                const emxArray_real_T *Xq
 *                const emxArray_real_T *Yq
 *                const double X_data[]
 *                const int X_size[2]
 *                const emxArray_real_T *Y
 *                emxArray_creal32_T *Vq
 * Return Type  : void
 */
void interp2_local(const emxArray_creal32_T *V, const emxArray_real_T *Xq,
                   const emxArray_real_T *Yq, const double X_data[],
                   const int X_size[2], const emxArray_real_T *Y,
                   emxArray_creal32_T *Vq)
{
  emxArray_real_T b_X_data;
  double rx;
  float zx1y1_im;
  float zx1y1_re;
  float zx1y2_im;
  float zx1y2_re;
  int ix;
  int iy;
  int k;
  int ub_loop;
  ub_loop = Vq->size[0] * Vq->size[1];
  Vq->size[0] = Xq->size[0];
  Vq->size[1] = Xq->size[1];
  emxEnsureCapacity_creal32_T(Vq, ub_loop);
  ub_loop = Xq->size[0] * Xq->size[1] - 1;
#pragma omp parallel for num_threads(omp_get_max_threads()) private(           \
    zx1y2_re, b_X_data, ix, iy, zx1y1_re, zx1y1_im, zx1y2_im, rx)

  for (k = 0; k <= ub_loop; k++) {
    if ((Xq->data[k] >= X_data[0]) && (Xq->data[k] <= X_data[X_size[1] - 1]) &&
        (Yq->data[k] >= Y->data[0]) &&
        (Yq->data[k] <= Y->data[Y->size[1] - 1])) {
      b_X_data.data = (double *)&X_data[0];
      b_X_data.size = (int *)&X_size[0];
      b_X_data.allocatedSize = -1;
      b_X_data.numDimensions = 2;
      b_X_data.canFreeData = false;
      ix = b_bsearch(&b_X_data, Xq->data[k]);
      iy = b_bsearch(Y, Yq->data[k]);
      zx1y1_re = V->data[(iy + V->size[0] * (ix - 1)) - 1].re;
      zx1y1_im = V->data[(iy + V->size[0] * (ix - 1)) - 1].im;
      zx1y2_re = V->data[iy + V->size[0] * (ix - 1)].re;
      zx1y2_im = V->data[iy + V->size[0] * (ix - 1)].im;
      rx = X_data[ix - 1];
      if (!(Xq->data[k] == rx)) {
        if (Xq->data[k] == X_data[ix]) {
          zx1y1_re = V->data[(iy + V->size[0] * ix) - 1].re;
          zx1y1_im = V->data[(iy + V->size[0] * ix) - 1].im;
          zx1y2_re = V->data[iy + V->size[0] * ix].re;
          zx1y2_im = V->data[iy + V->size[0] * ix].im;
        } else {
          rx = (Xq->data[k] - rx) / (X_data[ix] - rx);
          if ((!(V->data[(iy + V->size[0] * (ix - 1)) - 1].re ==
                 V->data[(iy + V->size[0] * ix) - 1].re)) ||
              (!(V->data[(iy + V->size[0] * (ix - 1)) - 1].im ==
                 V->data[(iy + V->size[0] * ix) - 1].im))) {
            zx1y1_re = (float)(1.0 - rx) *
                           V->data[(iy + V->size[0] * (ix - 1)) - 1].re +
                       (float)rx * V->data[(iy + V->size[0] * ix) - 1].re;
            zx1y1_im = (float)(1.0 - rx) *
                           V->data[(iy + V->size[0] * (ix - 1)) - 1].im +
                       (float)rx * V->data[(iy + V->size[0] * ix) - 1].im;
          }
          if ((!(V->data[iy + V->size[0] * (ix - 1)].re ==
                 V->data[iy + V->size[0] * ix].re)) ||
              (!(V->data[iy + V->size[0] * (ix - 1)].im ==
                 V->data[iy + V->size[0] * ix].im))) {
            zx1y2_re =
                (float)(1.0 - rx) * V->data[iy + V->size[0] * (ix - 1)].re +
                (float)rx * V->data[iy + V->size[0] * ix].re;
            zx1y2_im =
                (float)(1.0 - rx) * V->data[iy + V->size[0] * (ix - 1)].im +
                (float)rx * V->data[iy + V->size[0] * ix].im;
          }
        }
      }
      rx = Y->data[iy - 1];
      if ((Yq->data[k] == rx) ||
          ((zx1y1_re == zx1y2_re) && (zx1y1_im == zx1y2_im))) {
        zx1y2_re = zx1y1_re;
        zx1y2_im = zx1y1_im;
      } else if (!(Yq->data[k] == Y->data[iy])) {
        rx = (Yq->data[k] - rx) / (Y->data[iy] - rx);
        zx1y2_re = (float)(1.0 - rx) * zx1y1_re + (float)rx * zx1y2_re;
        zx1y2_im = (float)(1.0 - rx) * zx1y1_im + (float)rx * zx1y2_im;
      }
    } else {
      zx1y2_re = rtNaNF;
      zx1y2_im = rtNaNF;
    }
    Vq->data[k].re = zx1y2_re;
    Vq->data[k].im = zx1y2_im;
  }
}

/*
 * File trailer for interp2.c
 *
 * [EOF]
 */

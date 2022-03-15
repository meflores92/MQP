/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: interp1.c
 *
 * MATLAB Coder version            : 5.2
 * C/C++ source code generated on  : 20-Feb-2022 08:55:56
 */

/* Include Files */
#include "interp1.h"
#include "bsearch.h"
#include "operation_data.h"
#include "operation_emxutil.h"
#include "operation_types.h"
#include "rt_nonfinite.h"
#include "rt_nonfinite.h"
#include <string.h>

/* Function Declarations */
static void interp1SplineOrPCHIP(const emxArray_real32_T *y,
                                 const emxArray_real_T *xi,
                                 emxArray_real32_T *yi,
                                 const emxArray_real_T *x);

/* Function Definitions */
/*
 * Arguments    : const emxArray_real32_T *y
 *                const emxArray_real_T *xi
 *                emxArray_real32_T *yi
 *                const emxArray_real_T *x
 * Return Type  : void
 */
static void interp1SplineOrPCHIP(const emxArray_real32_T *y,
                                 const emxArray_real_T *xi,
                                 emxArray_real32_T *yi,
                                 const emxArray_real_T *x)
{
  c_struct_T pp;
  emxArray_real32_T *dvdf;
  emxArray_real32_T *pp_coefs;
  emxArray_real32_T *s;
  emxArray_real_T *dx;
  emxArray_real_T *md;
  double d31;
  double dnnm2;
  double r;
  double xloc;
  float c_data[4];
  float pp1_coefs[4];
  float dzdxdx;
  float dzzdx;
  float f;
  float v;
  int i;
  int ic;
  int ip;
  int k;
  int numTerms;
  int nxm1;
  int szdvdf_idx_1;
  int szs_idx_1;
  int yoffset;
  boolean_T has_endslopes;
  has_endslopes = (y->size[0] == x->size[0] + 2);
  emxInitStruct_struct_T(&pp);
  emxInit_real32_T(&s, 2);
  emxInit_real32_T(&dvdf, 2);
  emxInit_real_T(&dx, 1);
  emxInit_real_T(&md, 1);
  emxInit_real32_T(&pp_coefs, 2);
  if ((x->size[0] <= 2) || ((x->size[0] <= 3) && (!has_endslopes))) {
    has_endslopes = (y->size[0] == x->size[0] + 2);
    if (x->size[0] <= 2) {
      if (has_endslopes) {
        szdvdf_idx_1 = 4;
      } else {
        szdvdf_idx_1 = 2;
      }
      if (has_endslopes) {
        if (0 <= x->size[0] - 2) {
          d31 = x->data[1] - x->data[0];
          dzdxdx = (y->data[2] - y->data[1]) / (float)d31;
          dzzdx = (dzdxdx - y->data[0]) / (float)d31;
          dzdxdx = (y->data[y->size[0] - 1] - dzdxdx) / (float)d31;
          pp1_coefs[0] = (dzdxdx - dzzdx) / (float)d31;
          pp1_coefs[1] = 2.0F * dzzdx - dzdxdx;
          pp1_coefs[2] = y->data[0];
          pp1_coefs[3] = y->data[1];
        }
        if (0 <= szdvdf_idx_1 - 1) {
          memcpy(&c_data[0], &pp1_coefs[0], szdvdf_idx_1 * sizeof(float));
        }
      } else {
        c_data[0] =
            (y->data[1] - y->data[0]) / (float)(x->data[1] - x->data[0]);
        c_data[1] = y->data[0];
      }
      i = pp.breaks->size[0] * pp.breaks->size[1];
      pp.breaks->size[0] = 1;
      pp.breaks->size[1] = x->size[0];
      emxEnsureCapacity_real_T(pp.breaks, i);
      szs_idx_1 = x->size[0];
      for (i = 0; i < szs_idx_1; i++) {
        pp.breaks->data[i] = x->data[i];
      }
    } else {
      szdvdf_idx_1 = 3;
      d31 = x->data[1] - x->data[0];
      dzdxdx = (y->data[1] - y->data[0]) / (float)d31;
      c_data[0] =
          ((y->data[2] - y->data[1]) / (float)(x->data[2] - x->data[1]) -
           dzdxdx) /
          (float)(x->data[2] - x->data[0]);
      c_data[1] = dzdxdx - c_data[0] * (float)d31;
      c_data[2] = y->data[0];
      i = pp.breaks->size[0] * pp.breaks->size[1];
      pp.breaks->size[0] = 1;
      pp.breaks->size[1] = 2;
      emxEnsureCapacity_real_T(pp.breaks, i);
      pp.breaks->data[0] = x->data[0];
      pp.breaks->data[1] = x->data[2];
    }
    i = pp.coefs->size[0] * pp.coefs->size[1];
    pp.coefs->size[0] = 1;
    pp.coefs->size[1] = szdvdf_idx_1;
    emxEnsureCapacity_real32_T(pp.coefs, i);
    for (i = 0; i < szdvdf_idx_1; i++) {
      pp.coefs->data[i] = c_data[i];
    }
  } else {
    nxm1 = x->size[0] - 1;
    if (has_endslopes) {
      szdvdf_idx_1 = y->size[0] - 3;
      szs_idx_1 = y->size[0] - 2;
      yoffset = 1;
    } else {
      szdvdf_idx_1 = y->size[0] - 1;
      szs_idx_1 = y->size[0];
      yoffset = 0;
    }
    i = s->size[0] * s->size[1];
    s->size[0] = 1;
    s->size[1] = szs_idx_1;
    emxEnsureCapacity_real32_T(s, i);
    i = dvdf->size[0] * dvdf->size[1];
    dvdf->size[0] = 1;
    dvdf->size[1] = szdvdf_idx_1;
    emxEnsureCapacity_real32_T(dvdf, i);
    i = dx->size[0];
    dx->size[0] = x->size[0] - 1;
    emxEnsureCapacity_real_T(dx, i);
    for (szdvdf_idx_1 = 0; szdvdf_idx_1 < nxm1; szdvdf_idx_1++) {
      d31 = x->data[szdvdf_idx_1 + 1] - x->data[szdvdf_idx_1];
      dx->data[szdvdf_idx_1] = d31;
      szs_idx_1 = yoffset + szdvdf_idx_1;
      dvdf->data[szdvdf_idx_1] =
          (y->data[szs_idx_1 + 1] - y->data[szs_idx_1]) / (float)d31;
    }
    for (szdvdf_idx_1 = 2; szdvdf_idx_1 <= nxm1; szdvdf_idx_1++) {
      s->data[szdvdf_idx_1 - 1] =
          3.0F *
          ((float)dx->data[szdvdf_idx_1 - 1] * dvdf->data[szdvdf_idx_1 - 2] +
           (float)dx->data[szdvdf_idx_1 - 2] * dvdf->data[szdvdf_idx_1 - 1]);
    }
    if (has_endslopes) {
      d31 = 0.0;
      dnnm2 = 0.0;
      s->data[0] = (float)dx->data[1] * y->data[0];
      s->data[x->size[0] - 1] =
          (float)dx->data[x->size[0] - 3] * y->data[x->size[0] + 1];
    } else {
      d31 = x->data[2] - x->data[0];
      dnnm2 = x->data[x->size[0] - 1] - x->data[x->size[0] - 3];
      s->data[0] =
          ((float)((dx->data[0] + 2.0 * d31) * dx->data[1]) * dvdf->data[0] +
           (float)(dx->data[0] * dx->data[0]) * dvdf->data[1]) /
          (float)d31;
      s->data[x->size[0] - 1] =
          ((float)((dx->data[x->size[0] - 2] + 2.0 * dnnm2) *
                   dx->data[x->size[0] - 3]) *
               dvdf->data[x->size[0] - 2] +
           (float)(dx->data[x->size[0] - 2] * dx->data[x->size[0] - 2]) *
               dvdf->data[x->size[0] - 3]) /
          (float)dnnm2;
    }
    i = md->size[0];
    md->size[0] = x->size[0];
    emxEnsureCapacity_real_T(md, i);
    md->data[0] = dx->data[1];
    md->data[x->size[0] - 1] = dx->data[x->size[0] - 3];
    for (szdvdf_idx_1 = 2; szdvdf_idx_1 <= nxm1; szdvdf_idx_1++) {
      md->data[szdvdf_idx_1 - 1] =
          2.0 * (dx->data[szdvdf_idx_1 - 1] + dx->data[szdvdf_idx_1 - 2]);
    }
    r = dx->data[1] / md->data[0];
    md->data[1] -= r * d31;
    s->data[1] -= (float)r * s->data[0];
    for (szdvdf_idx_1 = 3; szdvdf_idx_1 <= nxm1; szdvdf_idx_1++) {
      r = dx->data[szdvdf_idx_1 - 1] / md->data[szdvdf_idx_1 - 2];
      md->data[szdvdf_idx_1 - 1] -= r * dx->data[szdvdf_idx_1 - 3];
      s->data[szdvdf_idx_1 - 1] -= (float)r * s->data[szdvdf_idx_1 - 2];
    }
    r = dnnm2 / md->data[x->size[0] - 2];
    md->data[x->size[0] - 1] -= r * dx->data[x->size[0] - 3];
    s->data[x->size[0] - 1] -= (float)r * s->data[x->size[0] - 2];
    s->data[x->size[0] - 1] /= (float)md->data[x->size[0] - 1];
    for (szdvdf_idx_1 = nxm1; szdvdf_idx_1 >= 2; szdvdf_idx_1--) {
      s->data[szdvdf_idx_1 - 1] =
          (s->data[szdvdf_idx_1 - 1] -
           (float)dx->data[szdvdf_idx_1 - 2] * s->data[szdvdf_idx_1]) /
          (float)md->data[szdvdf_idx_1 - 1];
    }
    s->data[0] = (s->data[0] - (float)d31 * s->data[1]) / (float)md->data[0];
    nxm1 = x->size[0];
    szdvdf_idx_1 = s->size[1] - 1;
    i = pp_coefs->size[0] * pp_coefs->size[1];
    pp_coefs->size[0] = s->size[1] - 1;
    pp_coefs->size[1] = 4;
    emxEnsureCapacity_real32_T(pp_coefs, i);
    for (szs_idx_1 = 0; szs_idx_1 <= nxm1 - 2; szs_idx_1++) {
      dzdxdx = dvdf->data[szs_idx_1];
      f = s->data[szs_idx_1];
      dzzdx = (dzdxdx - f) / (float)dx->data[szs_idx_1];
      dzdxdx = (s->data[szs_idx_1 + 1] - dzdxdx) / (float)dx->data[szs_idx_1];
      pp_coefs->data[szs_idx_1] = (dzdxdx - dzzdx) / (float)dx->data[szs_idx_1];
      pp_coefs->data[szdvdf_idx_1 + szs_idx_1] = 2.0F * dzzdx - dzdxdx;
      pp_coefs->data[(szdvdf_idx_1 << 1) + szs_idx_1] = f;
      pp_coefs->data[3 * szdvdf_idx_1 + szs_idx_1] =
          y->data[yoffset + szs_idx_1];
    }
    i = pp.breaks->size[0] * pp.breaks->size[1];
    pp.breaks->size[0] = 1;
    pp.breaks->size[1] = x->size[0];
    emxEnsureCapacity_real_T(pp.breaks, i);
    szs_idx_1 = x->size[0];
    for (i = 0; i < szs_idx_1; i++) {
      pp.breaks->data[i] = x->data[i];
    }
    i = pp.coefs->size[0] * pp.coefs->size[1];
    pp.coefs->size[0] = pp_coefs->size[0];
    pp.coefs->size[1] = 4;
    emxEnsureCapacity_real32_T(pp.coefs, i);
    szs_idx_1 = pp_coefs->size[0] * 4;
    for (i = 0; i < szs_idx_1; i++) {
      pp.coefs->data[i] = pp_coefs->data[i];
    }
  }
  emxFree_real32_T(&pp_coefs);
  emxFree_real_T(&md);
  emxFree_real_T(&dx);
  emxFree_real32_T(&dvdf);
  emxFree_real32_T(&s);
  szdvdf_idx_1 = xi->size[0] - 1;
#pragma omp parallel for num_threads(omp_get_max_threads()) private(           \
    numTerms, ip, v, xloc, ic)

  for (k = 0; k <= szdvdf_idx_1; k++) {
    if (rtIsNaN(xi->data[k])) {
      yi->data[k] = rtNaNF;
    } else {
      numTerms = pp.coefs->size[1];
      if (rtIsNaN(xi->data[k])) {
        v = (float)xi->data[k];
      } else {
        ip = b_bsearch(pp.breaks, xi->data[k]) - 1;
        xloc = xi->data[k] - pp.breaks->data[ip];
        v = pp.coefs->data[ip];
        for (ic = 2; ic <= numTerms; ic++) {
          v = (float)xloc * v +
              pp.coefs->data[ip + (ic - 1) * (pp.breaks->size[1] - 1)];
        }
      }
      yi->data[k] = v;
    }
  }
  emxFreeStruct_struct_T(&pp);
}

/*
 * Arguments    : const emxArray_real_T *varargin_1
 *                const emxArray_real32_T *varargin_2
 *                const emxArray_real_T *varargin_3
 *                emxArray_real32_T *Vq
 * Return Type  : void
 */
void b_interp1(const emxArray_real_T *varargin_1,
               const emxArray_real32_T *varargin_2,
               const emxArray_real_T *varargin_3, emxArray_real32_T *Vq)
{
  emxArray_real32_T *y;
  emxArray_real_T *x;
  double xtmp;
  float tmp;
  int b_j1;
  int exitg1;
  int k;
  int n;
  int nx;
  emxInit_real32_T(&y, 1);
  k = y->size[0];
  y->size[0] = varargin_2->size[0];
  emxEnsureCapacity_real32_T(y, k);
  n = varargin_2->size[0];
  for (k = 0; k < n; k++) {
    y->data[k] = varargin_2->data[k];
  }
  emxInit_real_T(&x, 1);
  k = x->size[0];
  x->size[0] = varargin_1->size[0];
  emxEnsureCapacity_real_T(x, k);
  n = varargin_1->size[0];
  for (k = 0; k < n; k++) {
    x->data[k] = varargin_1->data[k];
  }
  nx = varargin_1->size[0] - 1;
  k = Vq->size[0];
  Vq->size[0] = varargin_3->size[0];
  emxEnsureCapacity_real32_T(Vq, k);
  n = varargin_3->size[0];
  for (k = 0; k < n; k++) {
    Vq->data[k] = 0.0F;
  }
  if (varargin_3->size[0] != 0) {
    k = 0;
    do {
      exitg1 = 0;
      if (k <= nx) {
        if (rtIsNaN(varargin_1->data[k])) {
          exitg1 = 1;
        } else {
          k++;
        }
      } else {
        if (varargin_1->data[1] < varargin_1->data[0]) {
          k = (nx + 1) >> 1;
          for (b_j1 = 0; b_j1 < k; b_j1++) {
            xtmp = x->data[b_j1];
            n = nx - b_j1;
            x->data[b_j1] = x->data[n];
            x->data[n] = xtmp;
          }
          if ((varargin_2->size[0] != 0) && (varargin_2->size[0] > 1)) {
            n = varargin_2->size[0] - 1;
            nx = varargin_2->size[0] >> 1;
            for (k = 0; k < nx; k++) {
              tmp = y->data[k];
              b_j1 = n - k;
              y->data[k] = y->data[b_j1];
              y->data[b_j1] = tmp;
            }
          }
        }
        interp1SplineOrPCHIP(y, varargin_3, Vq, x);
        exitg1 = 1;
      }
    } while (exitg1 == 0);
  }
  emxFree_real_T(&x);
  emxFree_real32_T(&y);
}

/*
 * Arguments    : const float y_data[]
 *                int nyrows
 *                const emxArray_real_T *xi
 *                emxArray_real32_T *yi
 *                const emxArray_real_T *varargin_1
 * Return Type  : void
 */
void b_interp1Linear(const float y_data[], int nyrows,
                     const emxArray_real_T *xi, emxArray_real32_T *yi,
                     const emxArray_real_T *varargin_1)
{
  double maxx;
  double minx;
  double penx;
  double r;
  float f;
  int high_i;
  int k;
  int low_i;
  int low_ip1;
  int mid_i;
  int ub_loop;
  minx = varargin_1->data[0];
  penx = varargin_1->data[varargin_1->size[0] - 2];
  maxx = varargin_1->data[varargin_1->size[0] - 1];
  ub_loop = xi->size[0] - 1;
#pragma omp parallel for num_threads(omp_get_max_threads()) private(           \
    f, high_i, low_i, low_ip1, r, mid_i)

  for (k = 0; k <= ub_loop; k++) {
    if (rtIsNaN(xi->data[k])) {
      yi->data[k] = rtNaNF;
    } else if (xi->data[k] > maxx) {
      if (nyrows > 1) {
        f = y_data[nyrows - 1];
        yi->data[k] = f + (float)((xi->data[k] - maxx) / (maxx - penx)) *
                              (f - y_data[nyrows - 2]);
      }
    } else if (xi->data[k] < minx) {
      yi->data[k] = y_data[0] + (float)((xi->data[k] - minx) /
                                        (varargin_1->data[1] - minx)) *
                                    (y_data[1] - y_data[0]);
    } else {
      high_i = varargin_1->size[0];
      low_i = 1;
      low_ip1 = 2;
      while (high_i > low_ip1) {
        mid_i = (low_i >> 1) + (high_i >> 1);
        if (((low_i & 1) == 1) && ((high_i & 1) == 1)) {
          mid_i++;
        }
        if (xi->data[k] >= varargin_1->data[mid_i - 1]) {
          low_i = mid_i;
          low_ip1 = mid_i + 1;
        } else {
          high_i = mid_i;
        }
      }
      r = varargin_1->data[low_i - 1];
      r = (xi->data[k] - r) / (varargin_1->data[low_i] - r);
      if (r == 0.0) {
        yi->data[k] = y_data[low_i - 1];
      } else if (r == 1.0) {
        yi->data[k] = y_data[low_i];
      } else {
        f = y_data[low_i - 1];
        if (f == y_data[low_i]) {
          yi->data[k] = f;
        } else {
          yi->data[k] = (float)(1.0 - r) * f + (float)r * y_data[low_i];
        }
      }
    }
  }
}

/*
 * Arguments    : const double varargin_1_data[]
 *                const int varargin_1_size[2]
 *                const creal32_T varargin_2_data[]
 *                const int varargin_2_size[2]
 *                double varargin_3
 * Return Type  : creal32_T
 */
creal32_T c_interp1(const double varargin_1_data[],
                    const int varargin_1_size[2],
                    const creal32_T varargin_2_data[],
                    const int varargin_2_size[2], double varargin_3)
{
  emxArray_real_T b_x_data;
  creal32_T y_data[17];
  creal32_T Vq;
  double x_data[15];
  double xtmp;
  float xtmp_im;
  float xtmp_re;
  int x_size[2];
  int b_j1;
  int exitg1;
  int j2;
  int nd2;
  int nx;
  int nyrows;
  nd2 = varargin_2_size[1];
  if (0 <= nd2 - 1) {
    memcpy(&y_data[0], &varargin_2_data[0], nd2 * sizeof(creal32_T));
  }
  x_size[0] = 1;
  x_size[1] = varargin_1_size[1];
  nd2 = varargin_1_size[1];
  if (0 <= nd2 - 1) {
    memcpy(&x_data[0], &varargin_1_data[0], nd2 * sizeof(double));
  }
  nyrows = varargin_2_size[1];
  nx = varargin_1_size[1] - 1;
  nd2 = 0;
  do {
    exitg1 = 0;
    if (nd2 <= nx) {
      if (rtIsNaN(varargin_1_data[nd2])) {
        exitg1 = 1;
      } else {
        nd2++;
      }
    } else {
      if (varargin_1_data[1] < varargin_1_data[0]) {
        nd2 = (nx + 1) >> 1;
        for (b_j1 = 0; b_j1 < nd2; b_j1++) {
          xtmp = x_data[b_j1];
          j2 = nx - b_j1;
          x_data[b_j1] = x_data[j2];
          x_data[j2] = xtmp;
        }
        nd2 = varargin_2_size[1] >> 1;
        for (b_j1 = 0; b_j1 < nd2; b_j1++) {
          j2 = (varargin_2_size[1] - b_j1) - 1;
          xtmp_re = y_data[b_j1].re;
          xtmp_im = y_data[b_j1].im;
          y_data[b_j1] = y_data[j2];
          y_data[j2].re = xtmp_re;
          y_data[j2].im = xtmp_im;
        }
      }
      Vq.re = 0.0F;
      Vq.im = 0.0F;
      if (rtIsNaN(varargin_3)) {
        Vq.re = rtNaNF;
        Vq.im = rtNaNF;
      } else if (varargin_3 > x_data[x_size[1] - 1]) {
        if (nyrows > 1) {
          Vq.re = y_data[nyrows - 1].re +
                  (float)((varargin_3 - x_data[x_size[1] - 1]) /
                          (x_data[x_size[1] - 1] - x_data[x_size[1] - 2])) *
                      (y_data[nyrows - 1].re - y_data[nyrows - 2].re);
          Vq.im = y_data[nyrows - 1].im +
                  (float)((varargin_3 - x_data[x_size[1] - 1]) /
                          (x_data[x_size[1] - 1] - x_data[x_size[1] - 2])) *
                      (y_data[nyrows - 1].im - y_data[nyrows - 2].im);
        }
      } else if (varargin_3 < x_data[0]) {
        xtmp_re = (float)((varargin_3 - x_data[0]) / (x_data[1] - x_data[0]));
        Vq.re = y_data[0].re + xtmp_re * (y_data[1].re - y_data[0].re);
        Vq.im = y_data[0].im + xtmp_re * (y_data[1].im - y_data[0].im);
      } else {
        b_x_data.data = &x_data[0];
        b_x_data.size = &x_size[0];
        b_x_data.allocatedSize = 15;
        b_x_data.numDimensions = 2;
        b_x_data.canFreeData = false;
        nd2 = b_bsearch(&b_x_data, varargin_3) - 1;
        xtmp = (varargin_3 - x_data[nd2]) / (x_data[nd2 + 1] - x_data[nd2]);
        if (xtmp == 0.0) {
          Vq = y_data[nd2];
        } else if (xtmp == 1.0) {
          Vq = y_data[nd2 + 1];
        } else {
          Vq = y_data[nd2];
          if ((!(y_data[nd2].re == y_data[nd2 + 1].re)) ||
              (!(y_data[nd2].im == y_data[nd2 + 1].im))) {
            Vq.re = (float)(1.0 - xtmp) * y_data[nd2].re +
                    (float)xtmp * y_data[nd2 + 1].re;
            Vq.im = (float)(1.0 - xtmp) * y_data[nd2].im +
                    (float)xtmp * y_data[nd2 + 1].im;
          }
        }
      }
      exitg1 = 1;
    }
  } while (exitg1 == 0);
  return Vq;
}

/*
 * Arguments    : const emxArray_real_T *varargin_1
 *                const emxArray_creal32_T *varargin_2
 *                const emxArray_real_T *varargin_3
 *                emxArray_creal32_T *Vq
 * Return Type  : void
 */
void interp1(const emxArray_real_T *varargin_1,
             const emxArray_creal32_T *varargin_2,
             const emxArray_real_T *varargin_3, emxArray_creal32_T *Vq)
{
  emxArray_creal32_T *b_y;
  emxArray_creal32_T *dvdf;
  emxArray_creal32_T *pp1_coefs;
  emxArray_creal32_T *pp_coefs;
  emxArray_creal32_T *s;
  emxArray_creal32_T *y;
  emxArray_real_T *dx;
  emxArray_real_T *md;
  emxArray_real_T *pp_breaks;
  emxArray_real_T *x;
  creal32_T c_data[4];
  creal32_T endslopes_data_idx_0;
  creal32_T endslopes_data_idx_1;
  double dnnm2;
  double r;
  double xtmp;
  float ai;
  float ar;
  float b_dx_re_tmp;
  float dx_re_tmp;
  float dzzdx_im;
  float dzzdx_re;
  float tmp_im;
  float tmp_re;
  int exitg1;
  int k;
  int n;
  int nx;
  int nxi;
  int nxm1;
  int szs_idx_1;
  boolean_T has_endslopes;
  emxInit_creal32_T(&y, 2);
  k = y->size[0] * y->size[1];
  y->size[0] = varargin_2->size[0];
  y->size[1] = 1;
  emxEnsureCapacity_creal32_T(y, k);
  n = varargin_2->size[0];
  for (k = 0; k < n; k++) {
    y->data[k] = varargin_2->data[k];
  }
  emxInit_real_T(&x, 1);
  k = x->size[0];
  x->size[0] = varargin_1->size[0];
  emxEnsureCapacity_real_T(x, k);
  n = varargin_1->size[0];
  for (k = 0; k < n; k++) {
    x->data[k] = varargin_1->data[k];
  }
  nx = varargin_1->size[0] - 1;
  k = Vq->size[0] * Vq->size[1];
  Vq->size[0] = varargin_3->size[0];
  Vq->size[1] = 1;
  emxEnsureCapacity_creal32_T(Vq, k);
  n = varargin_3->size[0];
  for (k = 0; k < n; k++) {
    Vq->data[k].re = 0.0F;
    Vq->data[k].im = 0.0F;
  }
  emxInit_creal32_T(&pp_coefs, 3);
  emxInit_creal32_T(&b_y, 2);
  emxInit_creal32_T(&s, 2);
  emxInit_creal32_T(&dvdf, 2);
  emxInit_real_T(&dx, 1);
  emxInit_real_T(&md, 1);
  emxInit_real_T(&pp_breaks, 2);
  emxInit_creal32_T(&pp1_coefs, 3);
  if (varargin_3->size[0] != 0) {
    k = 0;
    do {
      exitg1 = 0;
      if (k <= nx) {
        if (rtIsNaN(varargin_1->data[k])) {
          exitg1 = 1;
        } else {
          k++;
        }
      } else {
        if (varargin_1->data[1] < varargin_1->data[0]) {
          k = (nx + 1) >> 1;
          for (szs_idx_1 = 0; szs_idx_1 < k; szs_idx_1++) {
            xtmp = x->data[szs_idx_1];
            n = nx - szs_idx_1;
            x->data[szs_idx_1] = x->data[n];
            x->data[n] = xtmp;
          }
          if ((varargin_2->size[0] != 0) && (varargin_2->size[0] > 1)) {
            n = varargin_2->size[0] - 1;
            nx = varargin_2->size[0] >> 1;
            for (k = 0; k < nx; k++) {
              tmp_re = y->data[k].re;
              tmp_im = y->data[k].im;
              szs_idx_1 = n - k;
              y->data[k] = y->data[szs_idx_1];
              y->data[szs_idx_1].re = tmp_re;
              y->data[szs_idx_1].im = tmp_im;
            }
          }
        }
        nxi = varargin_3->size[0];
        k = b_y->size[0] * b_y->size[1];
        b_y->size[0] = 1;
        b_y->size[1] = y->size[0];
        emxEnsureCapacity_creal32_T(b_y, k);
        n = y->size[0];
        for (k = 0; k < n; k++) {
          b_y->data[k] = y->data[k];
        }
        has_endslopes = (b_y->size[1] == x->size[0] + 2);
        if ((x->size[0] <= 2) || ((x->size[0] <= 3) && (!has_endslopes))) {
          has_endslopes = (b_y->size[1] == x->size[0] + 2);
          if (x->size[0] <= 2) {
            if (has_endslopes) {
              nx = 4;
            } else {
              nx = 2;
            }
          } else {
            nx = 3;
          }
          if (x->size[0] <= 2) {
            if (has_endslopes) {
              endslopes_data_idx_0 = b_y->data[0];
              endslopes_data_idx_1 = b_y->data[b_y->size[1] - 1];
              nxm1 = x->size[0] - 1;
              k = pp1_coefs->size[0] * pp1_coefs->size[1] * pp1_coefs->size[2];
              pp1_coefs->size[0] = 1;
              pp1_coefs->size[1] = x->size[0] - 1;
              pp1_coefs->size[2] = 4;
              emxEnsureCapacity_creal32_T(pp1_coefs, k);
              for (n = 0; n < nxm1; n++) {
                xtmp = x->data[1] - x->data[0];
                ar = b_y->data[2].re - b_y->data[1].re;
                ai = b_y->data[2].im - b_y->data[1].im;
                if (ai == 0.0F) {
                  tmp_re = ar / (float)xtmp;
                  tmp_im = 0.0F;
                } else if (ar == 0.0F) {
                  tmp_re = 0.0F;
                  tmp_im = ai / (float)xtmp;
                } else {
                  tmp_re = ar / (float)xtmp;
                  tmp_im = ai / (float)xtmp;
                }
                ar = tmp_re - endslopes_data_idx_0.re;
                ai = tmp_im - endslopes_data_idx_0.im;
                if (ai == 0.0F) {
                  dzzdx_re = ar / (float)xtmp;
                  dzzdx_im = 0.0F;
                } else if (ar == 0.0F) {
                  dzzdx_re = 0.0F;
                  dzzdx_im = ai / (float)xtmp;
                } else {
                  dzzdx_re = ar / (float)xtmp;
                  dzzdx_im = ai / (float)xtmp;
                }
                ar = endslopes_data_idx_1.re - tmp_re;
                ai = endslopes_data_idx_1.im - tmp_im;
                if (ai == 0.0F) {
                  b_dx_re_tmp = ar / (float)xtmp;
                  dx_re_tmp = 0.0F;
                } else if (ar == 0.0F) {
                  b_dx_re_tmp = 0.0F;
                  dx_re_tmp = ai / (float)xtmp;
                } else {
                  b_dx_re_tmp = ar / (float)xtmp;
                  dx_re_tmp = ai / (float)xtmp;
                }
                ar = b_dx_re_tmp - dzzdx_re;
                ai = dx_re_tmp - dzzdx_im;
                if (ai == 0.0F) {
                  pp1_coefs->data[0].re = ar / (float)xtmp;
                  pp1_coefs->data[0].im = 0.0F;
                } else if (ar == 0.0F) {
                  pp1_coefs->data[0].re = 0.0F;
                  pp1_coefs->data[0].im = ai / (float)xtmp;
                } else {
                  pp1_coefs->data[0].re = ar / (float)xtmp;
                  pp1_coefs->data[0].im = ai / (float)xtmp;
                }
                pp1_coefs->data[x->size[0] - 1].re =
                    2.0F * dzzdx_re - b_dx_re_tmp;
                pp1_coefs->data[x->size[0] - 1].im =
                    2.0F * dzzdx_im - dx_re_tmp;
                k = nxm1 << 1;
                pp1_coefs->data[k].re = endslopes_data_idx_0.re;
                pp1_coefs->data[k].im = endslopes_data_idx_0.im;
                pp1_coefs->data[3 * nxm1].re = b_y->data[1].re;
                pp1_coefs->data[3 * nxm1].im = b_y->data[1].im;
              }
              for (k = 0; k < nx; k++) {
                c_data[k] = pp1_coefs->data[k];
              }
            } else {
              ar = b_y->data[1].re - b_y->data[0].re;
              ai = b_y->data[1].im - b_y->data[0].im;
              dx_re_tmp = (float)(x->data[1] - x->data[0]);
              if (ai == 0.0F) {
                c_data[0].re = ar / dx_re_tmp;
                c_data[0].im = 0.0F;
              } else if (ar == 0.0F) {
                c_data[0].re = 0.0F;
                c_data[0].im = ai / dx_re_tmp;
              } else {
                c_data[0].re = ar / dx_re_tmp;
                c_data[0].im = ai / dx_re_tmp;
              }
              c_data[1] = b_y->data[0];
            }
            k = pp_breaks->size[0] * pp_breaks->size[1];
            pp_breaks->size[0] = 1;
            pp_breaks->size[1] = x->size[0];
            emxEnsureCapacity_real_T(pp_breaks, k);
            n = x->size[0];
            for (k = 0; k < n; k++) {
              pp_breaks->data[k] = x->data[k];
            }
          } else {
            xtmp = x->data[1] - x->data[0];
            ar = b_y->data[1].re - b_y->data[0].re;
            ai = b_y->data[1].im - b_y->data[0].im;
            if (ai == 0.0F) {
              tmp_re = ar / (float)xtmp;
              tmp_im = 0.0F;
            } else if (ar == 0.0F) {
              tmp_re = 0.0F;
              tmp_im = ai / (float)xtmp;
            } else {
              tmp_re = ar / (float)xtmp;
              tmp_im = ai / (float)xtmp;
            }
            ar = b_y->data[2].re - b_y->data[1].re;
            ai = b_y->data[2].im - b_y->data[1].im;
            dx_re_tmp = (float)(x->data[2] - x->data[1]);
            if (ai == 0.0F) {
              b_dx_re_tmp = ar / dx_re_tmp;
              dx_re_tmp = 0.0F;
            } else if (ar == 0.0F) {
              b_dx_re_tmp = 0.0F;
              dx_re_tmp = ai / dx_re_tmp;
            } else {
              b_dx_re_tmp = ar / dx_re_tmp;
              dx_re_tmp = ai / dx_re_tmp;
            }
            ar = b_dx_re_tmp - tmp_re;
            ai = dx_re_tmp - tmp_im;
            dx_re_tmp = (float)(x->data[2] - x->data[0]);
            if (ai == 0.0F) {
              c_data[0].re = ar / dx_re_tmp;
              c_data[0].im = 0.0F;
            } else if (ar == 0.0F) {
              c_data[0].re = 0.0F;
              c_data[0].im = ai / dx_re_tmp;
            } else {
              c_data[0].re = ar / dx_re_tmp;
              c_data[0].im = ai / dx_re_tmp;
            }
            c_data[1].re = tmp_re - c_data[0].re * (float)xtmp;
            c_data[1].im = tmp_im - c_data[0].im * (float)xtmp;
            c_data[2] = b_y->data[0];
            k = pp_breaks->size[0] * pp_breaks->size[1];
            pp_breaks->size[0] = 1;
            pp_breaks->size[1] = 2;
            emxEnsureCapacity_real_T(pp_breaks, k);
            pp_breaks->data[0] = x->data[0];
            pp_breaks->data[1] = x->data[2];
          }
          k = pp_coefs->size[0] * pp_coefs->size[1] * pp_coefs->size[2];
          pp_coefs->size[0] = 1;
          pp_coefs->size[1] = 1;
          pp_coefs->size[2] = nx;
          emxEnsureCapacity_creal32_T(pp_coefs, k);
          for (k = 0; k < nx; k++) {
            pp_coefs->data[k] = c_data[k];
          }
        } else {
          nxm1 = x->size[0] - 1;
          if (has_endslopes) {
            n = b_y->size[1] - 3;
            szs_idx_1 = b_y->size[1] - 2;
            nx = 1;
          } else {
            n = b_y->size[1] - 1;
            szs_idx_1 = b_y->size[1];
            nx = 0;
          }
          k = s->size[0] * s->size[1];
          s->size[0] = 1;
          s->size[1] = szs_idx_1;
          emxEnsureCapacity_creal32_T(s, k);
          k = dvdf->size[0] * dvdf->size[1];
          dvdf->size[0] = 1;
          dvdf->size[1] = n;
          emxEnsureCapacity_creal32_T(dvdf, k);
          k = dx->size[0];
          dx->size[0] = x->size[0] - 1;
          emxEnsureCapacity_real_T(dx, k);
          for (k = 0; k < nxm1; k++) {
            xtmp = x->data[k + 1] - x->data[k];
            dx->data[k] = xtmp;
            n = nx + k;
            ar = b_y->data[n + 1].re - b_y->data[n].re;
            ai = b_y->data[n + 1].im - b_y->data[n].im;
            if (ai == 0.0F) {
              dvdf->data[k].re = ar / (float)xtmp;
              dvdf->data[k].im = 0.0F;
            } else if (ar == 0.0F) {
              dvdf->data[k].re = 0.0F;
              dvdf->data[k].im = ai / (float)xtmp;
            } else {
              dvdf->data[k].re = ar / (float)xtmp;
              dvdf->data[k].im = ai / (float)xtmp;
            }
          }
          for (k = 2; k <= nxm1; k++) {
            dx_re_tmp = (float)dx->data[k - 1];
            b_dx_re_tmp = (float)dx->data[k - 2];
            s->data[k - 1].re = 3.0F * (dx_re_tmp * dvdf->data[k - 2].re +
                                        b_dx_re_tmp * dvdf->data[k - 1].re);
            s->data[k - 1].im = 3.0F * (dx_re_tmp * dvdf->data[k - 2].im +
                                        b_dx_re_tmp * dvdf->data[k - 1].im);
          }
          if (has_endslopes) {
            xtmp = 0.0;
            dnnm2 = 0.0;
            s->data[0].re = (float)dx->data[1] * b_y->data[0].re;
            s->data[0].im = (float)dx->data[1] * b_y->data[0].im;
            s->data[x->size[0] - 1].re =
                (float)dx->data[x->size[0] - 3] * b_y->data[x->size[0] + 1].re;
            s->data[x->size[0] - 1].im =
                (float)dx->data[x->size[0] - 3] * b_y->data[x->size[0] + 1].im;
          } else {
            xtmp = x->data[2] - x->data[0];
            dnnm2 = x->data[x->size[0] - 1] - x->data[x->size[0] - 3];
            dx_re_tmp = (float)((dx->data[0] + 2.0 * xtmp) * dx->data[1]);
            b_dx_re_tmp = (float)(dx->data[0] * dx->data[0]);
            ar = dx_re_tmp * dvdf->data[0].re + b_dx_re_tmp * dvdf->data[1].re;
            ai = dx_re_tmp * dvdf->data[0].im + b_dx_re_tmp * dvdf->data[1].im;
            if (ai == 0.0F) {
              s->data[0].re = ar / (float)xtmp;
              s->data[0].im = 0.0F;
            } else if (ar == 0.0F) {
              s->data[0].re = 0.0F;
              s->data[0].im = ai / (float)xtmp;
            } else {
              s->data[0].re = ar / (float)xtmp;
              s->data[0].im = ai / (float)xtmp;
            }
            ar = (float)((dx->data[x->size[0] - 2] + 2.0 * dnnm2) *
                         dx->data[x->size[0] - 3]) *
                     dvdf->data[x->size[0] - 2].re +
                 (float)(dx->data[x->size[0] - 2] * dx->data[x->size[0] - 2]) *
                     dvdf->data[x->size[0] - 3].re;
            ai = (float)((dx->data[x->size[0] - 2] + 2.0 * dnnm2) *
                         dx->data[x->size[0] - 3]) *
                     dvdf->data[x->size[0] - 2].im +
                 (float)(dx->data[x->size[0] - 2] * dx->data[x->size[0] - 2]) *
                     dvdf->data[x->size[0] - 3].im;
            if (ai == 0.0F) {
              s->data[x->size[0] - 1].re = ar / (float)dnnm2;
              s->data[x->size[0] - 1].im = 0.0F;
            } else if (ar == 0.0F) {
              s->data[x->size[0] - 1].re = 0.0F;
              s->data[x->size[0] - 1].im = ai / (float)dnnm2;
            } else {
              s->data[x->size[0] - 1].re = ar / (float)dnnm2;
              s->data[x->size[0] - 1].im = ai / (float)dnnm2;
            }
          }
          k = md->size[0];
          md->size[0] = x->size[0];
          emxEnsureCapacity_real_T(md, k);
          md->data[0] = dx->data[1];
          md->data[x->size[0] - 1] = dx->data[x->size[0] - 3];
          for (k = 2; k <= nxm1; k++) {
            md->data[k - 1] = 2.0 * (dx->data[k - 1] + dx->data[k - 2]);
          }
          r = dx->data[1] / md->data[0];
          md->data[1] -= r * xtmp;
          s->data[1].re -= (float)r * s->data[0].re;
          s->data[1].im -= (float)r * s->data[0].im;
          for (k = 3; k <= nxm1; k++) {
            r = dx->data[k - 1] / md->data[k - 2];
            md->data[k - 1] -= r * dx->data[k - 3];
            s->data[k - 1].re -= (float)r * s->data[k - 2].re;
            s->data[k - 1].im -= (float)r * s->data[k - 2].im;
          }
          r = dnnm2 / md->data[x->size[0] - 2];
          md->data[x->size[0] - 1] -= r * dx->data[x->size[0] - 3];
          s->data[x->size[0] - 1].re -= (float)r * s->data[x->size[0] - 2].re;
          s->data[x->size[0] - 1].im -= (float)r * s->data[x->size[0] - 2].im;
          ar = s->data[x->size[0] - 1].re;
          ai = s->data[x->size[0] - 1].im;
          dx_re_tmp = (float)md->data[x->size[0] - 1];
          if (ai == 0.0F) {
            b_dx_re_tmp = ar / dx_re_tmp;
            dx_re_tmp = 0.0F;
          } else if (ar == 0.0F) {
            b_dx_re_tmp = 0.0F;
            dx_re_tmp = ai / dx_re_tmp;
          } else {
            b_dx_re_tmp = ar / dx_re_tmp;
            dx_re_tmp = ai / dx_re_tmp;
          }
          s->data[x->size[0] - 1].re = b_dx_re_tmp;
          s->data[x->size[0] - 1].im = dx_re_tmp;
          for (k = nxm1; k >= 2; k--) {
            dx_re_tmp = (float)dx->data[k - 2];
            ar = s->data[k - 1].re - dx_re_tmp * s->data[k].re;
            ai = s->data[k - 1].im - dx_re_tmp * s->data[k].im;
            dx_re_tmp = (float)md->data[k - 1];
            if (ai == 0.0F) {
              b_dx_re_tmp = ar / dx_re_tmp;
              dx_re_tmp = 0.0F;
            } else if (ar == 0.0F) {
              b_dx_re_tmp = 0.0F;
              dx_re_tmp = ai / dx_re_tmp;
            } else {
              b_dx_re_tmp = ar / dx_re_tmp;
              dx_re_tmp = ai / dx_re_tmp;
            }
            s->data[k - 1].re = b_dx_re_tmp;
            s->data[k - 1].im = dx_re_tmp;
          }
          ar = s->data[0].re - (float)xtmp * s->data[1].re;
          ai = s->data[0].im - (float)xtmp * s->data[1].im;
          if (ai == 0.0F) {
            b_dx_re_tmp = ar / (float)md->data[0];
            dx_re_tmp = 0.0F;
          } else if (ar == 0.0F) {
            b_dx_re_tmp = 0.0F;
            dx_re_tmp = ai / (float)md->data[0];
          } else {
            b_dx_re_tmp = ar / (float)md->data[0];
            dx_re_tmp = ai / (float)md->data[0];
          }
          s->data[0].re = b_dx_re_tmp;
          s->data[0].im = dx_re_tmp;
          nxm1 = x->size[0] - 1;
          k = pp1_coefs->size[0] * pp1_coefs->size[1] * pp1_coefs->size[2];
          pp1_coefs->size[0] = 1;
          pp1_coefs->size[1] = x->size[0] - 1;
          pp1_coefs->size[2] = 4;
          emxEnsureCapacity_creal32_T(pp1_coefs, k);
          for (n = 0; n < nxm1; n++) {
            ar = dvdf->data[n].re - s->data[n].re;
            ai = dvdf->data[n].im - s->data[n].im;
            dx_re_tmp = (float)dx->data[n];
            if (ai == 0.0F) {
              dzzdx_re = ar / dx_re_tmp;
              dzzdx_im = 0.0F;
            } else if (ar == 0.0F) {
              dzzdx_re = 0.0F;
              dzzdx_im = ai / dx_re_tmp;
            } else {
              dzzdx_re = ar / dx_re_tmp;
              dzzdx_im = ai / dx_re_tmp;
            }
            ar = s->data[n + 1].re - dvdf->data[n].re;
            ai = s->data[n + 1].im - dvdf->data[n].im;
            if (ai == 0.0F) {
              tmp_re = ar / dx_re_tmp;
              tmp_im = 0.0F;
            } else if (ar == 0.0F) {
              tmp_re = 0.0F;
              tmp_im = ai / dx_re_tmp;
            } else {
              tmp_re = ar / dx_re_tmp;
              tmp_im = ai / dx_re_tmp;
            }
            ar = tmp_re - dzzdx_re;
            ai = tmp_im - dzzdx_im;
            if (ai == 0.0F) {
              pp1_coefs->data[n].re = ar / dx_re_tmp;
              pp1_coefs->data[n].im = 0.0F;
            } else if (ar == 0.0F) {
              pp1_coefs->data[n].re = 0.0F;
              pp1_coefs->data[n].im = ai / dx_re_tmp;
            } else {
              pp1_coefs->data[n].re = ar / dx_re_tmp;
              pp1_coefs->data[n].im = ai / dx_re_tmp;
            }
            pp1_coefs->data[(x->size[0] + n) - 1].re = 2.0F * dzzdx_re - tmp_re;
            pp1_coefs->data[(x->size[0] + n) - 1].im = 2.0F * dzzdx_im - tmp_im;
            pp1_coefs->data[(nxm1 << 1) + n] = s->data[n];
            pp1_coefs->data[3 * nxm1 + n] = b_y->data[nx + n];
          }
          k = pp_breaks->size[0] * pp_breaks->size[1];
          pp_breaks->size[0] = 1;
          pp_breaks->size[1] = x->size[0];
          emxEnsureCapacity_real_T(pp_breaks, k);
          n = x->size[0];
          for (k = 0; k < n; k++) {
            pp_breaks->data[k] = x->data[k];
          }
          k = pp_coefs->size[0] * pp_coefs->size[1] * pp_coefs->size[2];
          pp_coefs->size[0] = 1;
          pp_coefs->size[1] = pp1_coefs->size[1];
          pp_coefs->size[2] = 4;
          emxEnsureCapacity_creal32_T(pp_coefs, k);
          n = pp1_coefs->size[1] * 4;
          for (k = 0; k < n; k++) {
            pp_coefs->data[k] = pp1_coefs->data[k];
          }
        }
        for (k = 0; k < nxi; k++) {
          if (rtIsNaN(varargin_3->data[k])) {
            Vq->data[k].re = rtNaNF;
            Vq->data[k].im = rtNaNF;
          } else {
            n = pp_coefs->size[2];
            if (rtIsNaN(varargin_3->data[k])) {
              tmp_re = (float)varargin_3->data[k];
              tmp_im = 0.0F;
            } else {
              nx = b_bsearch(pp_breaks, varargin_3->data[k]) - 1;
              xtmp = varargin_3->data[k] - pp_breaks->data[nx];
              tmp_re = pp_coefs->data[nx].re;
              tmp_im = pp_coefs->data[nx].im;
              for (szs_idx_1 = 2; szs_idx_1 <= n; szs_idx_1++) {
                tmp_re =
                    (float)xtmp * tmp_re +
                    pp_coefs
                        ->data[nx + (szs_idx_1 - 1) * (pp_breaks->size[1] - 1)]
                        .re;
                tmp_im =
                    (float)xtmp * tmp_im +
                    pp_coefs
                        ->data[nx + (szs_idx_1 - 1) * (pp_breaks->size[1] - 1)]
                        .im;
              }
            }
            endslopes_data_idx_0.re = tmp_re;
            endslopes_data_idx_0.im = tmp_im;
            Vq->data[k] = endslopes_data_idx_0;
          }
        }
        exitg1 = 1;
      }
    } while (exitg1 == 0);
  }
  emxFree_creal32_T(&pp1_coefs);
  emxFree_real_T(&pp_breaks);
  emxFree_real_T(&md);
  emxFree_real_T(&dx);
  emxFree_creal32_T(&dvdf);
  emxFree_creal32_T(&s);
  emxFree_creal32_T(&b_y);
  emxFree_creal32_T(&pp_coefs);
  emxFree_real_T(&x);
  emxFree_creal32_T(&y);
}

/*
 * Arguments    : const float y_data[]
 *                int nyrows
 *                const emxArray_real_T *xi
 *                emxArray_real32_T *yi
 *                const emxArray_real_T *varargin_1
 * Return Type  : void
 */
void interp1Linear(const float y_data[], int nyrows, const emxArray_real_T *xi,
                   emxArray_real32_T *yi, const emxArray_real_T *varargin_1)
{
  double maxx;
  double minx;
  double penx;
  double r;
  float f;
  int high_i;
  int k;
  int low_i;
  int low_ip1;
  int mid_i;
  int ub_loop;
  minx = varargin_1->data[0];
  penx = varargin_1->data[varargin_1->size[0] * varargin_1->size[1] - 2];
  maxx = varargin_1->data[varargin_1->size[0] * varargin_1->size[1] - 1];
  ub_loop = xi->size[0] - 1;
#pragma omp parallel for num_threads(omp_get_max_threads()) private(           \
    f, high_i, low_i, low_ip1, r, mid_i)

  for (k = 0; k <= ub_loop; k++) {
    if (rtIsNaN(xi->data[k])) {
      yi->data[k] = rtNaNF;
    } else if (xi->data[k] > maxx) {
      if (nyrows > 1) {
        f = y_data[nyrows - 1];
        yi->data[k] = f + (float)((xi->data[k] - maxx) / (maxx - penx)) *
                              (f - y_data[nyrows - 2]);
      }
    } else if (xi->data[k] < minx) {
      yi->data[k] = y_data[0] + (float)((xi->data[k] - minx) /
                                        (varargin_1->data[1] - minx)) *
                                    (y_data[1] - y_data[0]);
    } else {
      high_i = varargin_1->size[0] * varargin_1->size[1];
      low_i = 1;
      low_ip1 = 2;
      while (high_i > low_ip1) {
        mid_i = (low_i >> 1) + (high_i >> 1);
        if (((low_i & 1) == 1) && ((high_i & 1) == 1)) {
          mid_i++;
        }
        if (xi->data[k] >= varargin_1->data[mid_i - 1]) {
          low_i = mid_i;
          low_ip1 = mid_i + 1;
        } else {
          high_i = mid_i;
        }
      }
      r = (xi->data[k] - varargin_1->data[low_i - 1]) /
          (varargin_1->data[low_i] - varargin_1->data[low_i - 1]);
      if (r == 0.0) {
        yi->data[k] = y_data[low_i - 1];
      } else if (r == 1.0) {
        yi->data[k] = y_data[low_i];
      } else {
        f = y_data[low_i - 1];
        if (f == y_data[low_i]) {
          yi->data[k] = f;
        } else {
          yi->data[k] = (float)(1.0 - r) * f + (float)r * y_data[low_i];
        }
      }
    }
  }
}

/*
 * File trailer for interp1.c
 *
 * [EOF]
 */

/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: nrChannelEstimateOpt.c
 *
 * MATLAB Coder version            : 5.2
 * C/C++ source code generated on  : 20-Feb-2022 08:55:56
 */

/* Include Files */
#include "nrChannelEstimateOpt.h"
#include "OFDMInfo.h"
#include "bsearch.h"
#include "bsxfun.h"
#include "cart2pol.h"
#include "circshift.h"
#include "colon.h"
#include "convmtx.h"
#include "diff.h"
#include "fft.h"
#include "find.h"
#include "floor.h"
#include "ifft.h"
#include "interp1.h"
#include "interp2.h"
#include "mean.h"
#include "meshgrid.h"
#include "minOrMax.h"
#include "operation_data.h"
#include "operation_emxutil.h"
#include "operation_rtwutil.h"
#include "operation_types.h"
#include "polyfit.h"
#include "repmat.h"
#include "rt_nonfinite.h"
#include "sum.h"
#include "unique.h"
#include "unwrap.h"
#include "var.h"
#include "rtGetNaN.h"
#include "rt_nonfinite.h"
#include <math.h>
#include <string.h>

/* Type Definitions */
#ifndef typedef_struct_T
#define typedef_struct_T
typedef struct {
  double CDMLengths[2];
  double AveragingWindow[2];
} struct_T;
#endif /* typedef_struct_T */

#ifndef struct_emxArray_boolean_T_1x52
#define struct_emxArray_boolean_T_1x52
struct emxArray_boolean_T_1x52 {
  boolean_T data[52];
  int size[2];
};
#endif /* struct_emxArray_boolean_T_1x52 */
#ifndef typedef_emxArray_boolean_T_1x52
#define typedef_emxArray_boolean_T_1x52
typedef struct emxArray_boolean_T_1x52 emxArray_boolean_T_1x52;
#endif /* typedef_emxArray_boolean_T_1x52 */

#ifndef typedef_cell_wrap_50
#define typedef_cell_wrap_50
typedef struct {
  emxArray_boolean_T_1x52 f1;
} cell_wrap_50;
#endif /* typedef_cell_wrap_50 */

/* Function Declarations */
static void averaging(const emxArray_creal32_T *in, double N,
                      emxArray_creal32_T *out);

static void b_averaging(const emxArray_creal32_T *in, double N,
                        emxArray_creal32_T *out);

static void b_getPortSubscripts(double N, const emxArray_real_T *ind,
                                emxArray_real_T *ksubs, emxArray_real_T *nsubs);

static void b_interpolate(const emxArray_real_T *k0, double l0_data[],
                          int l0_size[2], emxArray_creal32_T *H0,
                          const emxArray_real_T *k, const double n_data[],
                          const int n_size[2], emxArray_creal32_T *H);

static void calculateVPIndices(double nd, const emxArray_real_T *vpi,
                               const double siz[4], double n,
                               emxArray_real_T *ind);

static void cartInterpolate(double nd, const emxArray_real_T *ipt,
                            const emxArray_real_T *k, double n, float H0_data[],
                            int H0_size, emxArray_real32_T *H);

static void createVPInterpolators(const double kedge_data[], int kedge_size,
                                  emxArray_real_T *ksubs,
                                  const emxArray_real_T *lsubs,
                                  const double range[2],
                                  emxArray_cell_wrap_8 *vpis,
                                  emxArray_real_T *uk, emxArray_real_T *nd);

static void createVPs(double nd, const emxArray_real_T *vpi,
                      const emxArray_real_T *k, double n,
                      const emxArray_creal32_T *H0, emxArray_creal32_T *vps);

static float determineCSIScaling(const double refRBs_data[],
                                 const int refRBs_size[3],
                                 const emxArray_creal32_T *H, float nVar);

static double determineNoiseScaling(const emxArray_real_T *subn);

static void getPortSubscripts(double N, const emxArray_real_T *ind,
                              emxArray_real_T *ksubs, emxArray_real_T *nsubs,
                              boolean_T thisport_data[], int *thisport_size);

static void interpolate(const emxArray_real_T *k0, const emxArray_creal32_T *H0,
                        const emxArray_real_T *k, emxArray_creal32_T *H);

static void raised_cosine_window(double n, double w, double p_data[],
                                 int *p_size);

static void setAveragingParameters(struct_T *cec, float nVar);

/* Function Definitions */
/*
 * Convolve each channel of 'in' with an N-by-1 vector of ones
 *
 * Arguments    : const emxArray_creal32_T *in
 *                double N
 *                emxArray_creal32_T *out
 * Return Type  : void
 */
static void averaging(const emxArray_creal32_T *in, double N,
                      emxArray_creal32_T *out)
{
  emxArray_creal32_T *a;
  emxArray_creal32_T *b_m;
  emxArray_real_T *m;
  emxArray_real_T *nv;
  double M;
  double d;
  float ai;
  float ar;
  float br;
  int acoef;
  int bcoef;
  int csz_idx_0;
  int i;
  int i1;
  int loop_ub;
  /*  Perform averaging of columns of 'in' over 'N' samples. 'N' is expected */
  /*  to be odd */
  emxInit_real_T(&m, 2);
  emxInit_creal32_T(&b_m, 2);
  convmtx((int)N, in->size[0], m);
  i = b_m->size[0] * b_m->size[1];
  b_m->size[0] = m->size[0];
  b_m->size[1] = m->size[1];
  emxEnsureCapacity_creal32_T(b_m, i);
  bcoef = m->size[0] * m->size[1];
  for (i = 0; i < bcoef; i++) {
    b_m->data[i].re = (float)m->data[i];
    b_m->data[i].im = 0.0F;
  }
  i = out->size[0];
  out->size[0] = b_m->size[0];
  emxEnsureCapacity_creal32_T(out, i);
  bcoef = b_m->size[0];
  for (i = 0; i < bcoef; i++) {
    out->data[i].re = 0.0F;
    out->data[i].im = 0.0F;
    loop_ub = b_m->size[1];
    for (i1 = 0; i1 < loop_ub; i1++) {
      out->data[i].re += b_m->data[i + b_m->size[0] * i1].re * in->data[i1].re -
                         b_m->data[i + b_m->size[0] * i1].im * in->data[i1].im;
      out->data[i].im += b_m->data[i + b_m->size[0] * i1].re * in->data[i1].im +
                         b_m->data[i + b_m->size[0] * i1].im * in->data[i1].re;
    }
  }
  emxFree_creal32_T(&b_m);
  /*  Retain central rows of output to make the number of rows the same as */
  /*  the input, removing other rows */
  M = (N - 1.0) / 2.0;
  d = (double)out->size[0] - M;
  if (M + 1.0 > d) {
    i = 0;
    i1 = 0;
  } else {
    i = (int)(M + 1.0) - 1;
    i1 = (int)d;
  }
  emxInit_creal32_T(&a, 1);
  bcoef = i1 - i;
  acoef = a->size[0];
  a->size[0] = bcoef;
  emxEnsureCapacity_creal32_T(a, acoef);
  for (acoef = 0; acoef < bcoef; acoef++) {
    a->data[acoef] = out->data[i + acoef];
  }
  emxInit_real_T(&nv, 1);
  /*  Create and apply scaling matrix 'nv' which normalizes by the number */
  /*  of samples averaged in each element of 'out' */
  sum(m, nv);
  d = (double)nv->size[0] - M;
  emxFree_real_T(&m);
  if (M + 1.0 > d) {
    acoef = 0;
    loop_ub = 0;
  } else {
    acoef = (int)(M + 1.0) - 1;
    loop_ub = (int)d;
  }
  bcoef = loop_ub - acoef;
  for (loop_ub = 0; loop_ub < bcoef; loop_ub++) {
    nv->data[loop_ub] = nv->data[acoef + loop_ub];
  }
  acoef = nv->size[0];
  nv->size[0] = bcoef;
  emxEnsureCapacity_real_T(nv, acoef);
  loop_ub = i1 - i;
  if (bcoef < loop_ub) {
    loop_ub = bcoef;
  }
  if (bcoef == 1) {
    csz_idx_0 = i1 - i;
  } else if (i1 - i == 1) {
    csz_idx_0 = bcoef;
  } else if (i1 - i == bcoef) {
    csz_idx_0 = i1 - i;
  } else {
    csz_idx_0 = loop_ub;
  }
  acoef = out->size[0];
  loop_ub = i1 - i;
  if (bcoef < loop_ub) {
    loop_ub = bcoef;
  }
  if (bcoef == 1) {
    out->size[0] = i1 - i;
  } else if (i1 - i == 1) {
    out->size[0] = bcoef;
  } else if (i1 - i == bcoef) {
    out->size[0] = i1 - i;
  } else {
    out->size[0] = loop_ub;
  }
  emxEnsureCapacity_creal32_T(out, acoef);
  if (csz_idx_0 != 0) {
    acoef = (a->size[0] != 1);
    bcoef = (bcoef != 1);
    i = csz_idx_0 - 1;
    for (csz_idx_0 = 0; csz_idx_0 <= i; csz_idx_0++) {
      loop_ub = acoef * csz_idx_0;
      ar = a->data[loop_ub].re;
      ai = a->data[loop_ub].im;
      br = (float)nv->data[bcoef * csz_idx_0];
      if (ai == 0.0F) {
        out->data[csz_idx_0].re = ar / br;
        out->data[csz_idx_0].im = 0.0F;
      } else if (ar == 0.0F) {
        out->data[csz_idx_0].re = 0.0F;
        out->data[csz_idx_0].im = ai / br;
      } else {
        out->data[csz_idx_0].re = ar / br;
        out->data[csz_idx_0].im = ai / br;
      }
    }
  }
  emxFree_creal32_T(&a);
  emxFree_real_T(&nv);
}

/*
 * Convolve each channel of 'in' with an N-by-1 vector of ones
 *
 * Arguments    : const emxArray_creal32_T *in
 *                double N
 *                emxArray_creal32_T *out
 * Return Type  : void
 */
static void b_averaging(const emxArray_creal32_T *in, double N,
                        emxArray_creal32_T *out)
{
  emxArray_creal32_T *a;
  emxArray_real_T c_m_data;
  emxArray_real_T *r;
  emxArray_real_T *r1;
  creal32_T b_m_data[256];
  double m_data[256];
  double nv_data[16];
  double M;
  double d;
  float ai;
  float ar;
  float br;
  int m_size[2];
  int acoef;
  int b_acoef;
  int b_k;
  int csz_idx_0;
  int csz_idx_1;
  int i;
  int i1;
  int k;
  int loop_ub;
  int loop_ub_tmp;
  /*  Perform averaging of columns of 'in' over 'N' samples. 'N' is expected */
  /*  to be odd */
  emxInit_real_T(&r, 2);
  convmtx((int)N, in->size[0], r);
  m_size[0] = r->size[0];
  m_size[1] = r->size[1];
  loop_ub = r->size[0] * r->size[1];
  for (i = 0; i < loop_ub; i++) {
    m_data[i] = r->data[i];
  }
  emxFree_real_T(&r);
  csz_idx_1 = m_size[0];
  csz_idx_0 = m_size[1];
  loop_ub = m_size[0] * m_size[1];
  for (i = 0; i < loop_ub; i++) {
    b_m_data[i].re = (float)m_data[i];
    b_m_data[i].im = 0.0F;
  }
  i = out->size[0] * out->size[1];
  out->size[0] = csz_idx_1;
  out->size[1] = in->size[1];
  emxEnsureCapacity_creal32_T(out, i);
  for (i = 0; i < csz_idx_1; i++) {
    loop_ub = in->size[1];
    for (i1 = 0; i1 < loop_ub; i1++) {
      out->data[i + out->size[0] * i1].re = 0.0F;
      out->data[i + out->size[0] * i1].im = 0.0F;
      for (acoef = 0; acoef < csz_idx_0; acoef++) {
        b_acoef = i + csz_idx_1 * acoef;
        out->data[i + out->size[0] * i1].re +=
            b_m_data[b_acoef].re * in->data[acoef + in->size[0] * i1].re -
            b_m_data[b_acoef].im * in->data[acoef + in->size[0] * i1].im;
        out->data[i + out->size[0] * i1].im +=
            b_m_data[b_acoef].re * in->data[acoef + in->size[0] * i1].im +
            b_m_data[b_acoef].im * in->data[acoef + in->size[0] * i1].re;
      }
    }
  }
  /*  Retain central rows of output to make the number of rows the same as */
  /*  the input, removing other rows */
  M = (N - 1.0) / 2.0;
  d = (double)out->size[0] - M;
  if (M + 1.0 > d) {
    i = -1;
    i1 = -1;
  } else {
    i = (int)(M + 1.0) - 2;
    i1 = (int)d - 1;
  }
  emxInit_creal32_T(&a, 2);
  loop_ub = out->size[1];
  csz_idx_0 = i1 - i;
  i1 = a->size[0] * a->size[1];
  a->size[0] = csz_idx_0;
  a->size[1] = out->size[1];
  emxEnsureCapacity_creal32_T(a, i1);
  for (i1 = 0; i1 < loop_ub; i1++) {
    for (acoef = 0; acoef < csz_idx_0; acoef++) {
      a->data[acoef + a->size[0] * i1] =
          out->data[((i + acoef) + out->size[0] * i1) + 1];
    }
  }
  emxInit_real_T(&r1, 1);
  /*  Create and apply scaling matrix 'nv' which normalizes by the number */
  /*  of samples averaged in each element of 'out' */
  c_m_data.data = &m_data[0];
  c_m_data.size = &m_size[0];
  c_m_data.allocatedSize = 256;
  c_m_data.numDimensions = 2;
  c_m_data.canFreeData = false;
  sum(&c_m_data, r1);
  csz_idx_1 = r1->size[0];
  loop_ub = r1->size[0];
  for (i = 0; i < loop_ub; i++) {
    nv_data[i] = r1->data[i];
  }
  emxFree_real_T(&r1);
  d = (double)csz_idx_1 - M;
  if (M + 1.0 > d) {
    i = 0;
    i1 = 0;
  } else {
    i = (int)(M + 1.0) - 1;
    i1 = (int)d;
  }
  loop_ub_tmp = i1 - i;
  for (i1 = 0; i1 < loop_ub_tmp; i1++) {
    nv_data[i1] = nv_data[i + i1];
  }
  if (loop_ub_tmp != 1) {
    if (csz_idx_0 == 1) {
      csz_idx_0 = loop_ub_tmp;
    } else if ((csz_idx_0 != loop_ub_tmp) && (loop_ub_tmp < csz_idx_0)) {
      csz_idx_0 = loop_ub_tmp;
    }
  }
  i = out->size[1];
  csz_idx_1 = out->size[1];
  i1 = out->size[0] * out->size[1];
  out->size[0] = csz_idx_0;
  out->size[1] = i;
  emxEnsureCapacity_creal32_T(out, i1);
  if ((csz_idx_0 != 0) && (csz_idx_1 != 0)) {
    acoef = (a->size[1] != 1);
    i = csz_idx_1 - 1;
    for (k = 0; k <= i; k++) {
      csz_idx_1 = acoef * k;
      b_acoef = (a->size[0] != 1);
      csz_idx_0 = (loop_ub_tmp != 1);
      i1 = out->size[0] - 1;
      for (b_k = 0; b_k <= i1; b_k++) {
        loop_ub = b_acoef * b_k;
        ar = a->data[loop_ub + a->size[0] * csz_idx_1].re;
        ai = a->data[loop_ub + a->size[0] * csz_idx_1].im;
        br = (float)nv_data[csz_idx_0 * b_k];
        if (ai == 0.0F) {
          out->data[b_k + out->size[0] * k].re = ar / br;
          out->data[b_k + out->size[0] * k].im = 0.0F;
        } else if (ar == 0.0F) {
          out->data[b_k + out->size[0] * k].re = 0.0F;
          out->data[b_k + out->size[0] * k].im = ai / br;
        } else {
          out->data[b_k + out->size[0] * k].re = ar / br;
          out->data[b_k + out->size[0] * k].im = ai / br;
        }
      }
    }
  }
  emxFree_creal32_T(&a);
}

/*
 * Arguments    : double N
 *                const emxArray_real_T *ind
 *                emxArray_real_T *ksubs
 *                emxArray_real_T *nsubs
 * Return Type  : void
 */
static void b_getPortSubscripts(double N, const emxArray_real_T *ind,
                                emxArray_real_T *ksubs, emxArray_real_T *nsubs)
{
  static int v1_data[8736];
  static int varargout_6_data[8736];
  static int vk_data[8736];
  int cpsiz_idx_1;
  int i;
  int loop_ub;
  int v1_size;
  /*  Gets k,n subscripts for K-by-N-by-P grid given indices 'ind' and port */
  /*  'port'. 'thisport' is a logical indexing vector for the port 'port', used
   */
  /*  to extract the corresponding reference symbols */
  cpsiz_idx_1 = 624 * (int)N;
  v1_size = ind->size[0];
  loop_ub = ind->size[0];
  for (i = 0; i < loop_ub; i++) {
    v1_data[i] = (int)ind->data[i] - 1;
  }
  for (i = 0; i < v1_size; i++) {
    vk_data[i] = div_s32(v1_data[i], cpsiz_idx_1);
  }
  for (i = 0; i < v1_size; i++) {
    varargout_6_data[i] = vk_data[i] + 1;
  }
  for (i = 0; i < v1_size; i++) {
    v1_data[i] -= vk_data[i] * cpsiz_idx_1;
  }
  for (i = 0; i < v1_size; i++) {
    vk_data[i] = div_s32(v1_data[i], 624);
  }
  for (i = 0; i < v1_size; i++) {
    v1_data[i] -= vk_data[i] * 624;
  }
  i = ksubs->size[0];
  ksubs->size[0] = v1_size;
  emxEnsureCapacity_real_T(ksubs, i);
  for (i = 0; i < v1_size; i++) {
    ksubs->data[i] = v1_data[i] + 1;
  }
  i = nsubs->size[0];
  nsubs->size[0] = v1_size;
  emxEnsureCapacity_real_T(nsubs, i);
  for (i = 0; i < v1_size; i++) {
    nsubs->data[i] = vk_data[i] + 1;
  }
  v1_size--;
  loop_ub = 0;
  cpsiz_idx_1 = 0;
  for (i = 0; i <= v1_size; i++) {
    if (varargout_6_data[i] == 1) {
      loop_ub++;
      ksubs->data[cpsiz_idx_1] = ksubs->data[i];
      cpsiz_idx_1++;
    }
  }
  i = ksubs->size[0];
  ksubs->size[0] = loop_ub;
  emxEnsureCapacity_real_T(ksubs, i);
  loop_ub = 0;
  cpsiz_idx_1 = 0;
  for (i = 0; i <= v1_size; i++) {
    if (varargout_6_data[i] == 1) {
      loop_ub++;
      nsubs->data[cpsiz_idx_1] = nsubs->data[i];
      cpsiz_idx_1++;
    }
  }
  i = nsubs->size[0];
  nsubs->size[0] = loop_ub;
  emxEnsureCapacity_real_T(nsubs, i);
}

/*
 * Arguments    : const emxArray_real_T *k0
 *                double l0_data[]
 *                int l0_size[2]
 *                emxArray_creal32_T *H0
 *                const emxArray_real_T *k
 *                const double n_data[]
 *                const int n_size[2]
 *                emxArray_creal32_T *H
 * Return Type  : void
 */
static void b_interpolate(const emxArray_real_T *k0, double l0_data[],
                          int l0_size[2], emxArray_creal32_T *H0,
                          const emxArray_real_T *k, const double n_data[],
                          const int n_size[2], emxArray_creal32_T *H)
{
  emxArray_creal32_T *b_varargin_1;
  emxArray_creal32_T *varargin_1;
  emxArray_real_T *kg;
  emxArray_real_T *lg;
  emxArray_real_T *v;
  emxArray_real_T *xq;
  emxArray_real_T *yq;
  creal32_T H0_data[17];
  double b_n_data[16];
  double n;
  int H0_size[2];
  int b_n_size[2];
  int b_n;
  int i;
  int loop_ub;
  int x_size_idx_1;
  signed char input_sizes_idx_1;
  signed char sizes_idx_1;
  boolean_T x_data[15];
  boolean_T empty_non_axis_sizes;
  boolean_T exitg1;
  /*  Interpolate channel estimate 'H0' defined for subcarrier subscripts 'k0'
   */
  /*  and OFDM symbol subscripts 'l0', using 'method' as the interpolation */
  /*  method. 'nd' is the number of dimensions over which interpolation is */
  /*  performed. 'polar' determines whether interpolation is performed using */
  /*  polar (true) or Cartesian (false) co-ordinates. 'H' contains the */
  /*  interpolated channel estimate for subcarrier subscripts 'k' and OFDM */
  /*  symbol subscripts 'n' */
  /*  nd==2 */
  /*  For code generation, interp2 is used for 2-D interpolation. */
  /*  Unlike griddedInterpolant, interp2 does not perform */
  /*  extrapolation. Therefore, explicit extrapolation is performed */
  /*  here for the first and last OFDM symbol, unless they already */
  /*  appear in 'l0' */
  x_size_idx_1 = l0_size[1];
  n = n_data[0];
  loop_ub = l0_size[1];
  for (i = 0; i < loop_ub; i++) {
    x_data[i] = (n == l0_data[i]);
  }
  empty_non_axis_sizes = false;
  b_n = 1;
  exitg1 = false;
  while ((!exitg1) && (b_n <= x_size_idx_1)) {
    if (!x_data[b_n - 1]) {
      b_n++;
    } else {
      empty_non_axis_sizes = true;
      exitg1 = true;
    }
  }
  emxInit_creal32_T(&varargin_1, 1);
  if (!empty_non_axis_sizes) {
    /*  Extrapolates channel estimate 'H0' in Cartesian co-ordinates. Each row
     * of */
    /*  'H' contains the extrapolated channel estimate for OFDM subscript 'l0',
     */
    /*  with the corresponding row of 'H0' containing estimates for OFDM symbol
     */
    /*  subscripts 'l0'. 'method' is the extrapolation method */
    i = varargin_1->size[0];
    varargin_1->size[0] = H0->size[0];
    emxEnsureCapacity_creal32_T(varargin_1, i);
    loop_ub = H0->size[0];
    for (i = 0; i < loop_ub; i++) {
      varargin_1->data[i].re = 0.0F;
      varargin_1->data[i].im = 0.0F;
    }
    i = H0->size[0];
    if (0 <= i - 1) {
      H0_size[0] = 1;
    }
    for (b_n = 0; b_n < i; b_n++) {
      loop_ub = H0->size[1];
      H0_size[1] = H0->size[1];
      for (x_size_idx_1 = 0; x_size_idx_1 < loop_ub; x_size_idx_1++) {
        H0_data[x_size_idx_1] = H0->data[b_n + H0->size[0] * x_size_idx_1];
      }
      varargin_1->data[b_n] =
          c_interp1(l0_data, l0_size, H0_data, H0_size, n_data[0]);
    }
    if (varargin_1->size[0] != 0) {
      b_n = varargin_1->size[0];
    } else if ((H0->size[0] != 0) && (H0->size[1] != 0)) {
      b_n = H0->size[0];
    } else {
      b_n = 0;
      if (H0->size[0] > 0) {
        b_n = H0->size[0];
      }
    }
    empty_non_axis_sizes = (b_n == 0);
    if (empty_non_axis_sizes || (varargin_1->size[0] != 0)) {
      input_sizes_idx_1 = 1;
    } else {
      input_sizes_idx_1 = 0;
    }
    if (empty_non_axis_sizes || ((H0->size[0] != 0) && (H0->size[1] != 0))) {
      sizes_idx_1 = (signed char)H0->size[1];
    } else {
      sizes_idx_1 = 0;
    }
    emxInit_creal32_T(&b_varargin_1, 2);
    i = b_varargin_1->size[0] * b_varargin_1->size[1];
    b_varargin_1->size[0] = b_n;
    b_varargin_1->size[1] = input_sizes_idx_1 + sizes_idx_1;
    emxEnsureCapacity_creal32_T(b_varargin_1, i);
    loop_ub = input_sizes_idx_1;
    for (i = 0; i < loop_ub; i++) {
      for (x_size_idx_1 = 0; x_size_idx_1 < b_n; x_size_idx_1++) {
        b_varargin_1->data[x_size_idx_1] = varargin_1->data[x_size_idx_1];
      }
    }
    loop_ub = sizes_idx_1;
    for (i = 0; i < loop_ub; i++) {
      for (x_size_idx_1 = 0; x_size_idx_1 < b_n; x_size_idx_1++) {
        b_varargin_1->data[x_size_idx_1 +
                           b_varargin_1->size[0] * (i + input_sizes_idx_1)] =
            H0->data[x_size_idx_1 + b_n * i];
      }
    }
    i = H0->size[0] * H0->size[1];
    H0->size[0] = b_varargin_1->size[0];
    H0->size[1] = b_varargin_1->size[1];
    emxEnsureCapacity_creal32_T(H0, i);
    loop_ub = b_varargin_1->size[0] * b_varargin_1->size[1];
    for (i = 0; i < loop_ub; i++) {
      H0->data[i] = b_varargin_1->data[i];
    }
    emxFree_creal32_T(&b_varargin_1);
    b_n_size[1] = l0_size[1] + 1;
    b_n_data[0] = n_data[0];
    loop_ub = l0_size[1];
    if (0 <= loop_ub - 1) {
      memcpy(&b_n_data[1], &l0_data[0], loop_ub * sizeof(double));
    }
    l0_size[0] = 1;
    l0_size[1]++;
    loop_ub = b_n_size[1];
    if (0 <= loop_ub - 1) {
      memcpy(&l0_data[0], &b_n_data[0], loop_ub * sizeof(double));
    }
  }
  x_size_idx_1 = l0_size[1];
  n = n_data[n_size[1] - 1];
  loop_ub = l0_size[1];
  for (i = 0; i < loop_ub; i++) {
    x_data[i] = (n == l0_data[i]);
  }
  empty_non_axis_sizes = false;
  b_n = 1;
  exitg1 = false;
  while ((!exitg1) && (b_n <= x_size_idx_1)) {
    if (!x_data[b_n - 1]) {
      b_n++;
    } else {
      empty_non_axis_sizes = true;
      exitg1 = true;
    }
  }
  if (!empty_non_axis_sizes) {
    /*  Extrapolates channel estimate 'H0' in Cartesian co-ordinates. Each row
     * of */
    /*  'H' contains the extrapolated channel estimate for OFDM subscript 'l0',
     */
    /*  with the corresponding row of 'H0' containing estimates for OFDM symbol
     */
    /*  subscripts 'l0'. 'method' is the extrapolation method */
    i = varargin_1->size[0];
    varargin_1->size[0] = H0->size[0];
    emxEnsureCapacity_creal32_T(varargin_1, i);
    loop_ub = H0->size[0];
    for (i = 0; i < loop_ub; i++) {
      varargin_1->data[i].re = 0.0F;
      varargin_1->data[i].im = 0.0F;
    }
    i = H0->size[0];
    if (0 <= i - 1) {
      H0_size[0] = 1;
    }
    for (b_n = 0; b_n < i; b_n++) {
      loop_ub = H0->size[1];
      H0_size[1] = H0->size[1];
      for (x_size_idx_1 = 0; x_size_idx_1 < loop_ub; x_size_idx_1++) {
        H0_data[x_size_idx_1] = H0->data[b_n + H0->size[0] * x_size_idx_1];
      }
      varargin_1->data[b_n] = c_interp1(l0_data, l0_size, H0_data, H0_size, n);
    }
    if ((H0->size[0] != 0) && (H0->size[1] != 0)) {
      b_n = H0->size[0];
    } else if (varargin_1->size[0] != 0) {
      b_n = varargin_1->size[0];
    } else {
      b_n = H0->size[0];
    }
    empty_non_axis_sizes = (b_n == 0);
    if (empty_non_axis_sizes || ((H0->size[0] != 0) && (H0->size[1] != 0))) {
      input_sizes_idx_1 = (signed char)H0->size[1];
    } else {
      input_sizes_idx_1 = 0;
    }
    if (empty_non_axis_sizes || (varargin_1->size[0] != 0)) {
      sizes_idx_1 = 1;
    } else {
      sizes_idx_1 = 0;
    }
    loop_ub = input_sizes_idx_1;
    for (i = 0; i < loop_ub; i++) {
      for (x_size_idx_1 = 0; x_size_idx_1 < b_n; x_size_idx_1++) {
        H0->data[x_size_idx_1 + H0->size[0] * i] =
            H0->data[x_size_idx_1 + b_n * i];
      }
    }
    i = H0->size[0] * H0->size[1];
    H0->size[0] = b_n;
    H0->size[1] = input_sizes_idx_1 + sizes_idx_1;
    emxEnsureCapacity_creal32_T(H0, i);
    loop_ub = sizes_idx_1;
    for (i = 0; i < loop_ub; i++) {
      for (x_size_idx_1 = 0; x_size_idx_1 < b_n; x_size_idx_1++) {
        H0->data[x_size_idx_1 + H0->size[0] * input_sizes_idx_1] =
            varargin_1->data[x_size_idx_1];
      }
    }
    i = l0_size[1];
    l0_size[1]++;
    l0_data[i] = n;
  }
  emxFree_creal32_T(&varargin_1);
  emxInit_real_T(&lg, 2);
  emxInit_real_T(&kg, 2);
  emxInit_real_T(&yq, 2);
  emxInit_real_T(&xq, 2);
  meshgrid(l0_data, l0_size, k0, lg, kg);
  /*  For code generation, perform 2-D Cartesian interpolation. */
  /*  This relies on the (accurate) assumption that interpolate() */
  /*  is only called with polar=false for nd==2. The interp2 call */
  /*  requires to be here rather than inside cartInterpolate in */
  /*  order that coder does not attempt to generate code for */
  /*  interp2 inside a call to createVPs, where input 'n' is scalar */
  /*  and consequently fails size checks performed by interp2 */
  meshgrid(n_data, n_size, k, yq, xq);
  b_n = lg->size[1];
  b_n_size[0] = 1;
  b_n_size[1] = lg->size[1];
  for (x_size_idx_1 = 0; x_size_idx_1 < b_n; x_size_idx_1++) {
    b_n_data[x_size_idx_1] = lg->data[lg->size[0] * x_size_idx_1];
  }
  emxFree_real_T(&lg);
  emxInit_real_T(&v, 2);
  b_n = kg->size[0];
  i = v->size[0] * v->size[1];
  v->size[0] = 1;
  v->size[1] = kg->size[0];
  emxEnsureCapacity_real_T(v, i);
  for (x_size_idx_1 = 0; x_size_idx_1 < b_n; x_size_idx_1++) {
    v->data[x_size_idx_1] = kg->data[x_size_idx_1];
  }
  emxFree_real_T(&kg);
  interp2_local(H0, yq, xq, b_n_data, b_n_size, v, H);
  emxFree_real_T(&v);
  emxFree_real_T(&xq);
  emxFree_real_T(&yq);
}

/*
 * Arguments    : double nd
 *                const emxArray_real_T *vpi
 *                const double siz[4]
 *                double n
 *                emxArray_real_T *ind
 * Return Type  : void
 */
static void calculateVPIndices(double nd, const emxArray_real_T *vpi,
                               const double siz[4], double n,
                               emxArray_real_T *ind)
{
  emxArray_real_T *b_ind;
  emxArray_real_T *points;
  emxArray_real_T *y;
  int acoef;
  int i;
  int i1;
  int result;
  short outsize_idx_1;
  signed char input_sizes_idx_1;
  boolean_T empty_non_axis_sizes;
  /*  Calculate indices for VPs in the channel estimate across all receive */
  /*  antennas, given number of dimensions 'nd', VP interpolator 'vpi', channel
   */
  /*  estimate size vector 'siz' and port 'p'. For nd=0 or 1, 'n' additionally
   */
  /*  specifies the OFDM symbol index */
  emxInit_real_T(&points, 2);
  if (nd == 2.0) {
    i = points->size[0] * points->size[1];
    points->size[0] = vpi->size[0];
    points->size[1] = vpi->size[1];
    emxEnsureCapacity_real_T(points, i);
    acoef = vpi->size[0] * vpi->size[1];
    for (i = 0; i < acoef; i++) {
      points->data[i] = vpi->data[i];
    }
  } else if (nd == 1.0) {
    outsize_idx_1 = (short)vpi->size[1];
    if ((vpi->size[0] != 0) && (vpi->size[1] != 0)) {
      result = vpi->size[0];
    } else if (((short)vpi->size[0] != 0) && ((short)vpi->size[1] != 0)) {
      result = (short)vpi->size[0];
    } else {
      result = vpi->size[0];
      if ((short)vpi->size[0] > vpi->size[0]) {
        result = (short)vpi->size[0];
      }
    }
    empty_non_axis_sizes = (result == 0);
    if (empty_non_axis_sizes || ((vpi->size[0] != 0) && (vpi->size[1] != 0))) {
      input_sizes_idx_1 = (signed char)vpi->size[1];
    } else {
      input_sizes_idx_1 = 0;
    }
    if ((!empty_non_axis_sizes) &&
        (((short)vpi->size[0] == 0) || ((short)vpi->size[1] == 0))) {
      outsize_idx_1 = 0;
    }
    i = points->size[0] * points->size[1];
    points->size[0] = result;
    points->size[1] = input_sizes_idx_1 + outsize_idx_1;
    emxEnsureCapacity_real_T(points, i);
    acoef = input_sizes_idx_1;
    for (i = 0; i < acoef; i++) {
      for (i1 = 0; i1 < result; i1++) {
        points->data[i1 + points->size[0] * i] = vpi->data[i1 + result * i];
      }
    }
    acoef = outsize_idx_1;
    for (i = 0; i < acoef; i++) {
      for (i1 = 0; i1 < result; i1++) {
        points->data[i1 + points->size[0] * (i + input_sizes_idx_1)] = n;
      }
    }
  } else {
    /*  nd==0 */
    if ((vpi->size[0] != 0) && (vpi->size[1] != 0)) {
      input_sizes_idx_1 = (signed char)vpi->size[1];
    } else {
      input_sizes_idx_1 = 0;
    }
    i = points->size[0] * points->size[1];
    points->size[0] = 1;
    points->size[1] = input_sizes_idx_1 + 1;
    emxEnsureCapacity_real_T(points, i);
    acoef = input_sizes_idx_1;
    for (i = 0; i < acoef; i++) {
      points->data[points->size[0] * i] = vpi->data[i];
    }
    points->data[points->size[0] * input_sizes_idx_1] = n;
  }
  emxInit_real_T(&b_ind, 1);
  acoef = points->size[0];
  i = b_ind->size[0];
  b_ind->size[0] = points->size[0];
  emxEnsureCapacity_real_T(b_ind, i);
  for (i = 0; i < acoef; i++) {
    b_ind->data[i] =
        points->data[i] + (points->data[i + points->size[0]] - 1.0) * 672.0;
  }
  emxFree_real_T(&points);
  emxInit_real_T(&y, 2);
  i = y->size[0] * y->size[1];
  y->size[0] = 1;
  y->size[1] = 1;
  emxEnsureCapacity_real_T(y, i);
  y->data[0] = 0.0 * siz[1];
  i = ind->size[0] * ind->size[1];
  ind->size[0] = (short)b_ind->size[0];
  ind->size[1] = 1;
  emxEnsureCapacity_real_T(ind, i);
  if ((short)b_ind->size[0] != 0) {
    acoef = (b_ind->size[0] != 1);
    i = (short)b_ind->size[0] - 1;
    for (result = 0; result <= i; result++) {
      ind->data[result] = b_ind->data[acoef * result] + y->data[0];
    }
  }
  emxFree_real_T(&y);
  emxFree_real_T(&b_ind);
}

/*
 * If the specified method is 'VP', this indicates that the
 *  interpolation is being performed for the virtual pilots. In this
 *  case, the channel estimate 'H0' is pre-processed to give a best-fit
 *  linear interpolation (to remove noise) prior to linearly
 *  extrapolating to calculate the VPs
 *
 * Arguments    : double nd
 *                const emxArray_real_T *ipt
 *                const emxArray_real_T *k
 *                double n
 *                float H0_data[]
 *                int H0_size
 *                emxArray_real32_T *H
 * Return Type  : void
 */
static void cartInterpolate(double nd, const emxArray_real_T *ipt,
                            const emxArray_real_T *k, double n, float H0_data[],
                            int H0_size, emxArray_real32_T *H)
{
  static float varargin_2_data[8736];
  static int i_data[8737];
  static int tmp_data[8737];
  static int yn_data[8736];
  emxArray_real32_T *b_H;
  emxArray_real32_T *x;
  emxArray_real_T *b_k;
  emxArray_real_T *b_x;
  double xtmp;
  float p[2];
  float tmp;
  int c_k;
  int exitg1;
  int i_size;
  int loop_ub;
  int ntilerows;
  int nx;
  boolean_T ipt_data[8736];
  /*  Interpolates channel estimate 'H0' in Cartesian co-ordinates (or real */
  /*  values), using interpolant 'ipt'. 'H' contains the interpolated channel */
  /*  estimate for subcarrier subscripts 'k' and OFDM symbol subscripts 'n'. */
  /*  'nd' is the number of dimensions over which the interpolation is */
  /*  performed and 'method' is the interpolation method */
  /*  Perform interpolation */
  emxInit_real32_T(&b_H, 1);
  emxInit_real_T(&b_k, 1);
  emxInit_real32_T(&x, 1);
  if (nd == 1.0) {
    /*  Pre-process VPs if applicable */
    /*  Perform a linear fit of the channel estimate 'H' whose subcarrier */
    /*  subscripts are 'k' */
    if (ipt->size[0] > 1) {
      loop_ub = ipt->size[0];
      c_k = b_k->size[0];
      b_k->size[0] = ipt->size[0];
      emxEnsureCapacity_real_T(b_k, c_k);
      for (c_k = 0; c_k < loop_ub; c_k++) {
        b_k->data[c_k] = ipt->data[c_k];
      }
      polyfit(b_k, H0_data, H0_size, p);
      loop_ub = ipt->size[0];
      for (c_k = 0; c_k < loop_ub; c_k++) {
        H0_data[c_k] = p[0];
      }
      loop_ub = ipt->size[0];
      H0_size = ipt->size[0];
      for (c_k = 0; c_k < loop_ub; c_k++) {
        H0_data[c_k] = (float)ipt->data[c_k] * H0_data[c_k] + p[1];
      }
    }
    /*  Perform 1-D interpolation using interp1 */
    if (0 <= H0_size - 1) {
      memcpy(&varargin_2_data[0], &H0_data[0], H0_size * sizeof(float));
    }
    emxInit_real_T(&b_x, 2);
    c_k = b_x->size[0] * b_x->size[1];
    b_x->size[0] = ipt->size[0];
    b_x->size[1] = ipt->size[1];
    emxEnsureCapacity_real_T(b_x, c_k);
    loop_ub = ipt->size[0] * ipt->size[1];
    for (c_k = 0; c_k < loop_ub; c_k++) {
      b_x->data[c_k] = ipt->data[c_k];
    }
    nx = ipt->size[0] * ipt->size[1] - 1;
    c_k = b_H->size[0];
    b_H->size[0] = k->size[0];
    emxEnsureCapacity_real32_T(b_H, c_k);
    loop_ub = k->size[0];
    for (c_k = 0; c_k < loop_ub; c_k++) {
      b_H->data[c_k] = 0.0F;
    }
    if (k->size[0] != 0) {
      c_k = 0;
      do {
        exitg1 = 0;
        if (c_k <= nx) {
          if (rtIsNaN(ipt->data[c_k])) {
            exitg1 = 1;
          } else {
            c_k++;
          }
        } else {
          if (ipt->data[1] < ipt->data[0]) {
            c_k = (nx + 1) >> 1;
            for (loop_ub = 0; loop_ub < c_k; loop_ub++) {
              xtmp = b_x->data[loop_ub];
              ntilerows = nx - loop_ub;
              b_x->data[loop_ub] = b_x->data[ntilerows];
              b_x->data[ntilerows] = xtmp;
            }
            c_k = x->size[0];
            x->size[0] = H0_size;
            emxEnsureCapacity_real32_T(x, c_k);
            for (c_k = 0; c_k < H0_size; c_k++) {
              x->data[c_k] = H0_data[c_k];
            }
            if ((H0_size != 0) && (H0_size > 1)) {
              ntilerows = H0_size >> 1;
              for (c_k = 0; c_k < ntilerows; c_k++) {
                tmp = x->data[c_k];
                loop_ub = (H0_size - c_k) - 1;
                x->data[c_k] = x->data[loop_ub];
                x->data[loop_ub] = tmp;
              }
            }
            loop_ub = x->size[0];
            for (c_k = 0; c_k < loop_ub; c_k++) {
              varargin_2_data[c_k] = x->data[c_k];
            }
          }
          interp1Linear(varargin_2_data, H0_size, k, b_H, b_x);
          exitg1 = 1;
        }
      } while (exitg1 == 0);
    }
    emxFree_real_T(&b_x);
    c_k = H->size[0] * H->size[1];
    H->size[0] = b_H->size[0];
    H->size[1] = 1;
    emxEnsureCapacity_real32_T(H, c_k);
    loop_ub = b_H->size[0];
    for (c_k = 0; c_k < loop_ub; c_k++) {
      H->data[c_k] = b_H->data[c_k];
    }
  } else {
    /*  nd==2 */
    /*  Perform 2-D scattered interpolation */
    /*  'ipt' contains a sample points array for scattered */
    /*  interpolation */
    /*  Find the locations 'yn' of estimates for the current OFDM */
    /*  symbol 'n' */
    loop_ub = ipt->size[0];
    ntilerows = ipt->size[0];
    for (c_k = 0; c_k < loop_ub; c_k++) {
      ipt_data[c_k] = (ipt->data[c_k + ipt->size[0]] == n);
    }
    b_eml_find(ipt_data, ntilerows, tmp_data, &nx);
    i_size = nx;
    if (0 <= nx - 1) {
      memcpy(&i_data[0], &tmp_data[0], nx * sizeof(int));
    }
    loop_ub = ipt->size[0];
    ntilerows = ipt->size[0];
    for (c_k = 0; c_k < loop_ub; c_k++) {
      ipt_data[c_k] = (ipt->data[c_k + ipt->size[0]] == n);
    }
    b_eml_find(ipt_data, ntilerows, tmp_data, &nx);
    if (0 <= nx - 1) {
      memcpy(&yn_data[0], &tmp_data[0], nx * sizeof(int));
    }
    /*  Pre-process VPs if applicable */
    c_k = b_k->size[0];
    b_k->size[0] = i_size;
    emxEnsureCapacity_real_T(b_k, c_k);
    for (c_k = 0; c_k < i_size; c_k++) {
      b_k->data[c_k] = ipt->data[i_data[c_k] - 1];
    }
    ntilerows = i_size;
    for (c_k = 0; c_k < i_size; c_k++) {
      varargin_2_data[c_k] = H0_data[i_data[c_k] - 1];
    }
    /*  Perform a linear fit of the channel estimate 'H' whose subcarrier */
    /*  subscripts are 'k' */
    if (nx > 1) {
      polyfit(b_k, varargin_2_data, i_size, p);
      if (i_size != 0) {
        for (c_k = 0; c_k < i_size; c_k++) {
          varargin_2_data[c_k] = p[0];
        }
        ntilerows = b_k->size[0];
        loop_ub = b_k->size[0];
        for (c_k = 0; c_k < loop_ub; c_k++) {
          varargin_2_data[c_k] =
              (float)b_k->data[c_k] * varargin_2_data[c_k] + p[1];
        }
      }
    }
    for (c_k = 0; c_k < ntilerows; c_k++) {
      H0_data[yn_data[c_k] - 1] = varargin_2_data[c_k];
    }
    /*  If there are multiple estimates for the current OFDM */
    /*  symbol */
    if (i_size != 1) {
      /*  Interpolate the estimates for each subscript in 'k' */
      c_k = b_k->size[0];
      b_k->size[0] = i_size;
      emxEnsureCapacity_real_T(b_k, c_k);
      for (c_k = 0; c_k < i_size; c_k++) {
        b_k->data[c_k] = ipt->data[i_data[c_k] - 1];
      }
      for (c_k = 0; c_k < i_size; c_k++) {
        varargin_2_data[c_k] = H0_data[i_data[c_k] - 1];
      }
      c_k = b_H->size[0];
      b_H->size[0] = k->size[0];
      emxEnsureCapacity_real32_T(b_H, c_k);
      loop_ub = k->size[0];
      for (c_k = 0; c_k < loop_ub; c_k++) {
        b_H->data[c_k] = 0.0F;
      }
      if (k->size[0] != 0) {
        c_k = 0;
        do {
          exitg1 = 0;
          if (c_k <= nx - 1) {
            if (rtIsNaN(ipt->data[yn_data[c_k] - 1])) {
              exitg1 = 1;
            } else {
              c_k++;
            }
          } else {
            if (ipt->data[yn_data[1] - 1] < ipt->data[yn_data[0] - 1]) {
              c_k = nx >> 1;
              for (loop_ub = 0; loop_ub < c_k; loop_ub++) {
                xtmp = b_k->data[loop_ub];
                ntilerows = (nx - loop_ub) - 1;
                b_k->data[loop_ub] = b_k->data[ntilerows];
                b_k->data[ntilerows] = xtmp;
              }
              c_k = x->size[0];
              x->size[0] = i_size;
              emxEnsureCapacity_real32_T(x, c_k);
              for (c_k = 0; c_k < i_size; c_k++) {
                x->data[c_k] = varargin_2_data[c_k];
              }
              if ((i_size != 0) && (i_size > 1)) {
                ntilerows = i_size >> 1;
                for (c_k = 0; c_k < ntilerows; c_k++) {
                  tmp = x->data[c_k];
                  loop_ub = (i_size - c_k) - 1;
                  x->data[c_k] = x->data[loop_ub];
                  x->data[loop_ub] = tmp;
                }
              }
              loop_ub = x->size[0];
              for (c_k = 0; c_k < loop_ub; c_k++) {
                varargin_2_data[c_k] = x->data[c_k];
              }
            }
            b_interp1Linear(varargin_2_data, nx, k, b_H, b_k);
            exitg1 = 1;
          }
        } while (exitg1 == 0);
      }
      c_k = H->size[0] * H->size[1];
      H->size[0] = b_H->size[0];
      H->size[1] = 1;
      emxEnsureCapacity_real32_T(H, c_k);
      loop_ub = b_H->size[0];
      for (c_k = 0; c_k < loop_ub; c_k++) {
        H->data[c_k] = b_H->data[c_k];
      }
    } else {
      /*  Repeat the single estimate for all subscripts 'k' */
      c_k = H->size[0] * H->size[1];
      H->size[0] = k->size[0];
      H->size[1] = 1;
      emxEnsureCapacity_real32_T(H, c_k);
      ntilerows = k->size[0];
      for (loop_ub = 0; loop_ub < ntilerows; loop_ub++) {
        H->data[loop_ub] = H0_data[yn_data[0] - 1];
      }
    }
  }
  emxFree_real32_T(&x);
  emxFree_real_T(&b_k);
  emxFree_real32_T(&b_H);
}

/*
 * Determine the set of unique frequency subscripts
 *
 * Arguments    : const double kedge_data[]
 *                int kedge_size
 *                emxArray_real_T *ksubs
 *                const emxArray_real_T *lsubs
 *                const double range[2]
 *                emxArray_cell_wrap_8 *vpis
 *                emxArray_real_T *uk
 *                emxArray_real_T *nd
 * Return Type  : void
 */
static void createVPInterpolators(const double kedge_data[], int kedge_size,
                                  emxArray_real_T *ksubs,
                                  const emxArray_real_T *lsubs,
                                  const double range[2],
                                  emxArray_cell_wrap_8 *vpis,
                                  emxArray_real_T *uk, emxArray_real_T *nd)
{
  emxArray_real_T c_kedge_data;
  emxArray_real_T d_kedge_data;
  emxArray_real_T *b;
  emxArray_real_T *b_b;
  emxArray_real_T *kvp;
  emxArray_real_T *lvp;
  double b_kedge_data[14];
  double kend;
  double kstart;
  int b_i;
  int b_kedge_size;
  int end_tmp;
  int i;
  int partialTrueCount;
  int size_tmp_idx_0;
  int trueCount;
  signed char b_tmp_data[14];
  signed char tmp_data[14];
  boolean_T c_tmp_data[8736];
  boolean_T d_tmp_data[8736];
  boolean_T multiSubcarrier;
  /*  -------------------------------------------------------------------------
   */
  /*  local functions */
  /*  -------------------------------------------------------------------------
   */
  /*  Create interpolators used to create virtual pilots. 'vpis' is the set of
   */
  /*  interpolators with each element corresponding to the unique frequency */
  /*  subscript in the corresponding element of 'uk'. 'nd' is the number of */
  /*  dimensions over which each interpolator operates (0, 1 or 2) */
  end_tmp = kedge_size - 1;
  trueCount = 0;
  partialTrueCount = 0;
  for (i = 0; i <= end_tmp; i++) {
    if (kedge_data[i] != 0.0) {
      trueCount++;
      tmp_data[partialTrueCount] = (signed char)(i + 1);
      partialTrueCount++;
    }
  }
  b_kedge_size = trueCount;
  for (b_i = 0; b_i < trueCount; b_i++) {
    b_kedge_data[b_i] = kedge_data[tmp_data[b_i] - 1];
  }
  emxInit_real_T(&b, 1);
  c_kedge_data.data = &b_kedge_data[0];
  c_kedge_data.size = &b_kedge_size;
  c_kedge_data.allocatedSize = 14;
  c_kedge_data.numDimensions = 1;
  c_kedge_data.canFreeData = false;
  unique_vector(&c_kedge_data, b);
  size_tmp_idx_0 = b->size[0];
  trueCount = 0;
  partialTrueCount = 0;
  for (i = 0; i <= end_tmp; i++) {
    if (kedge_data[i] != 0.0) {
      trueCount++;
      b_tmp_data[partialTrueCount] = (signed char)(i + 1);
      partialTrueCount++;
    }
  }
  b_kedge_size = trueCount;
  for (b_i = 0; b_i < trueCount; b_i++) {
    b_kedge_data[b_i] = kedge_data[b_tmp_data[b_i] - 1];
  }
  emxInit_real_T(&b_b, 2);
  d_kedge_data.data = &b_kedge_data[0];
  d_kedge_data.size = &b_kedge_size;
  d_kedge_data.allocatedSize = 14;
  d_kedge_data.numDimensions = 1;
  d_kedge_data.canFreeData = false;
  unique_vector(&d_kedge_data, b);
  b_i = b_b->size[0] * b_b->size[1];
  b_b->size[0] = 1;
  b_b->size[1] = b->size[0];
  emxEnsureCapacity_real_T(b_b, b_i);
  end_tmp = b->size[0];
  for (b_i = 0; b_i < end_tmp; b_i++) {
    b_b->data[b_i] = b->data[b_i];
  }
  b_i = uk->size[0] * uk->size[1];
  uk->size[0] = 1;
  uk->size[1] = size_tmp_idx_0;
  emxEnsureCapacity_real_T(uk, b_i);
  for (b_i = 0; b_i < size_tmp_idx_0; b_i++) {
    uk->data[b_i] = b_b->data[b_i];
  }
  emxFree_real_T(&b_b);
  /*  Take extra subcarriers into account by adjusting reference symbol */
  /*  frequency subscripts */
  end_tmp = ksubs->size[0];
  for (b_i = 0; b_i < end_tmp; b_i++) {
    ksubs->data[b_i] += 24.0;
  }
  /*  For each unique frequency subscript */
  b_i = vpis->size[0] * vpis->size[1];
  vpis->size[0] = 1;
  vpis->size[1] = uk->size[1];
  emxEnsureCapacity_cell_wrap_8(vpis, b_i);
  b_i = nd->size[0] * nd->size[1];
  nd->size[0] = 1;
  nd->size[1] = (short)uk->size[1];
  emxEnsureCapacity_real_T(nd, b_i);
  end_tmp = (short)uk->size[1];
  for (b_i = 0; b_i < end_tmp; b_i++) {
    nd->data[b_i] = 0.0;
  }
  b_i = uk->size[1];
  emxInit_real_T(&kvp, 1);
  emxInit_real_T(&lvp, 1);
  for (i = 0; i < b_i; i++) {
    /*  Determine the start and end frequency subscripts of reference */
    /*  signals which will contribute to the interpolation performed by */
    /*  this interpolator */
    kend = uk->data[i];
    kstart = fmax(kend + range[0], 1.0);
    kend = fmin(kend + range[1], 672.0);
    /*  Determine the actual reference signal subcarrier and OFDM symbol */
    /*  subscripts ('kvp' and 'lvp') within those limits */
    end_tmp = ksubs->size[0];
    partialTrueCount = ksubs->size[0];
    for (size_tmp_idx_0 = 0; size_tmp_idx_0 < end_tmp; size_tmp_idx_0++) {
      c_tmp_data[size_tmp_idx_0] = (ksubs->data[size_tmp_idx_0] >= kstart);
      d_tmp_data[size_tmp_idx_0] = (ksubs->data[size_tmp_idx_0] <= kend);
    }
    end_tmp = partialTrueCount - 1;
    trueCount = 0;
    for (size_tmp_idx_0 = 0; size_tmp_idx_0 <= end_tmp; size_tmp_idx_0++) {
      if (c_tmp_data[size_tmp_idx_0] && d_tmp_data[size_tmp_idx_0]) {
        trueCount++;
      }
    }
    size_tmp_idx_0 = kvp->size[0];
    kvp->size[0] = trueCount;
    emxEnsureCapacity_real_T(kvp, size_tmp_idx_0);
    partialTrueCount = 0;
    for (size_tmp_idx_0 = 0; size_tmp_idx_0 <= end_tmp; size_tmp_idx_0++) {
      if (c_tmp_data[size_tmp_idx_0] && d_tmp_data[size_tmp_idx_0]) {
        kvp->data[partialTrueCount] = ksubs->data[size_tmp_idx_0];
        partialTrueCount++;
      }
    }
    trueCount = 0;
    for (size_tmp_idx_0 = 0; size_tmp_idx_0 <= end_tmp; size_tmp_idx_0++) {
      if (c_tmp_data[size_tmp_idx_0] && d_tmp_data[size_tmp_idx_0]) {
        trueCount++;
      }
    }
    size_tmp_idx_0 = lvp->size[0];
    lvp->size[0] = trueCount;
    emxEnsureCapacity_real_T(lvp, size_tmp_idx_0);
    partialTrueCount = 0;
    for (size_tmp_idx_0 = 0; size_tmp_idx_0 <= end_tmp; size_tmp_idx_0++) {
      if (c_tmp_data[size_tmp_idx_0] && d_tmp_data[size_tmp_idx_0]) {
        lvp->data[partialTrueCount] = lsubs->data[size_tmp_idx_0];
        partialTrueCount++;
      }
    }
    /*  If there are multiple subcarriers and/or multiple OFDM symbols, */
    /*  construct the interpolator. Values are set to zero and will be */
    /*  provided per receive antenna when performing interpolation. For */
    /*  cases with a single subcarrier and a single OFDM symbol, */
    /*  interpolation is not performed */
    unique_vector(kvp, b);
    multiSubcarrier = (b->size[0] > 1);
    unique_vector(lvp, b);
    if (multiSubcarrier && (b->size[0] > 1)) {
      nd->data[i] = 2.0;
      /*  2-D interpolation, record subcarrier and OFDM symbol */
      /*  subscripts */
      end_tmp = kvp->size[0];
      size_tmp_idx_0 = vpis->data[i].f1->size[0] * vpis->data[i].f1->size[1];
      vpis->data[i].f1->size[0] = kvp->size[0];
      vpis->data[i].f1->size[1] = 2;
      emxEnsureCapacity_real_T(vpis->data[i].f1, size_tmp_idx_0);
      for (size_tmp_idx_0 = 0; size_tmp_idx_0 < end_tmp; size_tmp_idx_0++) {
        vpis->data[i].f1->data[size_tmp_idx_0] = kvp->data[size_tmp_idx_0];
      }
      end_tmp = lvp->size[0];
      for (size_tmp_idx_0 = 0; size_tmp_idx_0 < end_tmp; size_tmp_idx_0++) {
        vpis->data[i].f1->data[size_tmp_idx_0 + vpis->data[i].f1->size[0]] =
            lvp->data[size_tmp_idx_0];
      }
    } else if (multiSubcarrier) {
      /*  && ~multiSymbol */
      nd->data[i] = 1.0;
      /*  1-D interpolation */
      /*  Record subcarrier subscripts */
      end_tmp = kvp->size[0];
      size_tmp_idx_0 = vpis->data[i].f1->size[0] * vpis->data[i].f1->size[1];
      vpis->data[i].f1->size[0] = kvp->size[0];
      vpis->data[i].f1->size[1] = 1;
      emxEnsureCapacity_real_T(vpis->data[i].f1, size_tmp_idx_0);
      for (size_tmp_idx_0 = 0; size_tmp_idx_0 < end_tmp; size_tmp_idx_0++) {
        vpis->data[i].f1->data[size_tmp_idx_0] = kvp->data[size_tmp_idx_0];
      }
    } else {
      /*  ~multiSubcarrier && ~multiSymbol */
      nd->data[i] = 0.0;
      /*  "0-D interpolation", just record the subscript of the single */
      /*  value from which the VPs will be copied (this subscript will */
      /*  not actually be used) */
      unique_vector(kvp, b);
      size_tmp_idx_0 = vpis->data[i].f1->size[0] * vpis->data[i].f1->size[1];
      vpis->data[i].f1->size[0] = b->size[0];
      vpis->data[i].f1->size[1] = 1;
      emxEnsureCapacity_real_T(vpis->data[i].f1, size_tmp_idx_0);
      end_tmp = b->size[0];
      for (size_tmp_idx_0 = 0; size_tmp_idx_0 < end_tmp; size_tmp_idx_0++) {
        vpis->data[i].f1->data[size_tmp_idx_0] = b->data[size_tmp_idx_0];
      }
    }
  }
  emxFree_real_T(&b);
  emxFree_real_T(&lvp);
  emxFree_real_T(&kvp);
}

/*
 * Arguments    : double nd
 *                const emxArray_real_T *vpi
 *                const emxArray_real_T *k
 *                double n
 *                const emxArray_creal32_T *H0
 *                emxArray_creal32_T *vps
 * Return Type  : void
 */
static void createVPs(double nd, const emxArray_real_T *vpi,
                      const emxArray_real_T *k, double n,
                      const emxArray_creal32_T *H0, emxArray_creal32_T *vps)
{
  static float H0_data[8736];
  static float b_H0_data[8736];
  static float theta_in_data[8736];
  emxArray_real32_T c_H0_data;
  emxArray_real32_T d_H0_data;
  emxArray_real32_T *r;
  emxArray_real32_T *r_in;
  emxArray_real32_T *r_out;
  emxArray_real32_T *theta_in;
  emxArray_real32_T *theta_out;
  float f;
  int H0_size;
  int b_H0_size;
  int b_k;
  int ibcol;
  int itilerow;
  int ntilerows;
  int nx;
  /*  Create virtual pilots 'vps' using interpolant 'vpi'. 'H0' contains the */
  /*  values from which the virtual pilots are created. Virtual pilots are */
  /*  created for subcarrier subscripts 'k' and OFDM symbol subscript 'n'. */
  /*  'nd' is the number of dimensions over which the interpolation is */
  /*  performed */
  if (nd == 0.0) {
    /*  Repeat the single estimate 'H0' for all subscripts 'k' */
    ibcol = vps->size[0] * vps->size[1];
    vps->size[0] = H0->size[0] * k->size[0];
    vps->size[1] = 1;
    emxEnsureCapacity_creal32_T(vps, ibcol);
    nx = H0->size[0];
    ntilerows = k->size[0];
    for (itilerow = 0; itilerow < ntilerows; itilerow++) {
      ibcol = itilerow * nx;
      for (b_k = 0; b_k < nx; b_k++) {
        vps->data[ibcol + b_k] = H0->data[b_k];
      }
    }
  } else {
    /*  nd==1 or nd==2 */
    /*  Perform interpolation */
    /*  Interpolates channel estimate 'H0' in polar co-ordinates, using */
    /*  interpolant 'ipt'. 'H' contains the interpolated channel estimate for */
    /*  subcarrier subscripts 'k' and OFDM symbol subscripts 'n'. 'nd' is the */
    /*  number of dimensions over which the interpolation is performed and */
    /*  'method' is the interpolation method */
    H0_size = H0->size[0];
    ntilerows = H0->size[0];
    for (ibcol = 0; ibcol < ntilerows; ibcol++) {
      H0_data[ibcol] = H0->data[ibcol].re;
    }
    b_H0_size = H0->size[0];
    ntilerows = H0->size[0];
    for (ibcol = 0; ibcol < ntilerows; ibcol++) {
      b_H0_data[ibcol] = H0->data[ibcol].im;
    }
    emxInit_real32_T(&theta_in, 1);
    emxInit_real32_T(&r_in, 1);
    c_H0_data.data = &H0_data[0];
    c_H0_data.size = &H0_size;
    c_H0_data.allocatedSize = 8736;
    c_H0_data.numDimensions = 1;
    c_H0_data.canFreeData = false;
    d_H0_data.data = &b_H0_data[0];
    d_H0_data.size = &b_H0_size;
    d_H0_data.allocatedSize = 8736;
    d_H0_data.numDimensions = 1;
    d_H0_data.canFreeData = false;
    cart2pol(&c_H0_data, &d_H0_data, theta_in, r_in);
    unwrap(theta_in);
    nx = theta_in->size[0];
    ntilerows = theta_in->size[0] - 1;
    for (ibcol = 0; ibcol <= ntilerows; ibcol++) {
      theta_in_data[ibcol] = theta_in->data[ibcol];
    }
    emxFree_real32_T(&theta_in);
    emxInit_real32_T(&theta_out, 2);
    cartInterpolate(nd, vpi, k, n, theta_in_data, nx, theta_out);
    nx = r_in->size[0];
    ntilerows = r_in->size[0] - 1;
    for (ibcol = 0; ibcol <= ntilerows; ibcol++) {
      theta_in_data[ibcol] = r_in->data[ibcol];
    }
    emxFree_real32_T(&r_in);
    emxInit_real32_T(&r_out, 2);
    emxInit_real32_T(&r, 2);
    cartInterpolate(nd, vpi, k, n, theta_in_data, nx, r_out);
    ibcol = r->size[0] * r->size[1];
    r->size[0] = theta_out->size[0];
    r->size[1] = 1;
    emxEnsureCapacity_real32_T(r, ibcol);
    ntilerows = theta_out->size[0];
    for (ibcol = 0; ibcol < ntilerows; ibcol++) {
      r->data[ibcol] = theta_out->data[ibcol];
    }
    nx = theta_out->size[0];
    for (b_k = 0; b_k < nx; b_k++) {
      r->data[b_k] = cosf(r->data[b_k]);
    }
    nx = theta_out->size[0];
    for (b_k = 0; b_k < nx; b_k++) {
      theta_out->data[b_k] = sinf(theta_out->data[b_k]);
    }
    ibcol = vps->size[0] * vps->size[1];
    vps->size[0] = r_out->size[0];
    vps->size[1] = 1;
    emxEnsureCapacity_creal32_T(vps, ibcol);
    ntilerows = r_out->size[0];
    for (ibcol = 0; ibcol < ntilerows; ibcol++) {
      f = r_out->data[ibcol];
      vps->data[ibcol].re = f * r->data[ibcol];
      vps->data[ibcol].im = f * theta_out->data[ibcol];
    }
    emxFree_real32_T(&r);
    emxFree_real32_T(&r_out);
    emxFree_real32_T(&theta_out);
  }
}

/*
 * Create a vector used to store CSI values
 *
 * Arguments    : const double refRBs_data[]
 *                const int refRBs_size[3]
 *                const emxArray_creal32_T *H
 *                float nVar
 * Return Type  : float
 */
static float determineCSIScaling(const double refRBs_data[],
                                 const int refRBs_size[3],
                                 const emxArray_creal32_T *H, float nVar)
{
  static int b_tmp_data[8737];
  static const signed char b_iv3[4] = {3, 4, 2, 1};
  static const signed char b_iv[3] = {2, 3, 1};
  static const signed char b_iv2[3] = {3, 1, 2};
  cell_wrap_50 outputs;
  emxArray_boolean_T *x;
  emxArray_real32_T *b_csi;
  emxArray_real32_T *csi;
  emxArray_real_T *RBs;
  creal32_T Hrb_data[14];
  creal32_T a_data[14];
  double tmp_data[728];
  double x_data[14];
  double d;
  float Hrb_re;
  float c;
  float f;
  float s_data_re;
  int H_size[4];
  int b_iv1[3];
  int b_i;
  int b_k;
  int i;
  int i1;
  int i2;
  int k;
  int plast;
  signed char c_tmp_data[14];
  signed char outsize_idx_1;
  boolean_T y_data[52];
  boolean_T b_x_data[14];
  boolean_T b;
  boolean_T exitg1;
  emxInit_real32_T(&csi, 2);
  /*  Determine how the channel CSI will affect the receiver SNR */
  /*  (assuming MMSE equalization) */
  csi->size[0] = 0;
  csi->size[1] = 1;
  /*  Get number of ports 'P' in the channel estimate */
  /*  Get the RBs (0-based) with reference symbols in any OFDM symbol (2nd */
  /*  dimension of 'refRBs') and any port (3rd dimension of 'refRBs') */
  b = true;
  plast = 0;
  k = 0;
  exitg1 = false;
  while ((!exitg1) && (k < 3)) {
    if (refRBs_size[b_iv[k] - 1] != 1) {
      if (plast > b_iv[k]) {
        b = false;
        exitg1 = true;
      } else {
        plast = b_iv[k];
        k++;
      }
    } else {
      k++;
    }
  }
  if (b) {
    plast = refRBs_size[0] * refRBs_size[1];
    if (0 <= plast - 1) {
      memcpy(&tmp_data[0], &refRBs_data[0], plast * sizeof(double));
    }
  } else {
    plast = refRBs_size[1];
    i = refRBs_size[1];
    for (k = 0; k < i; k++) {
      i1 = refRBs_size[0];
      if (0 <= refRBs_size[0] - 1) {
        outsize_idx_1 = (signed char)(k + 1);
      }
      for (b_k = 0; b_k < i1; b_k++) {
        tmp_data[(outsize_idx_1 + plast * ((signed char)(b_k + 1) - 1)) - 1] =
            refRBs_data[((signed char)(b_k + 1) +
                         refRBs_size[0] * (outsize_idx_1 - 1)) -
                        1];
      }
    }
  }
  i = refRBs_size[1];
  plast = (signed char)refRBs_size[0];
  if (0 <= plast - 1) {
    memset(&outputs.f1.data[0], 0, plast * sizeof(boolean_T));
  }
  plast = refRBs_size[0];
  i2 = 0;
  for (b_i = 0; b_i < plast; b_i++) {
    b_k = i2 + i;
    k = i2;
    i2 += refRBs_size[1];
    exitg1 = false;
    while ((!exitg1) && (k + 1 <= b_k)) {
      if ((tmp_data[k] == 0.0) || rtIsNaN(tmp_data[k])) {
        k++;
      } else {
        outputs.f1.data[b_i] = true;
        exitg1 = true;
      }
    }
  }
  b = true;
  plast = 0;
  k = 0;
  exitg1 = false;
  while ((!exitg1) && (k < 3)) {
    b_iv1[0] = 1;
    b_iv1[1] = 1;
    b_iv1[2] = refRBs_size[0];
    if (b_iv1[b_iv2[k] - 1] != 1) {
      if (plast > b_iv2[k]) {
        b = false;
        exitg1 = true;
      } else {
        plast = b_iv2[k];
        k++;
      }
    } else {
      k++;
    }
  }
  if (b) {
    b_k = refRBs_size[0];
    plast = refRBs_size[0];
    if (0 <= plast - 1) {
      memcpy(&y_data[0], &outputs.f1.data[0], plast * sizeof(boolean_T));
    }
  } else {
    b_k = refRBs_size[0];
    i = refRBs_size[0];
    for (k = 0; k < i; k++) {
      y_data[(signed char)(k + 1) - 1] =
          outputs.f1.data[(signed char)(k + 1) - 1];
    }
  }
  emxInit_real_T(&RBs, 1);
  b_eml_find(y_data, b_k, b_tmp_data, &plast);
  i = RBs->size[0];
  RBs->size[0] = plast;
  emxEnsureCapacity_real_T(RBs, i);
  for (i = 0; i < plast; i++) {
    RBs->data[i] = (double)b_tmp_data[i] - 1.0;
  }
  /*  For each RB */
  i = RBs->size[0];
  emxInit_real32_T(&b_csi, 1);
  for (b_i = 0; b_i < i; b_i++) {
    /*  Prepare logical indices 'x' giving the OFDM symbols for which the */
    /*  current RB has reference symbols on all ports */
    d = RBs->data[b_i];
    plast = refRBs_size[1];
    for (i1 = 0; i1 < plast; i1++) {
      x_data[i1] = refRBs_data[((int)(d + 1.0) + refRBs_size[0] * i1) - 1];
    }
    plast = (signed char)refRBs_size[1];
    for (i1 = 0; i1 < plast; i1++) {
      b_x_data[i1] = true;
    }
    i1 = refRBs_size[1];
    i2 = 0;
    plast = 0;
    for (b_k = 0; b_k < i1; b_k++) {
      plast++;
      i2++;
      k = plast;
      exitg1 = false;
      while ((!exitg1) && ((i1 > 0) && (k <= i2))) {
        if (x_data[k - 1] == 0.0) {
          b_x_data[b_k] = false;
          exitg1 = true;
        } else {
          k += i1;
        }
      }
    }
    /*  Extract the channel matrix 'Hrb' from one RE of the current RB, */
    /*  across the OFDM symbols 'x' and all receive antennas and ports */
    /*  Permute 'Hrb' to give an R-by-P-by-N array where R is the number */
    /*  of receive antennas, P is the number of ports, and N is the */
    /*  number of active OFDM symbols */
    plast = (signed char)refRBs_size[1] - 1;
    i2 = 0;
    b_k = 0;
    for (k = 0; k <= plast; k++) {
      if (b_x_data[k]) {
        i2++;
        c_tmp_data[b_k] = (signed char)(k + 1);
        b_k++;
      }
    }
    plast = (int)(RBs->data[b_i] * 12.0 + 6.0);
    for (i1 = 0; i1 < i2; i1++) {
      a_data[i1] = H->data[(plast + H->size[0] * (c_tmp_data[i1] - 1)) - 1];
    }
    b = true;
    if (i2 != 0) {
      plast = 0;
      k = 0;
      exitg1 = false;
      while ((!exitg1) && (k < 4)) {
        H_size[0] = 1;
        H_size[1] = i2;
        H_size[2] = 1;
        H_size[3] = 1;
        if (H_size[b_iv3[k] - 1] != 1) {
          if (plast > b_iv3[k]) {
            b = false;
            exitg1 = true;
          } else {
            plast = b_iv3[k];
            k++;
          }
        } else {
          k++;
        }
      }
    }
    if (b) {
      if (0 <= i2 - 1) {
        memcpy(&Hrb_data[0], &a_data[0], i2 * sizeof(creal32_T));
      }
    } else {
      for (k = 0; k < i2; k++) {
        Hrb_data[(signed char)(k + 1) - 1] = a_data[(signed char)(k + 1) - 1];
      }
    }
    /*  Compute the CSI for each R-by-P channel matrix 'Hn' in 'Hrb' */
    for (k = 0; k < i2; k++) {
      Hrb_re = Hrb_data[k].re;
      f = Hrb_data[k].im;
      s_data_re = (Hrb_re * Hrb_re - -f * f) + nVar;
      Hrb_re = Hrb_re * f + -f * Hrb_re;
      b = ((!rtIsInfF(s_data_re)) && (!rtIsInfF(Hrb_re)) &&
           ((!rtIsNaNF(s_data_re)) && (!rtIsNaNF(Hrb_re))));
      if (b) {
        if ((s_data_re != 0.0F) || (Hrb_re != 0.0F)) {
          s_data_re = rt_hypotf_snf(s_data_re, Hrb_re);
        }
        if (s_data_re < 0.0F) {
          s_data_re = -s_data_re;
        }
      } else {
        s_data_re = rtNaNF;
      }
      plast = csi->size[0];
      i1 = b_csi->size[0];
      b_csi->size[0] = csi->size[0] + 1;
      emxEnsureCapacity_real32_T(b_csi, i1);
      for (i1 = 0; i1 < plast; i1++) {
        b_csi->data[i1] = csi->data[i1];
      }
      b_csi->data[csi->size[0]] = s_data_re;
      i1 = csi->size[0] * csi->size[1];
      csi->size[0] = b_csi->size[0];
      csi->size[1] = 1;
      emxEnsureCapacity_real32_T(csi, i1);
      plast = b_csi->size[0];
      for (i1 = 0; i1 < plast; i1++) {
        csi->data[i1] = b_csi->data[i1];
      }
    }
  }
  emxFree_real_T(&RBs);
  /*  Compute factor 'c' used to scale the noise estimate 'nVar', such that */
  /*  1/(nVar*c) gives an estimate of the average receiver SNR assuming */
  /*  MMSE equalization */
  emxInit_boolean_T(&x, 1);
  if (csi->size[0] != 0) {
    i = x->size[0];
    x->size[0] = csi->size[0];
    emxEnsureCapacity_boolean_T(x, i);
    plast = csi->size[0];
    for (i = 0; i < plast; i++) {
      x->data[i] = (csi->data[i] == 0.0F);
    }
    b = false;
    k = 1;
    exitg1 = false;
    while ((!exitg1) && (k <= x->size[0])) {
      if (!x->data[k - 1]) {
        k++;
      } else {
        b = true;
        exitg1 = true;
      }
    }
    if (!b) {
      i = b_csi->size[0];
      b_csi->size[0] = csi->size[0];
      emxEnsureCapacity_real32_T(b_csi, i);
      plast = csi->size[0];
      for (i = 0; i < plast; i++) {
        b_csi->data[i] = 1.0F / csi->data[i];
      }
      c = mean(b_csi);
    } else {
      c = 1.0F;
    }
  } else {
    c = 1.0F;
  }
  emxFree_boolean_T(&x);
  emxFree_real32_T(&b_csi);
  emxFree_real32_T(&csi);
  return c;
}

/*
 * Determine if extended cyclic prefix length applies
 *
 * Arguments    : const emxArray_real_T *subn
 * Return Type  : double
 */
static double determineNoiseScaling(const emxArray_real_T *subn)
{
  static const double dv2[42] = {
      0.0713, 0.4633, 0.7403, 0.972,  1.2079, 1.3616, 1.5208, 0.1654, 0.6965,
      1.0876, 1.3307, 1.4707, 1.5517, 1.6357, 0.2845, 0.9379, 1.2757, 1.4028,
      1.499,  1.5961, 1.647,  0.4164, 1.1196, 0.0,    0.0,    0.0,    0.0,
      0.0,    0.6409, 1.2059, 0.0,    0.0,    0.0,    0.0,    0.0,    0.9259,
      0.0,    0.0,    0.0,    0.0,    0.0,    0.0};
  static double dv[6] = {0.0, 0.1087, 0.1203, 0.107, 0.0477, 0.009};
  static double dv1[6] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
  emxArray_boolean_T *x;
  emxArray_int32_T *c_idx;
  emxArray_int32_T *nz;
  emxArray_real_T b_deltas;
  emxArray_real_T b_x_data;
  emxArray_real_T *b_idx;
  emxArray_real_T *b_subn;
  emxArray_real_T *b_y1;
  emxArray_real_T *deltas;
  emxArray_real_T *idx;
  emxArray_real_T *y;
  double log10s_data[42];
  double b_log10s_data[6];
  double x_data[6];
  double y_data[6];
  double dk;
  double s;
  double tmp1;
  double work_data;
  int x_size[2];
  int c_deltas;
  int d_deltas;
  int dimSize;
  int firstBlockLength;
  int iyStart;
  int k;
  int lastBlockLength;
  int m;
  boolean_T exitg1;
  boolean_T idx_data_idx_0;
  boolean_T idx_data_idx_1;
  boolean_T idx_data_idx_2;
  boolean_T idx_data_idx_3;
  boolean_T idx_data_idx_4;
  boolean_T idx_data_idx_5;
  dv[0U] = rtGetNaN();
  dv1[0U] = rtGetNaN();
  emxInit_real_T(&idx, 2);
  /*  Determine noise scaling based on CP length, reference symbol layout, and
   */
  /*  CDM parameters */
  /*  Determine reference symbol frequency spacings 'deltas' and their */
  /*  average 'dk'. Each column of 'idx' will have two rows, giving the */
  /*  index of the first and last element of the subscripts for each OFDM */
  /*  symbol and antenna plane in 'subn'. Then for each OFDM symbol and */
  /*  antenna plane the reference symbol frequency spacing is calculated, */
  /*  and the average is taken. Note that any spacings greater than 12 are */
  /*  limited to a value of 12 to avoid skewing the average (and such large */
  /*  spacings are outside the range where the spacing influences the noise */
  /*  scaling) */
  firstBlockLength = subn->size[0];
  k = idx->size[0] * idx->size[1];
  idx->size[0] = subn->size[0] + 2;
  idx->size[1] = 2;
  emxEnsureCapacity_real_T(idx, k);
  for (k = 0; k < 2; k++) {
    idx->data[idx->size[0] * k] = 0.0;
    for (iyStart = 0; iyStart < firstBlockLength; iyStart++) {
      idx->data[(iyStart + idx->size[0] * k) + 1] =
          subn->data[iyStart + subn->size[0] * (k + 1)];
    }
    idx->data[(firstBlockLength + idx->size[0] * k) + 1] = 0.0;
  }
  emxInit_real_T(&b_idx, 1);
  emxInit_real_T(&b_y1, 2);
  dimSize = idx->size[0];
  k = b_y1->size[0] * b_y1->size[1];
  b_y1->size[0] = idx->size[0] - 1;
  b_y1->size[1] = 2;
  emxEnsureCapacity_real_T(b_y1, k);
  iyStart = 0;
  for (lastBlockLength = 0; lastBlockLength < 2; lastBlockLength++) {
    firstBlockLength = lastBlockLength * dimSize;
    work_data = idx->data[firstBlockLength];
    for (m = 2; m <= dimSize; m++) {
      tmp1 = idx->data[(firstBlockLength + m) - 1];
      dk = tmp1;
      tmp1 -= work_data;
      work_data = dk;
      b_y1->data[(iyStart + m) - 2] = tmp1;
    }
    iyStart = (iyStart + dimSize) - 1;
  }
  emxFree_real_T(&idx);
  emxInit_boolean_T(&x, 2);
  k = x->size[0] * x->size[1];
  x->size[0] = b_y1->size[0];
  x->size[1] = 2;
  emxEnsureCapacity_boolean_T(x, k);
  firstBlockLength = b_y1->size[0] * 2;
  for (k = 0; k < firstBlockLength; k++) {
    x->data[k] = (b_y1->data[k] != 0.0);
  }
  emxFree_real_T(&b_y1);
  emxInit_int32_T(&nz, 1);
  iyStart = x->size[0];
  k = nz->size[0];
  nz->size[0] = x->size[0];
  emxEnsureCapacity_int32_T(nz, k);
  for (firstBlockLength = 0; firstBlockLength < iyStart; firstBlockLength++) {
    nz->data[firstBlockLength] = x->data[firstBlockLength];
    nz->data[firstBlockLength] += x->data[iyStart + firstBlockLength];
  }
  emxFree_boolean_T(&x);
  k = b_idx->size[0];
  b_idx->size[0] = nz->size[0];
  emxEnsureCapacity_real_T(b_idx, k);
  firstBlockLength = nz->size[0];
  for (k = 0; k < firstBlockLength; k++) {
    b_idx->data[k] = nz->data[k];
  }
  dimSize = b_idx->size[0];
  iyStart = 0;
  k = nz->size[0];
  nz->size[0] = b_idx->size[0];
  emxEnsureCapacity_int32_T(nz, k);
  firstBlockLength = 0;
  exitg1 = false;
  while ((!exitg1) && (firstBlockLength <= dimSize - 1)) {
    if (b_idx->data[firstBlockLength] != 0.0) {
      iyStart++;
      nz->data[iyStart - 1] = firstBlockLength + 1;
      if (iyStart >= dimSize) {
        exitg1 = true;
      } else {
        firstBlockLength++;
      }
    } else {
      firstBlockLength++;
    }
  }
  if (b_idx->size[0] == 1) {
    if (iyStart == 0) {
      nz->size[0] = 0;
    }
  } else {
    k = nz->size[0];
    if (1 > iyStart) {
      nz->size[0] = 0;
    } else {
      nz->size[0] = iyStart;
    }
    emxEnsureCapacity_int32_T(nz, k);
  }
  k = b_idx->size[0];
  b_idx->size[0] = nz->size[0];
  emxEnsureCapacity_real_T(b_idx, k);
  firstBlockLength = nz->size[0];
  for (k = 0; k < firstBlockLength; k++) {
    b_idx->data[k] = nz->data[k];
  }
  emxFree_int32_T(&nz);
  if (1 > b_idx->size[0] - 1) {
    firstBlockLength = 0;
  } else {
    firstBlockLength = b_idx->size[0] - 1;
  }
  if (2 > b_idx->size[0]) {
    k = 0;
    iyStart = 0;
  } else {
    k = 1;
    iyStart = b_idx->size[0];
  }
  emxInit_int32_T(&c_idx, 2);
  lastBlockLength = c_idx->size[0] * c_idx->size[1];
  c_idx->size[0] = 2;
  c_idx->size[1] = firstBlockLength;
  emxEnsureCapacity_int32_T(c_idx, lastBlockLength);
  for (lastBlockLength = 0; lastBlockLength < firstBlockLength;
       lastBlockLength++) {
    c_idx->data[2 * lastBlockLength] = (int)b_idx->data[lastBlockLength];
  }
  firstBlockLength = iyStart - k;
  for (iyStart = 0; iyStart < firstBlockLength; iyStart++) {
    c_idx->data[2 * iyStart + 1] = (int)(b_idx->data[k + iyStart] - 1.0);
  }
  emxInit_real_T(&deltas, 2);
  deltas->size[0] = 0;
  deltas->size[1] = 1;
  k = c_idx->size[1];
  emxInit_real_T(&b_subn, 1);
  for (m = 0; m < k; m++) {
    iyStart = c_idx->data[2 * m];
    lastBlockLength = 2 * m + 1;
    dimSize = c_idx->data[lastBlockLength];
    if (iyStart > dimSize) {
      iyStart = 0;
      dimSize = 0;
    } else {
      iyStart--;
    }
    firstBlockLength = dimSize - iyStart;
    dimSize = b_subn->size[0];
    b_subn->size[0] = firstBlockLength;
    emxEnsureCapacity_real_T(b_subn, dimSize);
    for (dimSize = 0; dimSize < firstBlockLength; dimSize++) {
      b_subn->data[dimSize] = subn->data[iyStart + dimSize];
    }
    diff(b_subn, b_idx);
    firstBlockLength = b_idx->size[0];
    for (iyStart = 0; iyStart < firstBlockLength; iyStart++) {
      if (b_idx->data[iyStart] > 12.0) {
        b_idx->data[iyStart] = 12.0;
      }
    }
    if ((c_idx->data[lastBlockLength] - c_idx->data[2 * m]) + 1 > 1) {
      firstBlockLength = deltas->size[0];
      iyStart = b_subn->size[0];
      b_subn->size[0] = deltas->size[0] + b_idx->size[0];
      emxEnsureCapacity_real_T(b_subn, iyStart);
      for (iyStart = 0; iyStart < firstBlockLength; iyStart++) {
        b_subn->data[iyStart] = deltas->data[iyStart];
      }
      firstBlockLength = b_idx->size[0];
      for (iyStart = 0; iyStart < firstBlockLength; iyStart++) {
        b_subn->data[iyStart + deltas->size[0]] = b_idx->data[iyStart];
      }
      iyStart = deltas->size[0] * deltas->size[1];
      deltas->size[0] = b_subn->size[0];
      deltas->size[1] = 1;
      emxEnsureCapacity_real_T(deltas, iyStart);
      firstBlockLength = b_subn->size[0];
      for (iyStart = 0; iyStart < firstBlockLength; iyStart++) {
        deltas->data[iyStart] = b_subn->data[iyStart];
      }
    }
  }
  emxFree_real_T(&b_subn);
  emxFree_int32_T(&c_idx);
  if (deltas->size[0] == 0) {
    tmp1 = 0.0;
  } else {
    if (deltas->size[0] <= 1024) {
      firstBlockLength = deltas->size[0];
      lastBlockLength = 0;
      dimSize = 1;
    } else {
      firstBlockLength = 1024;
      dimSize = deltas->size[0] / 1024;
      lastBlockLength = deltas->size[0] - (dimSize << 10);
      if (lastBlockLength > 0) {
        dimSize++;
      } else {
        lastBlockLength = 1024;
      }
    }
    tmp1 = deltas->data[0];
    for (k = 2; k <= firstBlockLength; k++) {
      tmp1 += deltas->data[k - 1];
    }
    for (m = 2; m <= dimSize; m++) {
      firstBlockLength = (m - 1) << 10;
      work_data = deltas->data[firstBlockLength];
      if (m == dimSize) {
        iyStart = lastBlockLength;
      } else {
        iyStart = 1024;
      }
      for (k = 2; k <= iyStart; k++) {
        work_data += deltas->data[(firstBlockLength + k) - 1];
      }
      tmp1 += work_data;
    }
  }
  dk = tmp1 / (double)deltas->size[0];
  /*  The effect of FD-CDM despreading, interpolation and CIR denoising on */
  /*  the noise variance is influenced by the FD-CDM despreading length, */
  /*  the reference symbol frequency spacing, and the cyclic prefix length. */
  /*  For sufficiently large 'dk' and/or 'fdCDM', the frequency span of the */
  /*  averaging exceeds the bandwidth of the interpolation and CIR */
  /*  denoising, and the scaling factor is a constant multiple of 'fdCDM'. */
  /*  'log10s' contains log10 of the noise scaling factors for smaller */
  /*  values of 'dk' and 'fdCDM'. 'p1' and 'p2' are first order polynomials */
  /*  for creating the scaling factors for larger values of 'dk' with */
  /*  fdCDM=1 and fdCDM=2 respectively */
  /*   dk:         1      2      3      4      5      6 */
  memcpy(&log10s_data[0], &dv2[0], 42U * sizeof(double));
  /*  fdCDM=1 */
  /*  fdCDM=2 */
  /*  fdCDM=3 */
  /*  fdCDM=4 */
  /*  fdCDM=5 */
  /*  fdCDM=6 */
  /*  fdCDM=7 */
  work_data = 0.4342;
  tmp1 = -1.7355;
  /*  fdCDM=1 */
  /*  fdCDM=2 */
  /*  Establish if reference symbols are arranged in groups of adjacent */
  /*  resource elements. This arrangement influences the noise variance for */
  /*  fdCDM=1 and fdCDM=2, so specific scaling factors are used */
  /*  Get unique frequency spacings */
  iyStart = deltas->size[0];
  b_deltas = *deltas;
  c_deltas = iyStart;
  b_deltas.size = &c_deltas;
  b_deltas.numDimensions = 1;
  unique_vector(&b_deltas, b_idx);
  /*  If there are two unique spacings and the minimum spacing is 1 */
  /*  (adjacent resource elements) */
  if ((b_idx->size[0] == 2) && (minimum(b_idx) == 1.0)) {
    /*  Update 'dk' to be the other spacing and update the scaling */
    /*  factors. Note that the first column of 'log10s' corresponds */
    /*  to dk=1 which is not used as 'dk' is now greater than 1 */
    iyStart = deltas->size[0];
    b_deltas = *deltas;
    d_deltas = iyStart;
    b_deltas.size = &d_deltas;
    b_deltas.numDimensions = 1;
    dk = maximum(&b_deltas);
    for (k = 0; k < 6; k++) {
      log10s_data[7 * k] = dv[k];
    }
    for (k = 0; k < 6; k++) {
      log10s_data[7 * k + 1] = dv1[k];
    }
    work_data = 0.1343;
    tmp1 = -0.954;
  }
  emxFree_real_T(&b_idx);
  emxFree_real_T(&deltas);
  /*  General case, constant multiple of fdCDM */
  s = 6.41;
  /*  For tabulated fdCDM values */
  /*  Select the tabulated entries for the configured fdCDM value */
  b_log10s_data[0] = log10s_data[0];
  idx_data_idx_5 = (log10s_data[0] == 0.0);
  idx_data_idx_0 = idx_data_idx_5;
  iyStart = idx_data_idx_5;
  b_log10s_data[1] = log10s_data[7];
  idx_data_idx_5 = (log10s_data[7] == 0.0);
  idx_data_idx_1 = idx_data_idx_5;
  iyStart += idx_data_idx_5;
  b_log10s_data[2] = log10s_data[14];
  idx_data_idx_5 = (log10s_data[14] == 0.0);
  idx_data_idx_2 = idx_data_idx_5;
  iyStart += idx_data_idx_5;
  b_log10s_data[3] = log10s_data[21];
  idx_data_idx_5 = (log10s_data[21] == 0.0);
  idx_data_idx_3 = idx_data_idx_5;
  iyStart += idx_data_idx_5;
  b_log10s_data[4] = log10s_data[28];
  idx_data_idx_5 = (log10s_data[28] == 0.0);
  idx_data_idx_4 = idx_data_idx_5;
  iyStart += idx_data_idx_5;
  b_log10s_data[5] = log10s_data[35];
  idx_data_idx_5 = (log10s_data[35] == 0.0);
  iyStart += idx_data_idx_5;
  firstBlockLength = -1;
  if (!idx_data_idx_0) {
    firstBlockLength = 0;
  }
  if (!idx_data_idx_1) {
    firstBlockLength++;
    b_log10s_data[firstBlockLength] = log10s_data[7];
  }
  if (!idx_data_idx_2) {
    firstBlockLength++;
    b_log10s_data[firstBlockLength] = b_log10s_data[2];
  }
  if (!idx_data_idx_3) {
    firstBlockLength++;
    b_log10s_data[firstBlockLength] = b_log10s_data[3];
  }
  if (!idx_data_idx_4) {
    firstBlockLength++;
    b_log10s_data[firstBlockLength] = b_log10s_data[4];
  }
  if (!idx_data_idx_5) {
    firstBlockLength++;
    b_log10s_data[firstBlockLength] = b_log10s_data[5];
  }
  if (1 > 6 - iyStart) {
    lastBlockLength = 0;
  } else {
    lastBlockLength = 6 - iyStart;
  }
  emxInit_real_T(&y, 2);
  if ((dk >= 1.0) && (dk <= lastBlockLength)) {
    /*  For 'dk' in the tabulated range, interpolate between the */
    /*  tabulated entries in 'log10s' */
    k = y->size[0] * y->size[1];
    y->size[0] = 1;
    y->size[1] = lastBlockLength;
    emxEnsureCapacity_real_T(y, k);
    firstBlockLength = lastBlockLength - 1;
    for (k = 0; k <= firstBlockLength; k++) {
      y->data[k] = (double)k + 1.0;
    }
    x_size[0] = 1;
    x_size[1] = y->size[1];
    firstBlockLength = y->size[1];
    for (k = 0; k < firstBlockLength; k++) {
      x_data[k] = y->data[k];
    }
    if (0 <= lastBlockLength - 1) {
      memcpy(&y_data[0], &b_log10s_data[0], lastBlockLength * sizeof(double));
    }
    dimSize = y->size[1] - 1;
    if (y->data[1] < y->data[0]) {
      k = y->size[1] >> 1;
      for (m = 0; m < k; m++) {
        tmp1 = x_data[m];
        firstBlockLength = dimSize - m;
        x_data[m] = x_data[firstBlockLength];
        x_data[firstBlockLength] = tmp1;
      }
      iyStart = lastBlockLength >> 1;
      for (m = 0; m < iyStart; m++) {
        firstBlockLength = (lastBlockLength - m) - 1;
        tmp1 = y_data[m];
        y_data[m] = y_data[firstBlockLength];
        y_data[firstBlockLength] = tmp1;
      }
    }
    work_data = rtNaN;
    if ((!(dk > x_data[x_size[1] - 1])) && (!(dk < x_data[0]))) {
      b_x_data.data = &x_data[0];
      b_x_data.size = &x_size[0];
      b_x_data.allocatedSize = 6;
      b_x_data.numDimensions = 2;
      b_x_data.canFreeData = false;
      iyStart = b_bsearch(&b_x_data, dk) - 1;
      work_data =
          (dk - x_data[iyStart]) / (x_data[iyStart + 1] - x_data[iyStart]);
      if (work_data == 0.0) {
        work_data = y_data[iyStart];
      } else if (work_data == 1.0) {
        work_data = y_data[iyStart + 1];
      } else {
        dk = y_data[iyStart + 1];
        if (y_data[iyStart] == dk) {
          work_data = y_data[iyStart];
        } else {
          work_data = (1.0 - work_data) * y_data[iyStart] + work_data * dk;
        }
      }
    }
    s = rt_powd_snf(10.0, work_data);
  } else if (dk > lastBlockLength) {
    /*  For larger 'dk' with fdCDM=1 or fdCDM=2, interpolate using */
    /*  first order polynomial 'p' */
    s = rt_powd_snf(10.0, work_data * dk + tmp1);
  }
  emxFree_real_T(&y);
  /*  The effect of TD-CDM despreading on the noise variance is simply a */
  /*  processing gain factor given by the CDM despreading length 'tdCDM' */
  return s;
}

/*
 * Arguments    : double N
 *                const emxArray_real_T *ind
 *                emxArray_real_T *ksubs
 *                emxArray_real_T *nsubs
 *                boolean_T thisport_data[]
 *                int *thisport_size
 * Return Type  : void
 */
static void getPortSubscripts(double N, const emxArray_real_T *ind,
                              emxArray_real_T *ksubs, emxArray_real_T *nsubs,
                              boolean_T thisport_data[], int *thisport_size)
{
  static int v1_data[8736];
  static int varargout_6_data[8736];
  static int vk_data[8736];
  int cpsiz_idx_1;
  int i;
  int loop_ub;
  int v1_size;
  /*  Gets k,n subscripts for K-by-N-by-P grid given indices 'ind' and port */
  /*  'port'. 'thisport' is a logical indexing vector for the port 'port', used
   */
  /*  to extract the corresponding reference symbols */
  cpsiz_idx_1 = 624 * (int)N;
  v1_size = ind->size[0];
  loop_ub = ind->size[0];
  for (i = 0; i < loop_ub; i++) {
    v1_data[i] = (int)ind->data[i] - 1;
  }
  for (i = 0; i < v1_size; i++) {
    vk_data[i] = div_s32(v1_data[i], cpsiz_idx_1);
  }
  for (i = 0; i < v1_size; i++) {
    varargout_6_data[i] = vk_data[i] + 1;
  }
  for (i = 0; i < v1_size; i++) {
    v1_data[i] -= vk_data[i] * cpsiz_idx_1;
  }
  for (i = 0; i < v1_size; i++) {
    vk_data[i] = div_s32(v1_data[i], 624);
  }
  for (i = 0; i < v1_size; i++) {
    v1_data[i] -= vk_data[i] * 624;
  }
  i = ksubs->size[0];
  ksubs->size[0] = v1_size;
  emxEnsureCapacity_real_T(ksubs, i);
  for (i = 0; i < v1_size; i++) {
    ksubs->data[i] = v1_data[i] + 1;
  }
  i = nsubs->size[0];
  nsubs->size[0] = v1_size;
  emxEnsureCapacity_real_T(nsubs, i);
  for (i = 0; i < v1_size; i++) {
    nsubs->data[i] = vk_data[i] + 1;
  }
  *thisport_size = v1_size;
  for (i = 0; i < v1_size; i++) {
    thisport_data[i] = (varargout_6_data[i] == 1);
  }
  v1_size--;
  loop_ub = 0;
  cpsiz_idx_1 = 0;
  for (i = 0; i <= v1_size; i++) {
    if (varargout_6_data[i] == 1) {
      loop_ub++;
      ksubs->data[cpsiz_idx_1] = ksubs->data[i];
      cpsiz_idx_1++;
    }
  }
  i = ksubs->size[0];
  ksubs->size[0] = loop_ub;
  emxEnsureCapacity_real_T(ksubs, i);
  loop_ub = 0;
  cpsiz_idx_1 = 0;
  for (i = 0; i <= v1_size; i++) {
    if (varargout_6_data[i] == 1) {
      loop_ub++;
      nsubs->data[cpsiz_idx_1] = nsubs->data[i];
      cpsiz_idx_1++;
    }
  }
  i = nsubs->size[0];
  nsubs->size[0] = loop_ub;
  emxEnsureCapacity_real_T(nsubs, i);
}

/*
 * Arguments    : const emxArray_real_T *k0
 *                const emxArray_creal32_T *H0
 *                const emxArray_real_T *k
 *                emxArray_creal32_T *H
 * Return Type  : void
 */
static void interpolate(const emxArray_real_T *k0, const emxArray_creal32_T *H0,
                        const emxArray_real_T *k, emxArray_creal32_T *H)
{
  emxArray_real32_T *r_in;
  emxArray_real32_T *r_out;
  emxArray_real32_T *theta_in;
  emxArray_real32_T *theta_out;
  int b_k;
  int nx;
  emxInit_real32_T(&theta_out, 1);
  /*  Interpolate channel estimate 'H0' defined for subcarrier subscripts 'k0'
   */
  /*  and OFDM symbol subscripts 'l0', using 'method' as the interpolation */
  /*  method. 'nd' is the number of dimensions over which interpolation is */
  /*  performed. 'polar' determines whether interpolation is performed using */
  /*  polar (true) or Cartesian (false) co-ordinates. 'H' contains the */
  /*  interpolated channel estimate for subcarrier subscripts 'k' and OFDM */
  /*  symbol subscripts 'n' */
  /*  Interpolates channel estimate 'H0' in polar co-ordinates, using */
  /*  interpolant 'ipt'. 'H' contains the interpolated channel estimate for */
  /*  subcarrier subscripts 'k' and OFDM symbol subscripts 'n'. 'nd' is the */
  /*  number of dimensions over which the interpolation is performed and */
  /*  'method' is the interpolation method */
  b_k = theta_out->size[0];
  theta_out->size[0] = H0->size[0];
  emxEnsureCapacity_real32_T(theta_out, b_k);
  nx = H0->size[0];
  for (b_k = 0; b_k < nx; b_k++) {
    theta_out->data[b_k] = H0->data[b_k].re;
  }
  emxInit_real32_T(&r_out, 1);
  b_k = r_out->size[0];
  r_out->size[0] = H0->size[0];
  emxEnsureCapacity_real32_T(r_out, b_k);
  nx = H0->size[0];
  for (b_k = 0; b_k < nx; b_k++) {
    r_out->data[b_k] = H0->data[b_k].im;
  }
  emxInit_real32_T(&theta_in, 1);
  emxInit_real32_T(&r_in, 1);
  cart2pol(theta_out, r_out, theta_in, r_in);
  /*  If the specified method is 'VP', this indicates that the */
  /*  interpolation is being performed for the virtual pilots. In this */
  /*  case, the channel estimate 'H0' is pre-processed to give a best-fit */
  /*  linear interpolation (to remove noise) prior to linearly */
  /*  extrapolating to calculate the VPs */
  /*  Interpolates channel estimate 'H0' in Cartesian co-ordinates (or real */
  /*  values), using interpolant 'ipt'. 'H' contains the interpolated channel */
  /*  estimate for subcarrier subscripts 'k' and OFDM symbol subscripts 'n'. */
  /*  'nd' is the number of dimensions over which the interpolation is */
  /*  performed and 'method' is the interpolation method */
  /*  Perform interpolation */
  /*  Pre-process VPs if applicable */
  /*  Perform 1-D interpolation using interp1 */
  unwrap(theta_in);
  b_interp1(k0, theta_in, k, theta_out);
  /*  If the specified method is 'VP', this indicates that the */
  /*  interpolation is being performed for the virtual pilots. In this */
  /*  case, the channel estimate 'H0' is pre-processed to give a best-fit */
  /*  linear interpolation (to remove noise) prior to linearly */
  /*  extrapolating to calculate the VPs */
  /*  Interpolates channel estimate 'H0' in Cartesian co-ordinates (or real */
  /*  values), using interpolant 'ipt'. 'H' contains the interpolated channel */
  /*  estimate for subcarrier subscripts 'k' and OFDM symbol subscripts 'n'. */
  /*  'nd' is the number of dimensions over which the interpolation is */
  /*  performed and 'method' is the interpolation method */
  /*  Perform interpolation */
  /*  Pre-process VPs if applicable */
  /*  Perform 1-D interpolation using interp1 */
  b_interp1(k0, r_in, k, r_out);
  b_k = theta_in->size[0];
  theta_in->size[0] = theta_out->size[0];
  emxEnsureCapacity_real32_T(theta_in, b_k);
  nx = theta_out->size[0];
  emxFree_real32_T(&r_in);
  for (b_k = 0; b_k < nx; b_k++) {
    theta_in->data[b_k] = theta_out->data[b_k];
  }
  nx = theta_out->size[0];
  for (b_k = 0; b_k < nx; b_k++) {
    theta_in->data[b_k] = cosf(theta_in->data[b_k]);
  }
  nx = theta_out->size[0];
  for (b_k = 0; b_k < nx; b_k++) {
    theta_out->data[b_k] = sinf(theta_out->data[b_k]);
  }
  b_k = H->size[0];
  H->size[0] = r_out->size[0];
  emxEnsureCapacity_creal32_T(H, b_k);
  nx = r_out->size[0];
  for (b_k = 0; b_k < nx; b_k++) {
    H->data[b_k].re = r_out->data[b_k] * theta_in->data[b_k];
    H->data[b_k].im = r_out->data[b_k] * theta_out->data[b_k];
  }
  emxFree_real32_T(&theta_in);
  emxFree_real32_T(&r_out);
  emxFree_real32_T(&theta_out);
}

/*
 * Arguments    : double n
 *                double w
 *                double p_data[]
 *                int *p_size
 * Return Type  : void
 */
static void raised_cosine_window(double n, double w, double p_data[],
                                 int *p_size)
{
  emxArray_real_T *x;
  emxArray_real_T *y;
  double x_data[771];
  double y_data[771];
  double unnamed_idx_0;
  double xtmp;
  int i;
  int md2;
  int x_size;
  int x_tmp;
  /*  Raised cosine window creation; creates a window function of length n+w */
  /*  with raised cosine transitions on the first and last 'w' samples. */
  emxInit_real_T(&y, 2);
  if (rtIsNaN(w)) {
    x_tmp = y->size[0] * y->size[1];
    y->size[0] = 1;
    y->size[1] = 1;
    emxEnsureCapacity_real_T(y, x_tmp);
    y->data[0] = rtNaN;
  } else if (w < 1.0) {
    y->size[0] = 1;
    y->size[1] = 0;
  } else if (rtIsInf(w) && (1.0 == w)) {
    x_tmp = y->size[0] * y->size[1];
    y->size[0] = 1;
    y->size[1] = 1;
    emxEnsureCapacity_real_T(y, x_tmp);
    y->data[0] = rtNaN;
  } else {
    x_tmp = y->size[0] * y->size[1];
    y->size[0] = 1;
    md2 = (int)floor(w - 1.0);
    y->size[1] = md2 + 1;
    emxEnsureCapacity_real_T(y, x_tmp);
    for (x_tmp = 0; x_tmp <= md2; x_tmp++) {
      y->data[x_tmp] = (double)x_tmp + 1.0;
    }
  }
  unnamed_idx_0 = 2.0 * w;
  x_size = y->size[1];
  md2 = y->size[1];
  for (x_tmp = 0; x_tmp < md2; x_tmp++) {
    x_data[x_tmp] =
        3.1415926535897931 * ((w + 1.0) - 2.0 * y->data[x_tmp]) / unnamed_idx_0;
  }
  emxFree_real_T(&y);
  for (md2 = 0; md2 < x_size; md2++) {
    x_data[md2] = sin(x_data[md2]);
  }
  for (x_tmp = 0; x_tmp < x_size; x_tmp++) {
    y_data[x_tmp] = 0.5 * (1.0 - x_data[x_tmp]);
  }
  emxInit_real_T(&x, 1);
  unnamed_idx_0 = n - w;
  x_tmp = x->size[0];
  x->size[0] = x_size;
  emxEnsureCapacity_real_T(x, x_tmp);
  for (x_tmp = 0; x_tmp < x_size; x_tmp++) {
    x->data[x_tmp] = y_data[x_tmp];
  }
  md2 = x_size >> 1;
  for (i = 0; i < md2; i++) {
    xtmp = x->data[i];
    x_tmp = (x_size - i) - 1;
    x->data[i] = x->data[x_tmp];
    x->data[x_tmp] = xtmp;
  }
  *p_size = (x_size + (int)unnamed_idx_0) + x->size[0];
  if (0 <= x_size - 1) {
    memcpy(&p_data[0], &y_data[0], x_size * sizeof(double));
  }
  md2 = (int)unnamed_idx_0;
  for (x_tmp = 0; x_tmp < md2; x_tmp++) {
    p_data[x_tmp + x_size] = 1.0;
  }
  md2 = x->size[0];
  for (x_tmp = 0; x_tmp < md2; x_tmp++) {
    p_data[(x_tmp + x_size) + (int)unnamed_idx_0] = x->data[x_tmp];
  }
  emxFree_real_T(&x);
}

/*
 * Arguments    : struct_T *cec
 *                float nVar
 * Return Type  : void
 */
static void setAveragingParameters(struct_T *cec, float nVar)
{
  float SNR;
  int k;
  boolean_T x_data[3];
  boolean_T exitg1;
  boolean_T y;
  /*  If either averaging parameter is not set, automatically choose a value */
  /*  based on the SNR (calculated from the noise variance) */
  x_data[0] = (cec->AveragingWindow[0] == 0.0);
  x_data[1] = (cec->AveragingWindow[1] == 0.0);
  y = false;
  k = 0;
  exitg1 = false;
  while ((!exitg1) && (k <= 1)) {
    if (!x_data[k]) {
      k++;
    } else {
      y = true;
      exitg1 = true;
    }
  }
  if (y) {
    SNR = -10.0F * log10f(nVar);
    /*  Frequency averaging */
    if (SNR > 20.0F) {
      cec->AveragingWindow[0] = 1.0;
    } else if (SNR > 7.0F) {
      cec->AveragingWindow[0] = 3.0;
    } else if (SNR > 5.0F) {
      cec->AveragingWindow[0] = 5.0;
    } else {
      /*  SNR<=5 */
      cec->AveragingWindow[0] = 7.0;
    }
    /*  Time averaging */
    if (SNR > 8.0F) {
      cec->AveragingWindow[1] = 1.0;
    } else {
      /*  SNR<=8 */
      cec->AveragingWindow[1] = 3.0;
    }
  }
}

/*
 * nrChannelEstimate Practical channel estimation
 *    [H,NVAR,INFO] = nrChannelEstimate(...) performs channel estimation,
 *    returning channel estimate H, noise variance estimate NVAR and
 *    information structure INFO. H is a K-by-N-by-R-by-P array where K is
 *    the number of subcarriers, N is the number of OFDM symbols, R is the
 *    number of receive antennas and P is the number of reference signal
 *    ports. NVAR is scalar indicating the measured variance of additive
 *    white Gaussian noise on the received reference symbols. INFO is a
 *    structure containing the field:
 *    AveragingWindow - a 2-element row vector [F T] indicating the number
 *                      of adjacent reference symbols in the frequency
 *                      direction F and time direction T over which averaging
 *                      was performed prior to interpolation
 *
 *    [H,NVAR,INFO] = nrChannelEstimate(CARRIER,RXGRID,REFIND,REFSYM)
 *    performs channel estimation on the received resource grid RXGRID using
 *    reference symbols REFSYM whose locations are given by REFIND.
 *
 *    CARRIER is a carrier configuration object, <a
 *    href="matlab:help('nrCarrierConfig')"
 *    >nrCarrierConfig</a>. Only this
 *    object property is relevant for this function:
 *
 *    CyclicPrefix      - Cyclic prefix ('normal', 'extended')
 *
 *    RXGRID is an array of size K-by-L-by-R. K is the number of subcarriers,
 *    given by CARRIER.NSizeGrid * 12. L is the number of OFDM symbols in one
 *    slot, given by CARRIER.SymbolsPerSlot.
 *
 *    REFIND and REFSYM are the reference signal indices and symbols,
 *    respectively. REFIND is an array of 1-based linear indices addressing a
 *    K-by-L-by-P resource array. P is the number of reference signal ports
 *    and is inferred from the range of values in REFIND. Only nonzero
 *    elements in REFSYM are considered. Any zero-valued elements in REFSYM
 *    and their associated indices in REFIND are ignored.
 *
 *    [H,NVAR,INFO] = nrChannelEstimate(CARRIER,RXGRID,REFGRID) specifies a
 *    predefined reference resource grid in REFGRID. REFGRID is an array with
 *    nonzero elements representing the reference symbols in their
 *    appropriate locations. It is of size K-by-N-by-P, where N is the number
 *    of OFDM symbols. REFGRID can span multiple slots. RXGRID must be an
 *    array of size K-by-N-by-R.
 *
 *    [H,NVAR,INFO] = nrChannelEstimate(RXGRID,REFIND,REFSYM,NAME,VALUE,...)
 *    and [H,NVAR,INFO] = nrChannelEstimate(RXGRID,REFGRID,NAME,VALUE,...)
 *    perform channel estimation as above, but an additional NAME,VALUE pair
 *    is used in place of the CARRIER configuration object:
 *
 *    'CyclicPrefix'      - Cyclic prefix ('normal' (default), 'extended')
 *
 *    Note that for the numerologies specified in TS 38.211 Section 4.2,
 *    extended cyclic prefix length is only applicable for 60 kHz subcarrier
 *    spacing. For normal cyclic prefix there are L=14 OFDM symbols in a
 *    slot. For extended cyclic prefix, L=12.
 *
 *    [H,NVAR,INFO] = nrChannelEstimate(...,NAME,VALUE,...) specifies
 *    additional options as NAME,VALUE pairs:
 *
 *    'CDMLengths'      - A 2-element row vector [FD TD] specifying the
 *                        length of FD-CDM and TD-CDM despreading to perform.
 *                        A value of 1 for an element indicates no CDM and a
 *                        value greater than 1 indicates the length of the
 *                        CDM. For example, [2 1] indicates FD-CDM2 and no
 *                        TD-CDM. The default is [1 1] (no orthogonal
 *                        despreading)
 *
 *    'AveragingWindow' - A 2-element row vector [F T] specifying the number
 *                        of adjacent reference symbols in the frequency
 *                        domain F and time domain T over which to average
 *                        prior to interpolation. F and T must be odd or
 *                        zero. If F or T is zero, the averaging value is
 *                        determined automatically from the estimated SNR
 *                        (calculated using NVAR). The default is [0 0]
 *
 *    Example:
 *    % Create a resource grid containing the PDSCH DM-RS and pass it through
 *    % a TDL-C channel. Estimate the channel response and compare it with
 *    % the perfect channel estimator.
 *
 *    carrier = nrCarrierConfig;
 *    pdsch = nrPDSCHConfig;
 *    dmrsInd = nrPDSCHDMRSIndices(carrier,pdsch);
 *    dmrsSym = nrPDSCHDMRS(carrier,pdsch);
 *    nTxAnts = 1;
 *    txGrid = nrResourceGrid(carrier,nTxAnts);
 *    txGrid(dmrsInd) = dmrsSym;
 *
 *    [txWaveform,ofdmInfo] = nrOFDMModulate(carrier,txGrid);
 *
 *    channel = nrTDLChannel;
 *    channel.NumReceiveAntennas = 1;
 *    channel.SampleRate = ofdmInfo.SampleRate;
 *    channel.DelayProfile = 'TDL-C';
 *    channel.DelaySpread = 100e-9;
 *    channel.MaximumDopplerShift = 20;
 *    chInfo = info(channel);
 *    maxChDelay = ceil(max(chInfo.PathDelays*channel.SampleRate)) +
 * chInfo.ChannelFilterDelay; [rxWaveform,pathGains] = channel([txWaveform;
 * zeros(maxChDelay,nTxAnts)]);
 *
 *    offset = nrTimingEstimate(carrier,rxWaveform,dmrsInd,dmrsSym);
 *    rxWaveform = rxWaveform(1+offset:end,:);
 *
 *    rxGrid = nrOFDMDemodulate(carrier,rxWaveform);
 *
 *    [H,nVar,estInfo] = nrChannelEstimate(carrier,rxGrid,dmrsInd,dmrsSym);
 *
 *    pathFilters = getPathFilters(channel);
 *    H_ideal = nrPerfectChannelEstimate(carrier,pathGains,pathFilters,offset);
 *
 *    figure;
 *    subplot(1,2,1);
 *    imagesc(abs(H));
 *    xlabel('OFDM symbol');
 *    ylabel('Subcarrier');
 *    title('Practical estimate magnitude');
 *    subplot(1,2,2);
 *    imagesc(abs(H_ideal));
 *    xlabel('OFDM symbol');
 *    ylabel('Subcarrier');
 *    title('Perfect estimate magnitude');
 *
 *    See also nrTimingEstimate, nrPerfectChannelEstimate,
 *    nrPerfectTimingEstimate, nrCarrierConfig.
 *
 * Arguments    : const creal32_T rxGrid[624]
 *                emxArray_creal32_T *H
 *                float *nVar
 * Return Type  : void
 */
void nrChannelEstimateOpt(const creal32_T rxGrid[624], emxArray_creal32_T *H,
                          float *nVar)
{
  static const creal_T b_dcv[72] = {{
                                        0.707106781186548, /* re */
                                        -0.707106781186548 /* im */
                                    },
                                    {
                                        0.707106781186548, /* re */
                                        0.707106781186548  /* im */
                                    },
                                    {
                                        0.707106781186548, /* re */
                                        -0.707106781186548 /* im */
                                    },
                                    {
                                        -0.707106781186548, /* re */
                                        0.707106781186548   /* im */
                                    },
                                    {
                                        0.707106781186548, /* re */
                                        -0.707106781186548 /* im */
                                    },
                                    {
                                        0.707106781186548, /* re */
                                        -0.707106781186548 /* im */
                                    },
                                    {
                                        -0.707106781186548, /* re */
                                        0.707106781186548   /* im */
                                    },
                                    {
                                        0.707106781186548, /* re */
                                        -0.707106781186548 /* im */
                                    },
                                    {
                                        -0.707106781186548, /* re */
                                        0.707106781186548   /* im */
                                    },
                                    {
                                        0.707106781186548, /* re */
                                        -0.707106781186548 /* im */
                                    },
                                    {
                                        0.707106781186548, /* re */
                                        0.707106781186548  /* im */
                                    },
                                    {
                                        0.707106781186548, /* re */
                                        -0.707106781186548 /* im */
                                    },
                                    {
                                        -0.707106781186548, /* re */
                                        -0.707106781186548  /* im */
                                    },
                                    {
                                        -0.707106781186548, /* re */
                                        -0.707106781186548  /* im */
                                    },
                                    {
                                        -0.707106781186548, /* re */
                                        0.707106781186548   /* im */
                                    },
                                    {
                                        0.707106781186548, /* re */
                                        -0.707106781186548 /* im */
                                    },
                                    {
                                        -0.707106781186548, /* re */
                                        -0.707106781186548  /* im */
                                    },
                                    {
                                        0.707106781186548, /* re */
                                        -0.707106781186548 /* im */
                                    },
                                    {
                                        -0.707106781186548, /* re */
                                        0.707106781186548   /* im */
                                    },
                                    {
                                        -0.707106781186548, /* re */
                                        0.707106781186548   /* im */
                                    },
                                    {
                                        0.707106781186548, /* re */
                                        0.707106781186548  /* im */
                                    },
                                    {
                                        -0.707106781186548, /* re */
                                        -0.707106781186548  /* im */
                                    },
                                    {
                                        -0.707106781186548, /* re */
                                        0.707106781186548   /* im */
                                    },
                                    {
                                        0.707106781186548, /* re */
                                        0.707106781186548  /* im */
                                    },
                                    {
                                        0.707106781186548, /* re */
                                        0.707106781186548  /* im */
                                    },
                                    {
                                        -0.707106781186548, /* re */
                                        0.707106781186548   /* im */
                                    },
                                    {
                                        -0.707106781186548, /* re */
                                        -0.707106781186548  /* im */
                                    },
                                    {
                                        -0.707106781186548, /* re */
                                        0.707106781186548   /* im */
                                    },
                                    {
                                        0.707106781186548, /* re */
                                        -0.707106781186548 /* im */
                                    },
                                    {
                                        -0.707106781186548, /* re */
                                        -0.707106781186548  /* im */
                                    },
                                    {
                                        -0.707106781186548, /* re */
                                        -0.707106781186548  /* im */
                                    },
                                    {
                                        -0.707106781186548, /* re */
                                        0.707106781186548   /* im */
                                    },
                                    {
                                        0.707106781186548, /* re */
                                        0.707106781186548  /* im */
                                    },
                                    {
                                        0.707106781186548, /* re */
                                        -0.707106781186548 /* im */
                                    },
                                    {
                                        0.707106781186548, /* re */
                                        -0.707106781186548 /* im */
                                    },
                                    {
                                        0.707106781186548, /* re */
                                        0.707106781186548  /* im */
                                    },
                                    {
                                        0.707106781186548, /* re */
                                        -0.707106781186548 /* im */
                                    },
                                    {
                                        -0.707106781186548, /* re */
                                        -0.707106781186548  /* im */
                                    },
                                    {
                                        -0.707106781186548, /* re */
                                        0.707106781186548   /* im */
                                    },
                                    {
                                        0.707106781186548, /* re */
                                        -0.707106781186548 /* im */
                                    },
                                    {
                                        -0.707106781186548, /* re */
                                        0.707106781186548   /* im */
                                    },
                                    {
                                        -0.707106781186548, /* re */
                                        0.707106781186548   /* im */
                                    },
                                    {
                                        0.707106781186548, /* re */
                                        0.707106781186548  /* im */
                                    },
                                    {
                                        0.707106781186548, /* re */
                                        0.707106781186548  /* im */
                                    },
                                    {
                                        -0.707106781186548, /* re */
                                        -0.707106781186548  /* im */
                                    },
                                    {
                                        -0.707106781186548, /* re */
                                        -0.707106781186548  /* im */
                                    },
                                    {
                                        0.707106781186548, /* re */
                                        0.707106781186548  /* im */
                                    },
                                    {
                                        -0.707106781186548, /* re */
                                        0.707106781186548   /* im */
                                    },
                                    {
                                        0.707106781186548, /* re */
                                        0.707106781186548  /* im */
                                    },
                                    {
                                        0.707106781186548, /* re */
                                        0.707106781186548  /* im */
                                    },
                                    {
                                        0.707106781186548, /* re */
                                        0.707106781186548  /* im */
                                    },
                                    {
                                        -0.707106781186548, /* re */
                                        -0.707106781186548  /* im */
                                    },
                                    {
                                        0.707106781186548, /* re */
                                        -0.707106781186548 /* im */
                                    },
                                    {
                                        0.707106781186548, /* re */
                                        -0.707106781186548 /* im */
                                    },
                                    {
                                        -0.707106781186548, /* re */
                                        0.707106781186548   /* im */
                                    },
                                    {
                                        -0.707106781186548, /* re */
                                        -0.707106781186548  /* im */
                                    },
                                    {
                                        -0.707106781186548, /* re */
                                        0.707106781186548   /* im */
                                    },
                                    {
                                        -0.707106781186548, /* re */
                                        0.707106781186548   /* im */
                                    },
                                    {
                                        -0.707106781186548, /* re */
                                        -0.707106781186548  /* im */
                                    },
                                    {
                                        0.707106781186548, /* re */
                                        -0.707106781186548 /* im */
                                    },
                                    {
                                        0.707106781186548, /* re */
                                        0.707106781186548  /* im */
                                    },
                                    {
                                        -0.707106781186548, /* re */
                                        0.707106781186548   /* im */
                                    },
                                    {
                                        0.707106781186548, /* re */
                                        -0.707106781186548 /* im */
                                    },
                                    {
                                        0.707106781186548, /* re */
                                        0.707106781186548  /* im */
                                    },
                                    {
                                        0.707106781186548, /* re */
                                        0.707106781186548  /* im */
                                    },
                                    {
                                        0.707106781186548, /* re */
                                        0.707106781186548  /* im */
                                    },
                                    {
                                        -0.707106781186548, /* re */
                                        -0.707106781186548  /* im */
                                    },
                                    {
                                        0.707106781186548, /* re */
                                        -0.707106781186548 /* im */
                                    },
                                    {
                                        0.707106781186548, /* re */
                                        0.707106781186548  /* im */
                                    },
                                    {
                                        0.707106781186548, /* re */
                                        -0.707106781186548 /* im */
                                    },
                                    {
                                        -0.707106781186548, /* re */
                                        0.707106781186548   /* im */
                                    },
                                    {
                                        -0.707106781186548, /* re */
                                        0.707106781186548   /* im */
                                    }};
  static double w_data[4428];
  static double b_tmp_data[3756];
  static int tmp_data[8737];
  static int vpiIdxHigh_data[8736];
  static int vpiIdxLow_data[8736];
  static const short b_iv[72] = {
      1,   5,   9,   13,  17,  21,  25,  29,  33,  37,  41,  45,  49,  53,  57,
      61,  65,  69,  73,  77,  81,  85,  89,  93,  97,  101, 105, 109, 113, 117,
      121, 125, 129, 133, 137, 141, 289, 293, 297, 301, 305, 309, 313, 317, 321,
      325, 329, 333, 337, 341, 345, 349, 353, 357, 361, 365, 369, 373, 377, 381,
      385, 389, 393, 397, 401, 405, 409, 413, 417, 421, 425, 429};
  static const signed char b_iv1[3] = {0, 1, 3};
  emxArray_boolean_T *r1;
  emxArray_cell_wrap_8 *vpisHigh;
  emxArray_cell_wrap_8 *vpisLow;
  emxArray_creal32_T e_Hrp;
  emxArray_creal32_T *H_LS;
  emxArray_creal32_T *Hlrp;
  emxArray_creal32_T *Hn;
  emxArray_creal32_T *Hrp;
  emxArray_creal32_T *b_H;
  emxArray_creal32_T *b_Hrp;
  emxArray_creal32_T *b_vpLow;
  emxArray_creal32_T *c_H;
  emxArray_creal32_T *c_Hrp;
  emxArray_creal32_T *d_Hrp;
  emxArray_creal32_T *r3;
  emxArray_creal32_T *r7;
  emxArray_creal32_T *r8;
  emxArray_creal32_T *vpHigh;
  emxArray_creal32_T *vpLow;
  emxArray_creal_T *refGrid;
  emxArray_creal_T *refSym;
  emxArray_int16_T *r4;
  emxArray_int32_T *r2;
  emxArray_real_T b_z_data;
  emxArray_real_T *b_i;
  emxArray_real_T *b_kvpLow;
  emxArray_real_T *b_subn;
  emxArray_real_T *c_i;
  emxArray_real_T *c_kvpLow;
  emxArray_real_T *c_subn;
  emxArray_real_T *k;
  emxArray_real_T *ke;
  emxArray_real_T *krange;
  emxArray_real_T *ksubs;
  emxArray_real_T *kvpLow;
  emxArray_real_T *ln;
  emxArray_real_T *ndsHigh;
  emxArray_real_T *ndsLow;
  emxArray_real_T *nsubs;
  emxArray_real_T *pn;
  emxArray_real_T *r;
  emxArray_real_T *r5;
  emxArray_real_T *r6;
  emxArray_real_T *r9;
  emxArray_real_T *rbidx;
  emxArray_real_T *rbsubs;
  emxArray_real_T *refInd;
  emxArray_real_T *result;
  emxArray_real_T *subn;
  emxArray_real_T *ukHigh;
  emxArray_real_T *ukLow;
  emxArray_real_T *un;
  emxArray_real_T *uz;
  emxArray_real_T *vpIndicesHigh;
  emxArray_real_T *vpIndicesLow;
  struct_T estConfig;
  creal32_T H_data[672];
  creal32_T h_data[672];
  double refRBs_data[784];
  double b_data[742];
  double z_data[53];
  double un_data[16];
  double kmax_data[14];
  double kmin_data[14];
  double t1_CyclicPrefixLengths_data[14];
  double siz[4];
  double varargin_1[2];
  double b_expl_temp;
  double c_expl_temp;
  double d_expl_temp;
  double expl_temp;
  double n;
  double t1_Nfft;
  double t1_SlotsPerSubframe;
  double t1_SubcarrierSpacing;
  double t1_SymbolsPerSlot;
  double timeAveraging;
  float ai;
  float ar;
  float brm;
  float c;
  float refSym_im;
  float refSym_re;
  float s;
  int indx_data[53];
  int ipos_data[53];
  int refRBs_size[3];
  int b_result[2];
  int kmin_size[2];
  int t1_CyclicPrefixLengths_size[2];
  int ukLow_size[2];
  int un_size[2];
  int b_end;
  int b_loop_ub;
  int c_end;
  int c_loop_ub;
  int d_end;
  int d_loop_ub;
  int e_loop_ub;
  int end;
  int f_loop_ub;
  int i;
  int i1;
  int i2;
  int i6;
  int i7;
  int loop_ub;
  int loop_ub_tmp;
  int m;
  int md2;
  int tmp_size;
  int uni;
  int z_size;
  short c_tmp_data[8736];
  short e_tmp_data[8736];
  short f_tmp_data[8736];
  short sizes_idx_1;
  signed char d_tmp_data[8736];
  signed char b_refRBs_data[784];
  signed char d_refRBs_data[728];
  signed char i3;
  signed char i4;
  signed char i5;
  boolean_T kvpLow_data[8737];
  boolean_T refGrid_data[8736];
  boolean_T c_z_data[53];
  boolean_T c_refRBs_data[52];
  boolean_T b_kmin_data[14];
  boolean_T empty_non_axis_sizes;
  emxInit_creal_T(&refGrid, 3);
  /*    Copyright 2019-2020 The MathWorks, Inc. */
  /*  Determine if syntax with nrCarrierConfig is being used and parse */
  /*  relevant inputs */
  /*  Validate type of grid and number of dimensions in grid, dimension */
  /*  sizes are validated when other relevant parameters are known */
  /*  (refInd,refSym versus refGrid syntax, normal versus extended cyclic */
  /*  prefix) */
  /*  Determine whether the refInd,refSym syntax or refGrid syntax is being */
  /*  used */
  /*  Get OFDM information. Subcarrier spacing (SCS) is hard-wired to 15 */
  /*  kHz because SCS does not affect OFDM information fields as used in */
  /*  the channel estimator: */
  /*   * SampleRate is not used */
  /*   * SymbolsPerSubframe is not used */
  /*   * larger CP lengths every 0.5ms are not used, because only */
  /*     min(CyclicPrefixLengths) is used */
  /*   * length of CyclicPrefixLengths is not used */
  OFDMInfo(false, &n, &t1_Nfft, &t1_SubcarrierSpacing, &timeAveraging,
           t1_CyclicPrefixLengths_data, t1_CyclicPrefixLengths_size,
           &t1_SymbolsPerSlot, &t1_SlotsPerSubframe, &expl_temp, &b_expl_temp,
           &c_expl_temp, &d_expl_temp);
  /*  Get the number of subcarriers K and OFDM symbols L from the OFDM */
  /*  information */
  /*  Validate reference inputs */
  /*  Validate reference indices and place in a single column */
  /*  Validate reference symbols and place in a single column */
  /*  The number of OFDM symbols 'N' in the implied reference grid is */
  /*  'L', the number of OFDM symbols in one slot */
  /*  Get the number of ports, based on the range of the reference */
  /*  symbol indices */
  i = refGrid->size[0] * refGrid->size[1] * refGrid->size[2];
  refGrid->size[0] = 624;
  loop_ub_tmp = (int)t1_SymbolsPerSlot;
  refGrid->size[1] = (int)t1_SymbolsPerSlot;
  refGrid->size[2] = 1;
  emxEnsureCapacity_creal_T(refGrid, i);
  loop_ub = 624 * (int)t1_SymbolsPerSlot;
  for (i = 0; i < loop_ub; i++) {
    refGrid->data[i].re = 0.0;
    refGrid->data[i].im = 0.0;
  }
  for (i = 0; i < 72; i++) {
    refGrid->data[b_iv[i]] = b_dcv[i];
  }
  /*  Extract reference indices and symbols from reference grid */
  loop_ub = 624 * refGrid->size[1];
  for (i = 0; i < loop_ub; i++) {
    refGrid_data[i] =
        ((refGrid->data[i].re != 0.0) || (refGrid->data[i].im != 0.0));
  }
  emxInit_real_T(&b_i, 1);
  b_eml_find(refGrid_data, loop_ub, tmp_data, &tmp_size);
  i = b_i->size[0];
  b_i->size[0] = tmp_size;
  emxEnsureCapacity_real_T(b_i, i);
  for (i = 0; i < tmp_size; i++) {
    b_i->data[i] = tmp_data[i];
  }
  emxInit_creal_T(&refSym, 1);
  i = refSym->size[0];
  refSym->size[0] = b_i->size[0];
  emxEnsureCapacity_creal_T(refSym, i);
  loop_ub = b_i->size[0];
  for (i = 0; i < loop_ub; i++) {
    refSym->data[i] = refGrid->data[(int)b_i->data[i] - 1];
  }
  emxFree_creal_T(&refGrid);
  /*  Validate reference grid OFDM symbol dimension, it must be equal to */
  /*  the number of OFDM symbols in the received grid */
  /*  Get channel estimation configuration parameters from options */
  /*  Get optional inputs, inputs whose position depends upon the syntax, */
  /*  or variables that depend upon optional inputs */
  /*  rxGrid: received grid */
  /*  refInd: reference signal indices */
  /*  refSym: reference signal symbols */
  /*  N: number of OFDM symbols */
  /*  P: number of transmit antenna ports */
  /*  ofdmInfo: OFDM dimensionality information */
  /*  estConfig: channel estimator configuration */
  /*  Get channel estimate output dimensions, final channel estimate will */
  /*  be of size K-by-N-by-R-by-P */
  /*  'eK' is the number of extra subcarriers that will be added to the */
  /*  channel estimate grid (half at each side), to mitigate band edge */
  /*  effects when reconstructing the channel frequency response (CFR) */
  /*  from the denoised channel impulse response (CIR) */
  /*  Create the channel estimate grid, including 'eK' extra subcarriers */
  siz[0] = 672.0;
  siz[1] = t1_SymbolsPerSlot;
  siz[2] = 1.0;
  siz[3] = 1.0;
  i = H->size[0] * H->size[1] * H->size[2] * H->size[3];
  H->size[0] = 672;
  H->size[1] = (int)t1_SymbolsPerSlot;
  H->size[2] = 1;
  H->size[3] = 1;
  emxEnsureCapacity_creal32_T(H, i);
  loop_ub = 672 * (int)t1_SymbolsPerSlot;
  for (i = 0; i < loop_ub; i++) {
    H->data[i].re = 0.0F;
    H->data[i].im = 0.0F;
  }
  /*  Create matrices 'kmin' and 'kmax' which will be used to store the */
  /*  minimum and maximum frequency subscripts, for each OFDM symbol and */
  /*  transmit port */
  if (0 <= loop_ub_tmp - 1) {
    memset(&kmin_data[0], 0, loop_ub_tmp * sizeof(double));
    memset(&kmax_data[0], 0, loop_ub_tmp * sizeof(double));
  }
  /*  Create array 'refRBs' which will be used to identify the resource */
  /*  blocks which contain reference symbols, for each OFDM symbol and */
  /*  transmit port */
  loop_ub = 56 * (int)t1_SymbolsPerSlot;
  if (0 <= loop_ub - 1) {
    memset(&refRBs_data[0], 0, loop_ub * sizeof(double));
  }
  emxInit_real_T(&kvpLow, 1);
  emxInit_real_T(&nsubs, 1);
  emxInit_real_T(&refInd, 1);
  emxInit_real_T(&b_kvpLow, 2);
  /*  --------------------------------------------------------------------- */
  /*  LS estimation, CDM despreading */
  /*  --------------------------------------------------------------------- */
  /*  Extract CDM despreading parameters */
  /*  For each transmit port */
  getPortSubscripts(t1_SymbolsPerSlot, b_i, refInd, nsubs, refGrid_data,
                    &loop_ub);
  /*  Get frequency (subcarrier k) and time (OFDM symbol n) subscripts */
  /*  of reference signal for the current port. 'thisport' is a logical */
  /*  indexing vector for the current port, used to extract the */
  /*  corresponding reference symbols */
  /*  For each OFDM symbol */
  unique_vector(nsubs, kvpLow);
  m = kvpLow->size[0];
  unique_vector(nsubs, kvpLow);
  i = b_kvpLow->size[0] * b_kvpLow->size[1];
  b_kvpLow->size[0] = 1;
  b_kvpLow->size[1] = kvpLow->size[0];
  emxEnsureCapacity_real_T(b_kvpLow, i);
  md2 = kvpLow->size[0];
  for (i = 0; i < md2; i++) {
    b_kvpLow->data[i] = kvpLow->data[i];
  }
  emxInit_real_T(&un, 2);
  i = un->size[0] * un->size[1];
  un->size[0] = 1;
  un->size[1] = m;
  emxEnsureCapacity_real_T(un, i);
  for (i = 0; i < m; i++) {
    un->data[i] = b_kvpLow->data[i];
  }
  i = un->size[1];
  if (0 <= un->size[1] - 1) {
    end = nsubs->size[0] - 1;
    b_end = loop_ub - 1;
    c_end = nsubs->size[0] - 1;
  }
  emxInit_real_T(&k, 1);
  emxInit_real_T(&ke, 1);
  emxInit_real_T(&rbsubs, 1);
  for (uni = 0; uni < i; uni++) {
    /*  Get frequency and OFDM symbol subscripts */
    n = un->data[uni];
    md2 = 0;
    m = 0;
    for (loop_ub = 0; loop_ub <= end; loop_ub++) {
      if (nsubs->data[loop_ub] == n) {
        md2++;
      }
      if (nsubs->data[loop_ub] == n) {
        c_tmp_data[m] = (short)(loop_ub + 1);
        m++;
      }
    }
    i1 = k->size[0];
    k->size[0] = md2;
    emxEnsureCapacity_real_T(k, i1);
    for (i1 = 0; i1 < md2; i1++) {
      k->data[i1] = refInd->data[c_tmp_data[i1] - 1];
    }
    /*  Initialize frequency subscripts 'ke' to be used for */
    /*  interpolation, taking extra subcarriers into account */
    i1 = ke->size[0];
    ke->size[0] = k->size[0];
    emxEnsureCapacity_real_T(ke, i1);
    loop_ub = k->size[0];
    for (i1 = 0; i1 < loop_ub; i1++) {
      ke->data[i1] = k->data[i1] + 24.0;
    }
    /*  Calculate 'kmin' and 'kmax', the minimum and maximum */
    /*  frequency subscripts for this OFDM symbol and port */
    i1 = rbsubs->size[0];
    rbsubs->size[0] = ke->size[0];
    emxEnsureCapacity_real_T(rbsubs, i1);
    loop_ub = ke->size[0];
    for (i1 = 0; i1 < loop_ub; i1++) {
      rbsubs->data[i1] = (ke->data[i1] - 1.0) / 12.0;
    }
    m = (int)un->data[uni];
    kmin_data[m - 1] = (floor(minimum(rbsubs)) * 12.0 - 24.0) + 1.0;
    kmax_data[m - 1] =
        ceil(maximum(rbsubs) + 0.083333333333333329) * 12.0 + 24.0;
    /*  Mark resource blocks (RBs) containing reference signals for */
    /*  this OFDM symbol and port in 'refRBs' */
    b_floor(rbsubs);
    unique_vector(rbsubs, kvpLow);
    tmp_size = kvpLow->size[0];
    loop_ub = kvpLow->size[0];
    for (i1 = 0; i1 < loop_ub; i1++) {
      d_tmp_data[i1] = (signed char)(kvpLow->data[i1] + 1.0);
    }
    for (i1 = 0; i1 < tmp_size; i1++) {
      refRBs_data[(d_tmp_data[i1] + 56 * (m - 1)) - 1] = 1.0;
    }
    /*  For each receive antenna */
    /*  Perform least squares (LS) estimate of channel in the */
    /*  locations of the reference symbols. 'H_LS' is a column */
    /*  vector containing the LS estimates for all subcarriers */
    /*  for the current port, OFDM symbol and receive antenna */
    m = 0;
    for (loop_ub = 0; loop_ub <= b_end; loop_ub++) {
      if (refGrid_data[loop_ub]) {
        e_tmp_data[m] = (short)(loop_ub + 1);
        m++;
      }
    }
    m = 0;
    for (loop_ub = 0; loop_ub <= c_end; loop_ub++) {
      if (nsubs->data[loop_ub] == 1.0) {
        vpiIdxHigh_data[m] = loop_ub + 1;
        m++;
      }
    }
    /*  Perform FD-CDM despreading if required */
    /*  Assign the estimates into the appropriate region of */
    /*  the overall channel estimate array */
    loop_ub = k->size[0];
    for (i1 = 0; i1 < loop_ub; i1++) {
      m = e_tmp_data[vpiIdxHigh_data[i1] - 1] - 1;
      refSym_re = (float)refSym->data[m].re;
      refSym_im = (float)refSym->data[m].im;
      m = (int)k->data[i1] - 1;
      ar = rxGrid[m].re;
      ai = rxGrid[m].im;
      if (refSym_im == 0.0F) {
        if (ai == 0.0F) {
          i2 = (int)ke->data[i1] - 1;
          H->data[i2].re = ar / refSym_re;
          H->data[i2].im = 0.0F;
        } else if (ar == 0.0F) {
          i2 = (int)ke->data[i1] - 1;
          H->data[i2].re = 0.0F;
          H->data[i2].im = ai / refSym_re;
        } else {
          i2 = (int)ke->data[i1] - 1;
          H->data[i2].re = ar / refSym_re;
          H->data[i2].im = ai / refSym_re;
        }
      } else if (refSym_re == 0.0F) {
        if (ar == 0.0F) {
          i2 = (int)ke->data[i1] - 1;
          H->data[i2].re = ai / refSym_im;
          H->data[i2].im = 0.0F;
        } else if (ai == 0.0F) {
          i2 = (int)ke->data[i1] - 1;
          H->data[i2].re = 0.0F;
          H->data[i2].im = -(ar / refSym_im);
        } else {
          i2 = (int)ke->data[i1] - 1;
          H->data[i2].re = ai / refSym_im;
          H->data[i2].im = -(ar / refSym_im);
        }
      } else {
        brm = fabsf(refSym_re);
        c = fabsf(refSym_im);
        if (brm > c) {
          s = refSym_im / refSym_re;
          c = refSym_re + s * refSym_im;
          i2 = (int)ke->data[i1] - 1;
          H->data[i2].re = (ar + s * ai) / c;
          H->data[i2].im = (ai - s * ar) / c;
        } else if (c == brm) {
          if (refSym_re > 0.0F) {
            s = 0.5F;
          } else {
            s = -0.5F;
          }
          if (refSym_im > 0.0F) {
            c = 0.5F;
          } else {
            c = -0.5F;
          }
          i2 = (int)ke->data[i1] - 1;
          H->data[i2].re = (ar * s + ai * c) / brm;
          H->data[i2].im = (ai * s - ar * c) / brm;
        } else {
          s = refSym_re / refSym_im;
          c = refSym_im + s * refSym_re;
          i2 = (int)ke->data[i1] - 1;
          H->data[i2].re = (s * ar + ai) / c;
          H->data[i2].im = (s * ai - ar) / c;
        }
      }
    }
  }
  emxFree_creal_T(&refSym);
  /*  Perform TD-CDM despreading if required */
  /*  --------------------------------------------------------------------- */
  /*  Interpolation in frequency direction, CIR denoising */
  /*  --------------------------------------------------------------------- */
  /*  Calculate minimum cyclic prefix length in terms of a DFT of size K+eK */
  n = floor(
      b_minimum(t1_CyclicPrefixLengths_data, t1_CyclicPrefixLengths_size) /
      1024.0 * 672.0);
  /*  Create time-domain windowing function for CIR denoising */
  raised_cosine_window(n * 2.0, n, b_tmp_data, &tmp_size);
  if (0 <= tmp_size - 1) {
    memcpy(&w_data[0], &b_tmp_data[0], tmp_size * sizeof(double));
  }
  md2 = 672;
  loop_ub = (int)(672.0 - (double)tmp_size);
  if (0 <= loop_ub - 1) {
    memset(&w_data[tmp_size], 0,
           ((loop_ub + tmp_size) - tmp_size) * sizeof(double));
  }
  emxInit_creal32_T(&Hn, 2);
  emxInit_real_T(&subn, 2);
  circshift(w_data, &md2, -n - floor(n / 2.0));
  /*  Create empty matrices 'Hn' and 'subn' which will be used to store */
  /*  channel estimates and subscripts used for noise estimation */
  Hn->size[0] = 0;
  Hn->size[1] = 1;
  subn->size[0] = 0;
  subn->size[1] = 4;
  /*  For each transmit port */
  b_getPortSubscripts(t1_SymbolsPerSlot, b_i, refInd, nsubs);
  /*  Get frequency (subcarrier k) and time (OFDM symbol n) subscripts */
  /*  of the reference signal for the current port */
  /*  Create interpolators ('vpisLow' and 'vpisHigh') that will be used */
  /*  to create virtual pilots (VPs) for the low and high edge of the */
  /*  grid for this port, and the corresponding sets of unique minima */
  /*  ('ukLow') and maxima ('ukHigh') frequency subscripts (one value */
  /*  for each interpolator). The interpolators use reference symbols */
  /*  from any OFDM symbol across the whole reference resource grid, */
  /*  and whose frequency subscripts are near the minimum or maximum */
  /*  frequency subscript for that interpolator */
  if (0 <= loop_ub_tmp - 1) {
    memcpy(&t1_CyclicPrefixLengths_data[0], &kmin_data[0],
           loop_ub_tmp * sizeof(double));
  }
  varargin_1[0] = -12.0;
  varargin_1[1] = 36.0;
  i = ke->size[0];
  ke->size[0] = refInd->size[0];
  emxEnsureCapacity_real_T(ke, i);
  loop_ub = refInd->size[0] - 1;
  for (i = 0; i <= loop_ub; i++) {
    ke->data[i] = refInd->data[i];
  }
  emxInit_cell_wrap_8(&vpisLow, 2);
  emxInit_real_T(&ukLow, 2);
  emxInit_real_T(&ndsLow, 2);
  createVPInterpolators(t1_CyclicPrefixLengths_data, (int)t1_SymbolsPerSlot, ke,
                        nsubs, varargin_1, vpisLow, ukLow, ndsLow);
  if (0 <= loop_ub_tmp - 1) {
    memcpy(&t1_CyclicPrefixLengths_data[0], &kmax_data[0],
           loop_ub_tmp * sizeof(double));
  }
  varargin_1[0] = -36.0;
  varargin_1[1] = 12.0;
  i = ke->size[0];
  ke->size[0] = refInd->size[0];
  emxEnsureCapacity_real_T(ke, i);
  loop_ub = refInd->size[0] - 1;
  for (i = 0; i <= loop_ub; i++) {
    ke->data[i] = refInd->data[i];
  }
  emxInit_cell_wrap_8(&vpisHigh, 2);
  emxInit_real_T(&ukHigh, 2);
  emxInit_real_T(&ndsHigh, 2);
  createVPInterpolators(t1_CyclicPrefixLengths_data, (int)t1_SymbolsPerSlot, ke,
                        nsubs, varargin_1, vpisHigh, ukHigh, ndsHigh);
  /*  For each OFDM symbol */
  unique_vector(nsubs, kvpLow);
  m = kvpLow->size[0];
  unique_vector(nsubs, kvpLow);
  i = b_kvpLow->size[0] * b_kvpLow->size[1];
  b_kvpLow->size[0] = 1;
  b_kvpLow->size[1] = kvpLow->size[0];
  emxEnsureCapacity_real_T(b_kvpLow, i);
  loop_ub = kvpLow->size[0];
  for (i = 0; i < loop_ub; i++) {
    b_kvpLow->data[i] = kvpLow->data[i];
  }
  i = un->size[0] * un->size[1];
  un->size[0] = 1;
  un->size[1] = m;
  emxEnsureCapacity_real_T(un, i);
  for (i = 0; i < m; i++) {
    un->data[i] = b_kvpLow->data[i];
  }
  i = un->size[1];
  if (0 <= un->size[1] - 1) {
    d_end = nsubs->size[0] - 1;
    b_loop_ub = ukLow->size[1];
    c_loop_ub = ukHigh->size[1];
  }
  emxInit_creal32_T(&H_LS, 1);
  emxInit_real_T(&ln, 2);
  emxInit_real_T(&pn, 2);
  emxInit_real_T(&krange, 1);
  emxInit_real_T(&vpIndicesLow, 2);
  emxInit_real_T(&vpIndicesHigh, 2);
  emxInit_creal32_T(&vpLow, 2);
  emxInit_creal32_T(&vpHigh, 2);
  emxInit_creal32_T(&Hlrp, 1);
  emxInit_real_T(&result, 2);
  emxInit_real_T(&r, 2);
  emxInit_real_T(&b_subn, 2);
  emxInit_creal32_T(&b_H, 1);
  emxInit_real_T(&c_kvpLow, 1);
  emxInit_creal32_T(&b_vpLow, 2);
  for (uni = 0; uni < i; uni++) {
    /*  Get frequency subscripts for the current OFDM symbol */
    n = un->data[uni];
    md2 = 0;
    m = 0;
    for (loop_ub = 0; loop_ub <= d_end; loop_ub++) {
      if (nsubs->data[loop_ub] == n) {
        md2++;
      }
      if (nsubs->data[loop_ub] == n) {
        f_tmp_data[m] = (short)(loop_ub + 1);
        m++;
      }
    }
    i1 = k->size[0];
    k->size[0] = md2;
    emxEnsureCapacity_real_T(k, i1);
    for (i1 = 0; i1 < md2; i1++) {
      k->data[i1] = refInd->data[f_tmp_data[i1] - 1];
    }
    /*  Prepare subscripts 'ln' and 'pn' used for noise estimation */
    varargin_1[0] = md2;
    varargin_1[1] = 1.0;
    repmat(un->data[uni], varargin_1, r);
    i1 = ln->size[0] * ln->size[1];
    ln->size[0] = r->size[0];
    ln->size[1] = r->size[1];
    emxEnsureCapacity_real_T(ln, i1);
    loop_ub = r->size[0] * r->size[1];
    for (i1 = 0; i1 < loop_ub; i1++) {
      ln->data[i1] = r->data[i1];
    }
    varargin_1[0] = md2;
    varargin_1[1] = 1.0;
    repmat(1.0, varargin_1, r);
    i1 = pn->size[0] * pn->size[1];
    pn->size[0] = r->size[0];
    pn->size[1] = r->size[1];
    emxEnsureCapacity_real_T(pn, i1);
    loop_ub = r->size[0] * r->size[1];
    for (i1 = 0; i1 < loop_ub; i1++) {
      pn->data[i1] = r->data[i1];
    }
    /*  Initialize frequency subscripts 'ke' to be used for */
    /*  interpolation, taking extra subcarriers into account. Also */
    /*  store these as 'ke0' which is used to reset these subscripts */
    /*  for each receive antenna */
    i1 = ke->size[0];
    ke->size[0] = k->size[0];
    emxEnsureCapacity_real_T(ke, i1);
    loop_ub = k->size[0];
    for (i1 = 0; i1 < loop_ub; i1++) {
      ke->data[i1] = k->data[i1] + 24.0;
    }
    /*  Calculate range of frequency subscripts for this OFDM symbol */
    /*  and port */
    tmp_size = (int)un->data[uni];
    n = kmin_data[tmp_size - 1];
    t1_Nfft = kmax_data[tmp_size - 1];
    if (rtIsNaN(n) || rtIsNaN(t1_Nfft)) {
      i1 = b_kvpLow->size[0] * b_kvpLow->size[1];
      b_kvpLow->size[0] = 1;
      b_kvpLow->size[1] = 1;
      emxEnsureCapacity_real_T(b_kvpLow, i1);
      b_kvpLow->data[0] = rtNaN;
    } else if (t1_Nfft < n) {
      b_kvpLow->size[0] = 1;
      b_kvpLow->size[1] = 0;
    } else if ((rtIsInf(n) || rtIsInf(t1_Nfft)) && (n == t1_Nfft)) {
      i1 = b_kvpLow->size[0] * b_kvpLow->size[1];
      b_kvpLow->size[0] = 1;
      b_kvpLow->size[1] = 1;
      emxEnsureCapacity_real_T(b_kvpLow, i1);
      b_kvpLow->data[0] = rtNaN;
    } else if (n == n) {
      i1 = b_kvpLow->size[0] * b_kvpLow->size[1];
      b_kvpLow->size[0] = 1;
      loop_ub = (int)(t1_Nfft - n);
      b_kvpLow->size[1] = loop_ub + 1;
      emxEnsureCapacity_real_T(b_kvpLow, i1);
      for (i1 = 0; i1 <= loop_ub; i1++) {
        b_kvpLow->data[i1] = n + (double)i1;
      }
    } else {
      eml_float_colon(n, t1_Nfft, b_kvpLow);
    }
    i1 = krange->size[0];
    krange->size[0] = b_kvpLow->size[1];
    emxEnsureCapacity_real_T(krange, i1);
    loop_ub = b_kvpLow->size[1];
    for (i1 = 0; i1 < loop_ub; i1++) {
      krange->data[i1] = b_kvpLow->data[i1];
    }
    /*  Select VP interpolators that correspond to the current OFDM */
    /*  symbol */
    ukLow_size[0] = 1;
    ukLow_size[1] = ukLow->size[1];
    for (i1 = 0; i1 < b_loop_ub; i1++) {
      refGrid_data[i1] = (ukLow->data[i1] == n);
    }
    c_eml_find(refGrid_data, ukLow_size, vpiIdxHigh_data,
               t1_CyclicPrefixLengths_size);
    loop_ub = t1_CyclicPrefixLengths_size[1];
    if (0 <= loop_ub - 1) {
      memcpy(&vpiIdxLow_data[0], &vpiIdxHigh_data[0], loop_ub * sizeof(int));
    }
    ukLow_size[0] = 1;
    ukLow_size[1] = ukHigh->size[1];
    for (i1 = 0; i1 < c_loop_ub; i1++) {
      refGrid_data[i1] = (ukHigh->data[i1] == t1_Nfft);
    }
    c_eml_find(refGrid_data, ukLow_size, vpiIdxHigh_data,
               t1_CyclicPrefixLengths_size);
    /*  Extract channel estimate regions that will be used to provide */
    /*  values for the VP interpolation for each receive antenna */
    t1_SubcarrierSpacing = un->data[uni];
    calculateVPIndices(ndsLow->data[vpiIdxLow_data[0] - 1],
                       vpisLow->data[vpiIdxLow_data[0] - 1].f1, siz,
                       t1_SubcarrierSpacing, vpIndicesLow);
    calculateVPIndices(ndsHigh->data[vpiIdxHigh_data[0] - 1],
                       vpisHigh->data[vpiIdxHigh_data[0] - 1].f1, siz,
                       t1_SubcarrierSpacing, vpIndicesHigh);
    /*  For each receive antenna */
    /*  Initialize frequency subscripts to be used for */
    /*  interpolation */
    /*  Get LS estimates (already CDM despread if required). */
    /*  'H_LS' is a column vector containing the LS estimates for */
    /*  all subcarriers for the current port, OFDM symbol and */
    /*  receive antenna */
    i1 = H_LS->size[0];
    H_LS->size[0] = ke->size[0];
    emxEnsureCapacity_creal32_T(H_LS, i1);
    loop_ub = ke->size[0];
    for (i1 = 0; i1 < loop_ub; i1++) {
      H_LS->data[i1] = H->data[((int)ke->data[i1] +
                                H->size[0] * ((int)t1_SubcarrierSpacing - 1)) -
                               1];
    }
    /*  Record LS estimates and their subscripts, for use in */
    /*  noise estimation */
    loop_ub = Hn->size[0];
    i1 = Hlrp->size[0];
    Hlrp->size[0] = Hn->size[0] + H_LS->size[0];
    emxEnsureCapacity_creal32_T(Hlrp, i1);
    for (i1 = 0; i1 < loop_ub; i1++) {
      Hlrp->data[i1] = Hn->data[i1];
    }
    loop_ub = H_LS->size[0];
    for (i1 = 0; i1 < loop_ub; i1++) {
      Hlrp->data[i1 + Hn->size[0]] = H_LS->data[i1];
    }
    i1 = Hn->size[0] * Hn->size[1];
    Hn->size[0] = Hlrp->size[0];
    Hn->size[1] = 1;
    emxEnsureCapacity_creal32_T(Hn, i1);
    loop_ub = Hlrp->size[0];
    for (i1 = 0; i1 < loop_ub; i1++) {
      Hn->data[i1] = Hlrp->data[i1];
    }
    if (md2 != 0) {
      end = md2;
    } else if (ln->size[0] != 0) {
      end = ln->size[0];
    } else if (pn->size[0] != 0) {
      end = pn->size[0];
    } else {
      end = 0;
    }
    empty_non_axis_sizes = (end == 0);
    if (empty_non_axis_sizes || (md2 != 0)) {
      i3 = 1;
    } else {
      i3 = 0;
    }
    if (empty_non_axis_sizes || (ln->size[0] != 0)) {
      i4 = 1;
    } else {
      i4 = 0;
    }
    if (empty_non_axis_sizes || ((short)md2 != 0)) {
      i5 = 1;
    } else {
      i5 = 0;
    }
    if (empty_non_axis_sizes || (pn->size[0] != 0)) {
      sizes_idx_1 = 1;
    } else {
      sizes_idx_1 = 0;
    }
    loop_ub = i3;
    i1 = result->size[0] * result->size[1];
    result->size[0] = end;
    i2 = i3 + i4;
    c_end = i2 + i5;
    result->size[1] = c_end + sizes_idx_1;
    emxEnsureCapacity_real_T(result, i1);
    for (i1 = 0; i1 < loop_ub; i1++) {
      for (m = 0; m < end; m++) {
        result->data[m] = k->data[m];
      }
    }
    loop_ub = i4;
    for (i1 = 0; i1 < loop_ub; i1++) {
      for (m = 0; m < end; m++) {
        result->data[m + result->size[0] * i3] = ln->data[m];
      }
    }
    loop_ub = i5;
    for (i1 = 0; i1 < loop_ub; i1++) {
      for (m = 0; m < end; m++) {
        result->data[m + result->size[0] * i2] = 1.0;
      }
    }
    loop_ub = sizes_idx_1;
    for (i1 = 0; i1 < loop_ub; i1++) {
      for (i2 = 0; i2 < end; i2++) {
        result->data[i2 + result->size[0] * c_end] = pn->data[i2];
      }
    }
    if (subn->size[0] != 0) {
      end = subn->size[1];
    } else if ((result->size[0] != 0) && (result->size[1] != 0)) {
      end = result->size[1];
    } else {
      end = subn->size[1];
      if (result->size[1] > subn->size[1]) {
        end = result->size[1];
      }
    }
    if (subn->size[0] != 0) {
      loop_ub = subn->size[0];
    } else {
      loop_ub = 0;
    }
    if (subn->size[0] != 0) {
      m = subn->size[0];
    } else {
      m = 0;
    }
    if ((result->size[0] != 0) && (result->size[1] != 0)) {
      md2 = (short)result->size[0];
    } else {
      md2 = 0;
    }
    i1 = b_subn->size[0] * b_subn->size[1];
    if ((result->size[0] != 0) && (result->size[1] != 0)) {
      b_end = (short)result->size[0];
    } else {
      b_end = 0;
    }
    b_subn->size[0] = loop_ub + b_end;
    b_subn->size[1] = end;
    emxEnsureCapacity_real_T(b_subn, i1);
    for (i1 = 0; i1 < end; i1++) {
      for (i2 = 0; i2 < loop_ub; i2++) {
        b_subn->data[i2 + b_subn->size[0] * i1] = subn->data[i2 + m * i1];
      }
    }
    for (i1 = 0; i1 < end; i1++) {
      for (i2 = 0; i2 < md2; i2++) {
        b_subn->data[(i2 + loop_ub) + b_subn->size[0] * i1] =
            result->data[i2 + md2 * i1];
      }
    }
    i1 = subn->size[0] * subn->size[1];
    subn->size[0] = b_subn->size[0];
    subn->size[1] = b_subn->size[1];
    emxEnsureCapacity_real_T(subn, i1);
    loop_ub = b_subn->size[0] * b_subn->size[1];
    for (i1 = 0; i1 < loop_ub; i1++) {
      subn->data[i1] = b_subn->data[i1];
    }
    /*  If FD-CDM despreading is configured, replace groups of */
    /*  LS estimates with a single estimate in the position of */
    /*  the average frequency index */
    /*  Create virtual pilots at the lower edge: every 6 */
    /*  subcarriers between the minimum frequency subscript */
    /*  'kmin' and the lowest reference signal subcarrier */
    if (rtIsNaN(n) || rtIsNaN(ke->data[0] - 1.0)) {
      i1 = b_kvpLow->size[0] * b_kvpLow->size[1];
      b_kvpLow->size[0] = 1;
      b_kvpLow->size[1] = 1;
      emxEnsureCapacity_real_T(b_kvpLow, i1);
      b_kvpLow->data[0] = rtNaN;
    } else if (ke->data[0] - 1.0 < n) {
      b_kvpLow->size[0] = 1;
      b_kvpLow->size[1] = 0;
    } else if (rtIsInf(n) && (n == ke->data[0] - 1.0)) {
      i1 = b_kvpLow->size[0] * b_kvpLow->size[1];
      b_kvpLow->size[0] = 1;
      b_kvpLow->size[1] = 1;
      emxEnsureCapacity_real_T(b_kvpLow, i1);
      b_kvpLow->data[0] = rtNaN;
    } else if (n == n) {
      i1 = b_kvpLow->size[0] * b_kvpLow->size[1];
      b_kvpLow->size[0] = 1;
      loop_ub = (int)floor(((ke->data[0] - 1.0) - n) / 6.0);
      b_kvpLow->size[1] = loop_ub + 1;
      emxEnsureCapacity_real_T(b_kvpLow, i1);
      for (i1 = 0; i1 <= loop_ub; i1++) {
        b_kvpLow->data[i1] = n + 6.0 * (double)i1;
      }
    } else {
      b_eml_float_colon(n, ke->data[0] - 1.0, b_kvpLow);
    }
    i1 = kvpLow->size[0];
    kvpLow->size[0] = b_kvpLow->size[1];
    emxEnsureCapacity_real_T(kvpLow, i1);
    loop_ub = b_kvpLow->size[1];
    for (i1 = 0; i1 < loop_ub; i1++) {
      kvpLow->data[i1] = b_kvpLow->data[i1];
    }
    loop_ub = vpIndicesLow->size[0];
    i1 = b_H->size[0];
    b_H->size[0] = vpIndicesLow->size[0];
    emxEnsureCapacity_creal32_T(b_H, i1);
    for (i1 = 0; i1 < loop_ub; i1++) {
      b_H->data[i1] = H->data[(short)vpIndicesLow->data[i1] - 1];
    }
    createVPs(ndsLow->data[vpiIdxLow_data[0] - 1],
              vpisLow->data[vpiIdxLow_data[0] - 1].f1, kvpLow, un->data[uni],
              b_H, vpLow);
    /*  Create virtual pilots at the upper edge: every 6 */
    /*  subcarriers between the maximum frequency subscript */
    /*  'kmax' and the highest reference signal subcarrier */
    if (rtIsNaN(t1_Nfft) || rtIsNaN(ke->data[ke->size[0] - 1] + 1.0)) {
      i1 = b_kvpLow->size[0] * b_kvpLow->size[1];
      b_kvpLow->size[0] = 1;
      b_kvpLow->size[1] = 1;
      emxEnsureCapacity_real_T(b_kvpLow, i1);
      b_kvpLow->data[0] = rtNaN;
    } else if (t1_Nfft < ke->data[ke->size[0] - 1] + 1.0) {
      b_kvpLow->size[0] = 1;
      b_kvpLow->size[1] = 0;
    } else if (rtIsInf(t1_Nfft) &&
               (t1_Nfft == ke->data[ke->size[0] - 1] + 1.0)) {
      i1 = b_kvpLow->size[0] * b_kvpLow->size[1];
      b_kvpLow->size[0] = 1;
      b_kvpLow->size[1] = 1;
      emxEnsureCapacity_real_T(b_kvpLow, i1);
      b_kvpLow->data[0] = rtNaN;
    } else if (t1_Nfft == t1_Nfft) {
      i1 = b_kvpLow->size[0] * b_kvpLow->size[1];
      b_kvpLow->size[0] = 1;
      b_kvpLow->size[1] =
          (int)floor(((ke->data[ke->size[0] - 1] + 1.0) - t1_Nfft) / -6.0) + 1;
      emxEnsureCapacity_real_T(b_kvpLow, i1);
      loop_ub =
          (int)floor(((ke->data[ke->size[0] - 1] + 1.0) - t1_Nfft) / -6.0);
      for (i1 = 0; i1 <= loop_ub; i1++) {
        b_kvpLow->data[i1] = t1_Nfft + -6.0 * (double)i1;
      }
    } else {
      c_eml_float_colon(t1_Nfft, ke->data[ke->size[0] - 1] + 1.0, b_kvpLow);
    }
    i1 = k->size[0];
    k->size[0] = b_kvpLow->size[1];
    emxEnsureCapacity_real_T(k, i1);
    loop_ub = b_kvpLow->size[1];
    for (i1 = 0; i1 < loop_ub; i1++) {
      k->data[i1] = b_kvpLow->data[i1];
    }
    m = k->size[0] - 1;
    md2 = k->size[0] >> 1;
    for (loop_ub = 0; loop_ub < md2; loop_ub++) {
      n = k->data[loop_ub];
      end = m - loop_ub;
      k->data[loop_ub] = k->data[end];
      k->data[end] = n;
    }
    loop_ub = vpIndicesHigh->size[0];
    i1 = b_H->size[0];
    b_H->size[0] = vpIndicesHigh->size[0];
    emxEnsureCapacity_creal32_T(b_H, i1);
    for (i1 = 0; i1 < loop_ub; i1++) {
      b_H->data[i1] = H->data[(short)vpIndicesHigh->data[i1] - 1];
    }
    createVPs(ndsHigh->data[vpiIdxHigh_data[0] - 1],
              vpisHigh->data[vpiIdxHigh_data[0] - 1].f1, k, un->data[uni], b_H,
              vpHigh);
    /*  Perform interpolation in the frequency direction to give */
    /*  a channel estimate for all relevant frequency subscripts, */
    /*  and assign the estimate into the appropriate region of */
    /*  the overall channel estimate array */
    if (vpLow->size[0] != 0) {
      loop_ub = vpLow->size[0];
    } else {
      loop_ub = 0;
    }
    if (ke->size[0] != 0) {
      sizes_idx_1 = (short)ke->size[0];
    } else {
      sizes_idx_1 = 0;
    }
    if (vpHigh->size[0] != 0) {
      md2 = vpHigh->size[0];
    } else {
      md2 = 0;
    }
    /*  Interpolate channel estimate 'H0' defined for subcarrier subscripts 'k0'
     */
    /*  and OFDM symbol subscripts 'l0', using 'method' as the interpolation */
    /*  method. 'nd' is the number of dimensions over which interpolation is */
    /*  performed. 'polar' determines whether interpolation is performed using
     */
    /*  polar (true) or Cartesian (false) co-ordinates. 'H' contains the */
    /*  interpolated channel estimate for subcarrier subscripts 'k' and OFDM */
    /*  symbol subscripts 'n' */
    /*  If the specified method is 'VP', this indicates that the */
    /*  interpolation is being performed for the virtual pilots. In this */
    /*  case, the channel estimate 'H0' is pre-processed to give a best-fit */
    /*  linear interpolation (to remove noise) prior to linearly */
    /*  extrapolating to calculate the VPs */
    /*  Interpolates channel estimate 'H0' in Cartesian co-ordinates (or real */
    /*  values), using interpolant 'ipt'. 'H' contains the interpolated channel
     */
    /*  estimate for subcarrier subscripts 'k' and OFDM symbol subscripts 'n'.
     */
    /*  'nd' is the number of dimensions over which the interpolation is */
    /*  performed and 'method' is the interpolation method */
    /*  Perform interpolation */
    /*  Pre-process VPs if applicable */
    /*  Perform 1-D interpolation using interp1 */
    i1 = c_kvpLow->size[0];
    c_kvpLow->size[0] = (kvpLow->size[0] + ke->size[0]) + k->size[0];
    emxEnsureCapacity_real_T(c_kvpLow, i1);
    m = kvpLow->size[0];
    for (i1 = 0; i1 < m; i1++) {
      c_kvpLow->data[i1] = kvpLow->data[i1];
    }
    m = ke->size[0];
    for (i1 = 0; i1 < m; i1++) {
      c_kvpLow->data[i1 + kvpLow->size[0]] = ke->data[i1];
    }
    m = k->size[0];
    for (i1 = 0; i1 < m; i1++) {
      c_kvpLow->data[(i1 + kvpLow->size[0]) + ke->size[0]] = k->data[i1];
    }
    i1 = b_vpLow->size[0] * b_vpLow->size[1];
    b_vpLow->size[0] = (loop_ub + sizes_idx_1) + md2;
    b_vpLow->size[1] = 1;
    emxEnsureCapacity_creal32_T(b_vpLow, i1);
    for (i1 = 0; i1 < loop_ub; i1++) {
      b_vpLow->data[i1] = vpLow->data[i1];
    }
    m = sizes_idx_1;
    for (i1 = 0; i1 < m; i1++) {
      b_vpLow->data[i1 + loop_ub] = H_LS->data[i1];
    }
    for (i1 = 0; i1 < md2; i1++) {
      b_vpLow->data[(i1 + loop_ub) + sizes_idx_1] = vpHigh->data[i1];
    }
    interp1(c_kvpLow, b_vpLow, krange, vpLow);
    loop_ub = vpLow->size[0];
    for (i1 = 0; i1 < loop_ub; i1++) {
      H->data[((int)krange->data[i1] + H->size[0] * (tmp_size - 1)) - 1] =
          vpLow->data[i1];
    }
    /*  Perform an IDFT of the channel estimate to give the CIR */
    t1_SubcarrierSpacing = un->data[uni];
    for (i1 = 0; i1 < 672; i1++) {
      H_data[i1] = H->data[i1 + H->size[0] * ((int)t1_SubcarrierSpacing - 1)];
    }
    ifft(H_data, 672, h_data, &m);
    /*  Apply time domain windowing function to denoise CIR */
    for (i1 = 0; i1 < 672; i1++) {
      n = w_data[i1];
      h_data[i1].re *= (float)n;
      h_data[i1].im *= (float)n;
    }
    /*  Perform a DFT of the CIR to give the denoised CFR */
    fft(h_data, 672, H_data, &m);
    for (i1 = 0; i1 < 672; i1++) {
      H->data[i1 + H->size[0] * ((int)t1_SubcarrierSpacing - 1)] = H_data[i1];
    }
  }
  emxFree_creal32_T(&b_vpLow);
  emxFree_real_T(&c_kvpLow);
  emxFree_creal32_T(&b_H);
  emxFree_real_T(&b_subn);
  emxFree_real_T(&r);
  emxFree_real_T(&result);
  emxFree_real_T(&ndsHigh);
  emxFree_real_T(&ukHigh);
  emxFree_cell_wrap_8(&vpisHigh);
  emxFree_real_T(&ndsLow);
  emxFree_real_T(&ukLow);
  emxFree_cell_wrap_8(&vpisLow);
  emxFree_creal32_T(&vpHigh);
  emxFree_creal32_T(&vpLow);
  emxFree_real_T(&vpIndicesHigh);
  emxFree_real_T(&vpIndicesLow);
  emxFree_real_T(&pn);
  emxFree_real_T(&ln);
  emxFree_creal32_T(&H_LS);
  emxFree_real_T(&ke);
  emxInit_creal32_T(&c_H, 4);
  /*  Remove extra subcarriers from the channel estimate that were added */
  /*  during interpolation, remove the corresponding resource blocks from */
  /*  'RBs' and adjust 'kmax' appropriately ('kmin' and 'kmax' are used */
  /*  subsequently to determine indices for interpolation in the time */
  /*  direction) */
  m = H->size[1] - 1;
  i = c_H->size[0] * c_H->size[1] * c_H->size[2] * c_H->size[3];
  c_H->size[0] = 624;
  c_H->size[1] = H->size[1];
  c_H->size[2] = 1;
  c_H->size[3] = 1;
  emxEnsureCapacity_creal32_T(c_H, i);
  for (i = 0; i <= m; i++) {
    for (i1 = 0; i1 < 624; i1++) {
      c_H->data[i1 + c_H->size[0] * i] = H->data[(i1 + H->size[0] * i) + 24];
    }
  }
  i = H->size[0] * H->size[1] * H->size[2] * H->size[3];
  H->size[0] = 624;
  H->size[1] = c_H->size[1];
  H->size[2] = 1;
  H->size[3] = 1;
  emxEnsureCapacity_creal32_T(H, i);
  loop_ub = 624 * c_H->size[1];
  for (i = 0; i < loop_ub; i++) {
    H->data[i] = c_H->data[i];
  }
  emxFree_creal32_T(&c_H);
  md2 = (int)t1_SymbolsPerSlot - 1;
  for (i = 0; i <= md2; i++) {
    for (i1 = 0; i1 < 52; i1++) {
      b_refRBs_data[i1 + 52 * i] = (signed char)refRBs_data[(i1 + 56 * i) + 2];
    }
  }
  refRBs_size[0] = 52;
  refRBs_size[1] = (int)t1_SymbolsPerSlot;
  refRBs_size[2] = 1;
  loop_ub = 52 * (int)t1_SymbolsPerSlot;
  for (i = 0; i < loop_ub; i++) {
    refRBs_data[i] = b_refRBs_data[i];
  }
  loop_ub = (int)t1_SymbolsPerSlot - 1;
  for (i = 0; i <= loop_ub; i++) {
    kmax_data[i] -= 48.0;
  }
  /*  --------------------------------------------------------------------- */
  /*  Noise estimation, averaging parameter selection */
  /*  --------------------------------------------------------------------- */
  /*  Perform noise estimation, by measuring the variance between the */
  /*  original LS estimates 'Hn' and the same locations in the denoised */
  /*  estimate 'Hdn'. The scaling factor 's' is empirically determined. */
  /*  Note that the accuracy of the noise estimation will be influenced by */
  /*  the number of LS estimates available */
  if (Hn->size[0] > 1) {
    emxInit_boolean_T(&r1, 1);
    /*  1st receive antenna */
    loop_ub = subn->size[0];
    i = r1->size[0];
    r1->size[0] = subn->size[0];
    emxEnsureCapacity_boolean_T(r1, i);
    for (i = 0; i < loop_ub; i++) {
      r1->data[i] = (subn->data[i + subn->size[0] * 2] == 1.0);
    }
    end = r1->size[0] - 1;
    md2 = 0;
    for (loop_ub = 0; loop_ub <= end; loop_ub++) {
      if (r1->data[loop_ub]) {
        md2++;
      }
    }
    emxInit_int32_T(&r2, 1);
    i = r2->size[0];
    r2->size[0] = md2;
    emxEnsureCapacity_int32_T(r2, i);
    m = 0;
    for (loop_ub = 0; loop_ub <= end; loop_ub++) {
      if (r1->data[loop_ub]) {
        r2->data[m] = loop_ub + 1;
        m++;
      }
    }
    emxFree_boolean_T(&r1);
    emxInit_real_T(&c_subn, 2);
    i = c_subn->size[0] * c_subn->size[1];
    c_subn->size[0] = r2->size[0];
    c_subn->size[1] = 3;
    emxEnsureCapacity_real_T(c_subn, i);
    loop_ub = r2->size[0];
    for (i = 0; i < 3; i++) {
      for (i1 = 0; i1 < loop_ub; i1++) {
        c_subn->data[i1 + c_subn->size[0] * i] =
            subn->data[(r2->data[i1] + subn->size[0] * b_iv1[i]) - 1];
      }
    }
    emxFree_int32_T(&r2);
    t1_Nfft = determineNoiseScaling(c_subn);
    m = 624 * (short)H->size[1];
    i = Hlrp->size[0];
    Hlrp->size[0] = Hn->size[0];
    emxEnsureCapacity_creal32_T(Hlrp, i);
    loop_ub = Hn->size[0];
    emxFree_real_T(&c_subn);
    for (i = 0; i < loop_ub; i++) {
      i1 = ((((int)subn->data[i] +
              624 * ((int)subn->data[i + subn->size[0]] - 1)) +
             m * ((int)subn->data[i + subn->size[0] * 2] - 1)) +
            m * ((int)subn->data[i + subn->size[0] * 3] - 1)) -
           1;
      Hlrp->data[i].re = Hn->data[i].re - H->data[i1].re;
      Hlrp->data[i].im = Hn->data[i].im - H->data[i1].im;
    }
    *nVar = var(Hlrp) * (float)t1_Nfft;
    /*  Determine how the channel CSI will affect the receiver SNR */
    /*  (assuming MMSE equalization) */
    c = determineCSIScaling(refRBs_data, refRBs_size, H, *nVar);
  } else {
    *nVar = 0.0F;
    c = 1.0F;
  }
  emxFree_real_T(&subn);
  emxFree_creal32_T(&Hn);
  /*  If either averaging parameter is not set, automatically choose a */
  /*  value based on the SNR (calculated from the noise variance and the */
  /*  effect of CSI) */
  estConfig.CDMLengths[0] = 1.0;
  estConfig.AveragingWindow[0] = 0.0;
  estConfig.CDMLengths[1] = 1.0;
  estConfig.AveragingWindow[1] = 0.0;
  setAveragingParameters(&estConfig, *nVar * c);
  /*  Return averaging parameters in info output structure */
  /*  --------------------------------------------------------------------- */
  /*  Averaging and interpolation in frequency direction */
  /*  --------------------------------------------------------------------- */
  /*  Extract frequency averaging parameter */
  n = estConfig.AveragingWindow[0];
  /*  For each transmit port */
  b_getPortSubscripts(t1_SymbolsPerSlot, b_i, refInd, nsubs);
  /*  Get time (OFDM symbol n) subscripts of reference signal for the */
  /*  current port */
  if (estConfig.AveragingWindow[0] != 1.0) {
    /*  For each OFDM symbol */
    unique_vector(nsubs, kvpLow);
    m = kvpLow->size[0];
    unique_vector(nsubs, kvpLow);
    i = b_kvpLow->size[0] * b_kvpLow->size[1];
    b_kvpLow->size[0] = 1;
    b_kvpLow->size[1] = kvpLow->size[0];
    emxEnsureCapacity_real_T(b_kvpLow, i);
    loop_ub = kvpLow->size[0];
    for (i = 0; i < loop_ub; i++) {
      b_kvpLow->data[i] = kvpLow->data[i];
    }
    i = un->size[0] * un->size[1];
    un->size[0] = 1;
    un->size[1] = m;
    emxEnsureCapacity_real_T(un, i);
    for (i = 0; i < m; i++) {
      un->data[i] = b_kvpLow->data[i];
    }
    i = un->size[1];
    emxInit_creal32_T(&r3, 1);
    for (uni = 0; uni < i; uni++) {
      /*  Get current OFDM symbol number 'n' */
      /*  For each receive antenna */
      /*  Get frequency subscripts 'k' for the current OFDM */
      /*  symbol (2 points per RB) */
      tmp_size = (int)un->data[uni];
      t1_SubcarrierSpacing = kmin_data[tmp_size - 1];
      t1_Nfft = kmax_data[tmp_size - 1];
      if (rtIsNaN(t1_SubcarrierSpacing) || rtIsNaN(t1_Nfft)) {
        i1 = b_kvpLow->size[0] * b_kvpLow->size[1];
        b_kvpLow->size[0] = 1;
        b_kvpLow->size[1] = 1;
        emxEnsureCapacity_real_T(b_kvpLow, i1);
        b_kvpLow->data[0] = rtNaN;
      } else if (t1_Nfft < t1_SubcarrierSpacing) {
        b_kvpLow->size[0] = 1;
        b_kvpLow->size[1] = 0;
      } else if ((rtIsInf(t1_SubcarrierSpacing) || rtIsInf(t1_Nfft)) &&
                 (t1_SubcarrierSpacing == t1_Nfft)) {
        i1 = b_kvpLow->size[0] * b_kvpLow->size[1];
        b_kvpLow->size[0] = 1;
        b_kvpLow->size[1] = 1;
        emxEnsureCapacity_real_T(b_kvpLow, i1);
        b_kvpLow->data[0] = rtNaN;
      } else if (t1_SubcarrierSpacing == t1_SubcarrierSpacing) {
        i1 = b_kvpLow->size[0] * b_kvpLow->size[1];
        b_kvpLow->size[0] = 1;
        loop_ub = (int)(t1_Nfft - t1_SubcarrierSpacing);
        b_kvpLow->size[1] = loop_ub + 1;
        emxEnsureCapacity_real_T(b_kvpLow, i1);
        for (i1 = 0; i1 <= loop_ub; i1++) {
          b_kvpLow->data[i1] = t1_SubcarrierSpacing + (double)i1;
        }
      } else {
        eml_float_colon(t1_SubcarrierSpacing, t1_Nfft, b_kvpLow);
      }
      i1 = krange->size[0];
      krange->size[0] = b_kvpLow->size[1];
      emxEnsureCapacity_real_T(krange, i1);
      loop_ub = b_kvpLow->size[1];
      for (i1 = 0; i1 < loop_ub; i1++) {
        krange->data[i1] = b_kvpLow->data[i1];
      }
      if (3 > krange->size[0]) {
        i1 = 0;
        i2 = 1;
        c_end = -1;
      } else {
        i1 = 2;
        i2 = 6;
        c_end = krange->size[0] - 1;
      }
      /*  Extract channel estimate for those subscripts. 'Hlrp' */
      /*  is a column vector containing the denoised estimates */
      /*  for 2 subcarriers per resource block for the current */
      /*  port, OFDM symbol and receive antenna */
      /*  Perform averaging in the frequency direction */
      loop_ub = div_s32_floor(c_end - i1, i2);
      c_end = Hlrp->size[0];
      Hlrp->size[0] = loop_ub + 1;
      emxEnsureCapacity_creal32_T(Hlrp, c_end);
      for (c_end = 0; c_end <= loop_ub; c_end++) {
        Hlrp->data[c_end] = H->data[((int)krange->data[i1 + i2 * c_end] +
                                     H->size[0] * (tmp_size - 1)) -
                                    1];
      }
      /*  Perform interpolation of estimates in the frequency */
      /*  direction to give a channel estimate for all */
      /*  subcarriers, and assign the estimate into the */
      /*  appropriate region of the overall channel estimate */
      /*  array */
      c_end = kvpLow->size[0];
      kvpLow->size[0] = loop_ub + 1;
      emxEnsureCapacity_real_T(kvpLow, c_end);
      for (c_end = 0; c_end <= loop_ub; c_end++) {
        kvpLow->data[c_end] = krange->data[i1 + i2 * c_end];
      }
      averaging(Hlrp, n, r3);
      interpolate(kvpLow, r3, krange, Hlrp);
      loop_ub = Hlrp->size[0];
      for (i1 = 0; i1 < loop_ub; i1++) {
        H->data[((int)krange->data[i1] + H->size[0] * (tmp_size - 1)) - 1] =
            Hlrp->data[i1];
      }
    }
    emxFree_creal32_T(&r3);
  }
  emxFree_real_T(&nsubs);
  emxFree_creal32_T(&Hlrp);
  emxFree_real_T(&krange);
  /*  For each OFDM symbol for this port, blank any subcarriers */
  /*  outside of the RBs that originally contained reference symbols */
  kmin_size[0] = 1;
  kmin_size[1] = (int)t1_SymbolsPerSlot;
  for (i = 0; i < loop_ub_tmp; i++) {
    b_kmin_data[i] = (kmin_data[i] != 0.0);
  }
  c_eml_find(b_kmin_data, kmin_size, vpiIdxHigh_data,
             t1_CyclicPrefixLengths_size);
  i = un->size[0] * un->size[1];
  un->size[0] = 1;
  un->size[1] = t1_CyclicPrefixLengths_size[1];
  emxEnsureCapacity_real_T(un, i);
  loop_ub = t1_CyclicPrefixLengths_size[1];
  for (i = 0; i < loop_ub; i++) {
    un->data[i] = vpiIdxHigh_data[i];
  }
  i = un->size[1];
  emxInit_real_T(&ksubs, 2);
  emxInit_int16_T(&r4, 1);
  emxInit_real_T(&r5, 2);
  for (uni = 0; uni < i; uni++) {
    for (i1 = 0; i1 < 52; i1++) {
      c_refRBs_data[i1] =
          (refRBs_data[i1 + 52 * ((int)un->data[uni] - 1)] == 0.0);
    }
    b_eml_find(c_refRBs_data, 52, tmp_data, &tmp_size);
    i1 = rbsubs->size[0];
    rbsubs->size[0] = tmp_size;
    emxEnsureCapacity_real_T(rbsubs, i1);
    for (i1 = 0; i1 < tmp_size; i1++) {
      rbsubs->data[i1] = (double)tmp_data[i1] - 1.0;
    }
    if (rbsubs->size[0] != 0) {
      loop_ub = rbsubs->size[0];
      for (i1 = 0; i1 < loop_ub; i1++) {
        rbsubs->data[i1] *= 12.0;
      }
      b_bsxfun(rbsubs, r5);
      i1 = ksubs->size[0] * ksubs->size[1];
      ksubs->size[0] = 12;
      ksubs->size[1] = r5->size[0];
      emxEnsureCapacity_real_T(ksubs, i1);
      loop_ub = r5->size[0];
      for (i1 = 0; i1 < loop_ub; i1++) {
        ksubs->data[12 * i1] = r5->data[i1];
        ksubs->data[12 * i1 + 1] = r5->data[i1 + r5->size[0]];
        ksubs->data[12 * i1 + 2] = r5->data[i1 + r5->size[0] * 2];
        ksubs->data[12 * i1 + 3] = r5->data[i1 + r5->size[0] * 3];
        ksubs->data[12 * i1 + 4] = r5->data[i1 + r5->size[0] * 4];
        ksubs->data[12 * i1 + 5] = r5->data[i1 + r5->size[0] * 5];
        ksubs->data[12 * i1 + 6] = r5->data[i1 + r5->size[0] * 6];
        ksubs->data[12 * i1 + 7] = r5->data[i1 + r5->size[0] * 7];
        ksubs->data[12 * i1 + 8] = r5->data[i1 + r5->size[0] * 8];
        ksubs->data[12 * i1 + 9] = r5->data[i1 + r5->size[0] * 9];
        ksubs->data[12 * i1 + 10] = r5->data[i1 + r5->size[0] * 10];
        ksubs->data[12 * i1 + 11] = r5->data[i1 + r5->size[0] * 11];
      }
      i1 = r4->size[0];
      r4->size[0] = 12 * ksubs->size[1];
      emxEnsureCapacity_int16_T(r4, i1);
      loop_ub = 12 * ksubs->size[1];
      for (i1 = 0; i1 < loop_ub; i1++) {
        r4->data[i1] = (short)(int)ksubs->data[i1];
      }
      loop_ub = r4->size[0];
      for (i1 = 0; i1 < loop_ub; i1++) {
        i2 = r4->data[i1] - 1;
        H->data[i2 + H->size[0] * ((int)un->data[uni] - 1)].re = 0.0F;
        H->data[i2 + H->size[0] * ((int)un->data[uni] - 1)].im = 0.0F;
      }
    }
  }
  emxFree_real_T(&r5);
  emxFree_int16_T(&r4);
  emxFree_real_T(&ksubs);
  emxFree_real_T(&rbsubs);
  /*  --------------------------------------------------------------------- */
  /*  Averaging and interpolation in time direction */
  /*  --------------------------------------------------------------------- */
  /*  Extract time averaging parameter */
  timeAveraging = estConfig.AveragingWindow[1];
  /*  For each transmit port */
  loop_ub = (int)t1_SymbolsPerSlot;
  /*  Establish labels 'z' for the OFDM symbols containing reference */
  /*  symbols in each RB for this port. The labels are integers which */
  /*  identify different OFDM symbol sets. 'z' is a column vector */
  /*  containing one label for each RB */
  sizes_idx_1 = 52;
  for (i = 0; i < loop_ub; i++) {
    for (i1 = 0; i1 < 52; i1++) {
      md2 = i1 + 52 * i;
      d_refRBs_data[md2] = (signed char)refRBs_data[md2];
    }
  }
  emxInit_real_T(&r6, 2);
  m = sizes_idx_1;
  i = r6->size[0] * r6->size[1];
  r6->size[0] = sizes_idx_1 + 1;
  r6->size[1] = (int)t1_SymbolsPerSlot;
  emxEnsureCapacity_real_T(r6, i);
  for (i = 0; i < loop_ub_tmp; i++) {
    for (i1 = 0; i1 < 1; i1++) {
      r6->data[r6->size[0] * i] = 0.0;
    }
  }
  for (i = 0; i < loop_ub_tmp; i++) {
    for (i1 = 0; i1 < m; i1++) {
      r6->data[(i1 + r6->size[0] * i) + 1] =
          d_refRBs_data[i1 + sizes_idx_1 * i];
    }
  }
  b_unique_rows((double *)r6->data, *(int(*)[2])r6->size, b_data,
                t1_CyclicPrefixLengths_size, indx_data, &m, ipos_data, &md2);
  z_size = md2;
  emxFree_real_T(&r6);
  for (i = 0; i < md2; i++) {
    z_data[i] = ipos_data[i];
  }
  if (2 > z_size) {
    i = 0;
    i1 = 0;
  } else {
    i = 1;
    i1 = z_size;
  }
  loop_ub_tmp = i1 - i;
  for (i1 = 0; i1 < loop_ub_tmp; i1++) {
    z_data[i1] = z_data[i + i1] - 1.0;
  }
  z_size = loop_ub_tmp;
  /*  Form unique set of labels 'uz', corresponding to unique OFDM */
  /*  symbol sets, excluding the case of the empty set of OFDM symbols */
  /*  (having label 0) */
  b_z_data.data = &z_data[0];
  b_z_data.size = &z_size;
  b_z_data.allocatedSize = 53;
  b_z_data.numDimensions = 1;
  b_z_data.canFreeData = false;
  t1_Nfft = maximum(&b_z_data);
  emxInit_real_T(&uz, 2);
  if (rtIsNaN(t1_Nfft)) {
    i = uz->size[0] * uz->size[1];
    uz->size[0] = 1;
    uz->size[1] = 1;
    emxEnsureCapacity_real_T(uz, i);
    uz->data[0] = rtNaN;
  } else if (t1_Nfft < 1.0) {
    uz->size[0] = 1;
    uz->size[1] = 0;
  } else if (rtIsInf(t1_Nfft) && (1.0 == t1_Nfft)) {
    i = uz->size[0] * uz->size[1];
    uz->size[0] = 1;
    uz->size[1] = 1;
    emxEnsureCapacity_real_T(uz, i);
    uz->data[0] = rtNaN;
  } else {
    i = uz->size[0] * uz->size[1];
    uz->size[0] = 1;
    loop_ub = (int)floor(t1_Nfft - 1.0);
    uz->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(uz, i);
    for (i = 0; i <= loop_ub; i++) {
      uz->data[i] = (double)i + 1.0;
    }
  }
  /*  For each unique OFDM symbol set */
  i = uz->size[1];
  if (0 <= uz->size[1] - 1) {
    d_loop_ub = loop_ub_tmp;
    i6 = (int)t1_SymbolsPerSlot;
    e_loop_ub = (int)t1_SymbolsPerSlot;
    i7 = (int)t1_SymbolsPerSlot;
    f_loop_ub = (int)t1_SymbolsPerSlot;
  }
  emxInit_real_T(&rbidx, 2);
  emxInit_creal32_T(&Hrp, 2);
  emxInit_creal32_T(&r7, 2);
  emxInit_creal32_T(&r8, 2);
  emxInit_real_T(&r9, 1);
  emxInit_real_T(&c_i, 2);
  emxInit_creal32_T(&b_Hrp, 2);
  emxInit_creal32_T(&c_Hrp, 2);
  emxInit_creal32_T(&d_Hrp, 2);
  for (b_end = 0; b_end < i; b_end++) {
    /*  Get RBs having this OFDM symbol set */
    n = uz->data[b_end];
    for (i1 = 0; i1 < d_loop_ub; i1++) {
      c_z_data[i1] = (z_data[i1] == n);
    }
    b_eml_find(c_z_data, loop_ub_tmp, tmp_data, &tmp_size);
    i1 = refInd->size[0];
    refInd->size[0] = tmp_size;
    emxEnsureCapacity_real_T(refInd, i1);
    for (i1 = 0; i1 < tmp_size; i1++) {
      refInd->data[i1] = (double)tmp_data[i1] - 1.0;
    }
    /*  Calculate OFDM symbol subscripts 'n' for this OFDM symbol */
    /*  set. Also store these as 'n0' which is used to reset these */
    /*  subscripts for each RB block / receive antenna */
    kmin_size[0] = 1;
    kmin_size[1] = i6;
    for (i1 = 0; i1 < e_loop_ub; i1++) {
      b_kmin_data[i1] =
          (refRBs_data[((int)(refInd->data[0] + 1.0) + 52 * i1) - 1] != 0.0);
    }
    c_eml_find(b_kmin_data, kmin_size, vpiIdxHigh_data,
               t1_CyclicPrefixLengths_size);
    i1 = un->size[0] * un->size[1];
    un->size[0] = 1;
    loop_ub = t1_CyclicPrefixLengths_size[1];
    un->size[1] = t1_CyclicPrefixLengths_size[1];
    emxEnsureCapacity_real_T(un, i1);
    for (i1 = 0; i1 < loop_ub; i1++) {
      un->data[i1] = vpiIdxHigh_data[i1];
    }
    kmin_size[0] = 1;
    kmin_size[1] = i7;
    for (i1 = 0; i1 < f_loop_ub; i1++) {
      b_kmin_data[i1] =
          (refRBs_data[((int)(refInd->data[0] + 1.0) + 52 * i1) - 1] != 0.0);
    }
    c_eml_find(b_kmin_data, kmin_size, vpiIdxHigh_data,
               t1_CyclicPrefixLengths_size);
    end = t1_CyclicPrefixLengths_size[1];
    /*  Split into blocks of contiguous RBs. Each column of 'rbidx' */
    /*  will have two rows, giving the index of the first and last RB */
    /*  of the block within 'RBs' */
    i1 = r9->size[0];
    r9->size[0] = refInd->size[0] + 2;
    emxEnsureCapacity_real_T(r9, i1);
    r9->data[0] = -2.0;
    loop_ub = refInd->size[0];
    for (i1 = 0; i1 < loop_ub; i1++) {
      r9->data[i1 + 1] = refInd->data[i1];
    }
    r9->data[refInd->size[0] + 1] = 53.0;
    diff(r9, kvpLow);
    m = kvpLow->size[0];
    loop_ub = kvpLow->size[0];
    for (i1 = 0; i1 < loop_ub; i1++) {
      kvpLow_data[i1] = (kvpLow->data[i1] != 1.0);
    }
    b_eml_find(kvpLow_data, m, tmp_data, &tmp_size);
    i1 = b_i->size[0];
    b_i->size[0] = tmp_size;
    emxEnsureCapacity_real_T(b_i, i1);
    for (i1 = 0; i1 < tmp_size; i1++) {
      b_i->data[i1] = tmp_data[i1];
    }
    i1 = r9->size[0];
    r9->size[0] = refInd->size[0] + 2;
    emxEnsureCapacity_real_T(r9, i1);
    r9->data[0] = -2.0;
    loop_ub = refInd->size[0];
    for (i1 = 0; i1 < loop_ub; i1++) {
      r9->data[i1 + 1] = refInd->data[i1];
    }
    r9->data[refInd->size[0] + 1] = 53.0;
    diff(r9, kvpLow);
    m = kvpLow->size[0];
    loop_ub = kvpLow->size[0];
    for (i1 = 0; i1 < loop_ub; i1++) {
      kvpLow_data[i1] = (kvpLow->data[i1] != 1.0);
    }
    b_eml_find(kvpLow_data, m, tmp_data, &tmp_size);
    if (1 > b_i->size[0] - 1) {
      loop_ub = 0;
    } else {
      loop_ub = tmp_size - 1;
    }
    if (2 > b_i->size[0]) {
      i1 = 0;
      tmp_size = 0;
    } else {
      i1 = 1;
    }
    i2 = c_i->size[0] * c_i->size[1];
    c_i->size[0] = 2;
    c_i->size[1] = loop_ub;
    emxEnsureCapacity_real_T(c_i, i2);
    for (i2 = 0; i2 < loop_ub; i2++) {
      c_i->data[2 * i2] = (int)b_i->data[i2];
    }
    md2 = tmp_size - i1;
    for (i2 = 0; i2 < md2; i2++) {
      c_i->data[2 * i2 + 1] = (double)(int)b_i->data[i1 + i2] - 1.0;
    }
    i1 = rbidx->size[0] * rbidx->size[1];
    rbidx->size[0] = 2;
    rbidx->size[1] = loop_ub;
    emxEnsureCapacity_real_T(rbidx, i1);
    md2 = loop_ub << 1;
    for (i1 = 0; i1 < md2; i1++) {
      rbidx->data[i1] = c_i->data[i1];
    }
    /*  For each contiguous block of RBs */
    i1 = rbidx->size[1];
    for (tmp_size = 0; tmp_size < i1; tmp_size++) {
      /*  Calculate frequency subscripts 'k' for the block */
      t1_SubcarrierSpacing =
          refInd->data[(int)rbidx->data[2 * tmp_size] - 1] * 12.0 + 1.0;
      t1_Nfft =
          refInd->data[(int)rbidx->data[2 * tmp_size + 1] - 1] * 12.0 + 12.0;
      if (t1_Nfft < t1_SubcarrierSpacing) {
        b_kvpLow->size[0] = 1;
        b_kvpLow->size[1] = 0;
      } else if (t1_SubcarrierSpacing == t1_SubcarrierSpacing) {
        i2 = b_kvpLow->size[0] * b_kvpLow->size[1];
        b_kvpLow->size[0] = 1;
        loop_ub = (int)(t1_Nfft - t1_SubcarrierSpacing);
        b_kvpLow->size[1] = loop_ub + 1;
        emxEnsureCapacity_real_T(b_kvpLow, i2);
        for (i2 = 0; i2 <= loop_ub; i2++) {
          b_kvpLow->data[i2] = t1_SubcarrierSpacing + (double)i2;
        }
      } else {
        eml_float_colon(t1_SubcarrierSpacing, t1_Nfft, b_kvpLow);
      }
      i2 = k->size[0];
      k->size[0] = b_kvpLow->size[1];
      emxEnsureCapacity_real_T(k, i2);
      loop_ub = b_kvpLow->size[1];
      for (i2 = 0; i2 < loop_ub; i2++) {
        k->data[i2] = b_kvpLow->data[i2];
      }
      /*  For each receive antenna */
      /*  Initialize OFDM symbol subscripts 'n' to be used for */
      /*  interpolation */
      /*  Extract channel estimate. 'Hrp' is a matrix of */
      /*  denoised and frequency averaged / interpolated */
      /*  estimates for the current RB block, port and receive */
      /*  antenna. The rows of 'Hrp' correspond to the */
      /*  subcarriers within the current RB block, the columns */
      /*  to OFDM symbols containing reference symbols */
      i2 = Hrp->size[0] * Hrp->size[1];
      Hrp->size[0] = k->size[0];
      loop_ub = un->size[1];
      Hrp->size[1] = un->size[1];
      emxEnsureCapacity_creal32_T(Hrp, i2);
      for (i2 = 0; i2 < loop_ub; i2++) {
        md2 = k->size[0];
        for (c_end = 0; c_end < md2; c_end++) {
          Hrp->data[c_end + Hrp->size[0] * i2] =
              H->data[((int)k->data[c_end] +
                       H->size[0] * ((int)un->data[i2] - 1)) -
                      1];
        }
      }
      /*  If TD-CDM despreading is configured, replace groups */
      /*  of estimates with a single estimate in the position */
      /*  of the average OFDM symbol index */
      /*  Perform averaging in the time direction */
      if (timeAveraging != 1.0) {
        i2 = b_Hrp->size[0] * b_Hrp->size[1];
        b_Hrp->size[0] = Hrp->size[1];
        b_Hrp->size[1] = Hrp->size[0];
        emxEnsureCapacity_creal32_T(b_Hrp, i2);
        loop_ub = Hrp->size[0];
        for (i2 = 0; i2 < loop_ub; i2++) {
          md2 = Hrp->size[1];
          for (c_end = 0; c_end < md2; c_end++) {
            b_Hrp->data[c_end + b_Hrp->size[0] * i2] =
                Hrp->data[i2 + Hrp->size[0] * c_end];
          }
        }
        m = k->size[0];
        e_Hrp = *b_Hrp;
        b_result[0] = end;
        b_result[1] = m;
        e_Hrp.size = &b_result[0];
        e_Hrp.numDimensions = 2;
        b_averaging(&e_Hrp, timeAveraging, r7);
        i2 = Hrp->size[0] * Hrp->size[1];
        Hrp->size[0] = r7->size[1];
        Hrp->size[1] = r7->size[0];
        emxEnsureCapacity_creal32_T(Hrp, i2);
        loop_ub = r7->size[0];
        for (i2 = 0; i2 < loop_ub; i2++) {
          md2 = r7->size[1];
          for (c_end = 0; c_end < md2; c_end++) {
            Hrp->data[c_end + Hrp->size[0] * i2] =
                r7->data[i2 + r7->size[0] * c_end];
          }
        }
      }
      /*  Obtain a channel estimate for all OFDM symbols. After */
      /*  this step, the columns of 'Hrp' correspond to all */
      /*  OFDM symbols in the slot */
      if (un->size[1] > 1) {
        /*  For multiple reference OFDM symbols, perform 2-D */
        /*  interpolation of estimates */
        if (rtIsNaN(t1_SymbolsPerSlot)) {
          i2 = b_kvpLow->size[0] * b_kvpLow->size[1];
          b_kvpLow->size[0] = 1;
          b_kvpLow->size[1] = 1;
          emxEnsureCapacity_real_T(b_kvpLow, i2);
          b_kvpLow->data[0] = rtNaN;
        } else if (t1_SymbolsPerSlot < 1.0) {
          b_kvpLow->size[0] = 1;
          b_kvpLow->size[1] = 0;
        } else if (rtIsInf(t1_SymbolsPerSlot) && (1.0 == t1_SymbolsPerSlot)) {
          i2 = b_kvpLow->size[0] * b_kvpLow->size[1];
          b_kvpLow->size[0] = 1;
          b_kvpLow->size[1] = 1;
          emxEnsureCapacity_real_T(b_kvpLow, i2);
          b_kvpLow->data[0] = rtNaN;
        } else {
          i2 = b_kvpLow->size[0] * b_kvpLow->size[1];
          b_kvpLow->size[0] = 1;
          loop_ub = (int)floor(t1_SymbolsPerSlot - 1.0);
          b_kvpLow->size[1] = loop_ub + 1;
          emxEnsureCapacity_real_T(b_kvpLow, i2);
          for (i2 = 0; i2 <= loop_ub; i2++) {
            b_kvpLow->data[i2] = (double)i2 + 1.0;
          }
        }
        un_size[0] = 1;
        un_size[1] = un->size[1];
        loop_ub = un->size[0] * un->size[1] - 1;
        for (i2 = 0; i2 <= loop_ub; i2++) {
          un_data[i2] = un->data[i2];
        }
        i2 = d_Hrp->size[0] * d_Hrp->size[1];
        d_Hrp->size[0] = Hrp->size[0];
        d_Hrp->size[1] = Hrp->size[1];
        emxEnsureCapacity_creal32_T(d_Hrp, i2);
        loop_ub = Hrp->size[0] * Hrp->size[1] - 1;
        for (i2 = 0; i2 <= loop_ub; i2++) {
          d_Hrp->data[i2] = Hrp->data[i2];
        }
        t1_CyclicPrefixLengths_size[0] = un_size[0];
        t1_CyclicPrefixLengths_size[1] = un_size[1];
        b_interpolate(k, un_data, t1_CyclicPrefixLengths_size, d_Hrp, k,
                      (double *)b_kvpLow->data, *(int(*)[2])b_kvpLow->size, r8);
        i2 = Hrp->size[0] * Hrp->size[1];
        Hrp->size[0] = r8->size[0];
        Hrp->size[1] = r8->size[1];
        emxEnsureCapacity_creal32_T(Hrp, i2);
        loop_ub = r8->size[0] * r8->size[1];
        for (i2 = 0; i2 < loop_ub; i2++) {
          Hrp->data[i2] = r8->data[i2];
        }
      } else {
        /*  For a single reference OFDM symbol, repeat the */
        /*  single estimate */
        varargin_1[0] = 1.0;
        varargin_1[1] = t1_SymbolsPerSlot;
        i2 = c_Hrp->size[0] * c_Hrp->size[1];
        c_Hrp->size[0] = Hrp->size[0];
        c_Hrp->size[1] = Hrp->size[1];
        emxEnsureCapacity_creal32_T(c_Hrp, i2);
        loop_ub = Hrp->size[0] * Hrp->size[1] - 1;
        for (i2 = 0; i2 <= loop_ub; i2++) {
          c_Hrp->data[i2] = Hrp->data[i2];
        }
        b_repmat(c_Hrp, varargin_1, Hrp);
      }
      /*  Assign the estimate into the appropriate region of */
      /*  the overall channel estimate array */
      loop_ub = Hrp->size[1];
      for (i2 = 0; i2 < loop_ub; i2++) {
        md2 = Hrp->size[0];
        for (c_end = 0; c_end < md2; c_end++) {
          H->data[((int)k->data[c_end] + H->size[0] * i2) - 1] =
              Hrp->data[c_end + Hrp->size[0] * i2];
        }
      }
    }
  }
  emxFree_creal32_T(&d_Hrp);
  emxFree_creal32_T(&c_Hrp);
  emxFree_creal32_T(&b_Hrp);
  emxFree_real_T(&c_i);
  emxFree_real_T(&r9);
  emxFree_real_T(&b_kvpLow);
  emxFree_creal32_T(&r8);
  emxFree_creal32_T(&r7);
  emxFree_real_T(&b_i);
  emxFree_real_T(&refInd);
  emxFree_creal32_T(&Hrp);
  emxFree_real_T(&rbidx);
  emxFree_real_T(&uz);
  emxFree_real_T(&kvpLow);
  emxFree_real_T(&k);
  emxFree_real_T(&un);
}

/*
 * File trailer for nrChannelEstimateOpt.c
 *
 * [EOF]
 */

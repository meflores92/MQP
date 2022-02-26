/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: polar_decode.c
 *
 * MATLAB Coder version            : 5.2
 * C/C++ source code generated on  : 20-Feb-2022 08:55:56
 */

/* Include Files */
#include "polar_decode.h"
#include "rt_nonfinite.h"
#include <math.h>
#include <string.h>

/* Function Declarations */
static void b_polar_decode(double u_data[], int u_size[2], double v[32]);

static void c_polar_decode(double u_data[], int u_size[2], double v[16]);

static void d_polar_decode(double u_data[], int u_size[2], double v[8]);

static void e_polar_decode(double u_data[], int u_size[2], double v[4]);

static void g_polar_decode(const double y[64],
                           const double frozen_bits_indicator[64],
                           double u_data[], int u_size[2], double v[64]);

static void h_polar_decode(const double y[32],
                           const double frozen_bits_indicator[32],
                           double u_data[], int u_size[2], double v[32]);

static void i_polar_decode(const double y[16],
                           const double frozen_bits_indicator[16],
                           double u_data[], int u_size[2], double v[16]);

static void j_polar_decode(const double y[8],
                           const double frozen_bits_indicator[8],
                           double u_data[], int u_size[2], double v[8]);

static void k_polar_decode(const double y[4],
                           const double frozen_bits_indicator[4],
                           double u_data[], int u_size[2], double v[4]);

static void l_polar_decode(const double y[2],
                           const double frozen_bits_indicator[2],
                           double u_data[], int u_size[2], double v_data[],
                           int v_size[2]);

/* Function Definitions */
/*
 * Arguments    : double u_data[]
 *                int u_size[2]
 *                double v[32]
 * Return Type  : void
 */
static void b_polar_decode(double u_data[], int u_size[2], double v[32])
{
  double u_1_data[16];
  double u_2_data[16];
  double v_1[16];
  double v_2[16];
  int u_1_size[2];
  c_polar_decode(u_1_data, u_1_size, v_1);
  c_polar_decode(u_2_data, u_1_size, v_2);
  u_size[0] = 1;
  u_size[1] = 32;
  u_data[0] = u_1_data[0];
  u_data[16] = u_2_data[0];
  v[0] = (double)((unsigned long)v_1[0] ^ (unsigned long)v_2[0]);
  v[1] = v_2[0];
  u_data[1] = u_1_data[1];
  u_data[17] = u_2_data[1];
  v[2] = (double)((unsigned long)v_1[1] ^ (unsigned long)v_2[1]);
  v[3] = v_2[1];
  u_data[2] = u_1_data[2];
  u_data[18] = u_2_data[2];
  v[4] = (double)((unsigned long)v_1[2] ^ (unsigned long)v_2[2]);
  v[5] = v_2[2];
  u_data[3] = u_1_data[3];
  u_data[19] = u_2_data[3];
  v[6] = (double)((unsigned long)v_1[3] ^ (unsigned long)v_2[3]);
  v[7] = v_2[3];
  u_data[4] = u_1_data[4];
  u_data[20] = u_2_data[4];
  v[8] = (double)((unsigned long)v_1[4] ^ (unsigned long)v_2[4]);
  v[9] = v_2[4];
  u_data[5] = u_1_data[5];
  u_data[21] = u_2_data[5];
  v[10] = (double)((unsigned long)v_1[5] ^ (unsigned long)v_2[5]);
  v[11] = v_2[5];
  u_data[6] = u_1_data[6];
  u_data[22] = u_2_data[6];
  v[12] = (double)((unsigned long)v_1[6] ^ (unsigned long)v_2[6]);
  v[13] = v_2[6];
  u_data[7] = u_1_data[7];
  u_data[23] = u_2_data[7];
  v[14] = (double)((unsigned long)v_1[7] ^ (unsigned long)v_2[7]);
  v[15] = v_2[7];
  u_data[8] = u_1_data[8];
  u_data[24] = u_2_data[8];
  v[16] = (double)((unsigned long)v_1[8] ^ (unsigned long)v_2[8]);
  v[17] = v_2[8];
  u_data[9] = u_1_data[9];
  u_data[25] = u_2_data[9];
  v[18] = (double)((unsigned long)v_1[9] ^ (unsigned long)v_2[9]);
  v[19] = v_2[9];
  u_data[10] = u_1_data[10];
  u_data[26] = u_2_data[10];
  v[20] = (double)((unsigned long)v_1[10] ^ (unsigned long)v_2[10]);
  v[21] = v_2[10];
  u_data[11] = u_1_data[11];
  u_data[27] = u_2_data[11];
  v[22] = (double)((unsigned long)v_1[11] ^ (unsigned long)v_2[11]);
  v[23] = v_2[11];
  u_data[12] = u_1_data[12];
  u_data[28] = u_2_data[12];
  v[24] = (double)((unsigned long)v_1[12] ^ (unsigned long)v_2[12]);
  v[25] = v_2[12];
  u_data[13] = u_1_data[13];
  u_data[29] = u_2_data[13];
  v[26] = (double)((unsigned long)v_1[13] ^ (unsigned long)v_2[13]);
  v[27] = v_2[13];
  u_data[14] = u_1_data[14];
  u_data[30] = u_2_data[14];
  v[28] = (double)((unsigned long)v_1[14] ^ (unsigned long)v_2[14]);
  v[29] = v_2[14];
  u_data[15] = u_1_data[15];
  u_data[31] = u_2_data[15];
  v[30] = (double)((unsigned long)v_1[15] ^ (unsigned long)v_2[15]);
  v[31] = v_2[15];
}

/*
 * Arguments    : double u_data[]
 *                int u_size[2]
 *                double v[16]
 * Return Type  : void
 */
static void c_polar_decode(double u_data[], int u_size[2], double v[16])
{
  double u_1_data[8];
  double u_2_data[8];
  double v_1[8];
  double v_2[8];
  int u_1_size[2];
  d_polar_decode(u_1_data, u_1_size, v_1);
  d_polar_decode(u_2_data, u_1_size, v_2);
  u_size[0] = 1;
  u_size[1] = 16;
  u_data[0] = u_1_data[0];
  u_data[8] = u_2_data[0];
  v[0] = (double)((unsigned long)v_1[0] ^ (unsigned long)v_2[0]);
  v[1] = v_2[0];
  u_data[1] = u_1_data[1];
  u_data[9] = u_2_data[1];
  v[2] = (double)((unsigned long)v_1[1] ^ (unsigned long)v_2[1]);
  v[3] = v_2[1];
  u_data[2] = u_1_data[2];
  u_data[10] = u_2_data[2];
  v[4] = (double)((unsigned long)v_1[2] ^ (unsigned long)v_2[2]);
  v[5] = v_2[2];
  u_data[3] = u_1_data[3];
  u_data[11] = u_2_data[3];
  v[6] = (double)((unsigned long)v_1[3] ^ (unsigned long)v_2[3]);
  v[7] = v_2[3];
  u_data[4] = u_1_data[4];
  u_data[12] = u_2_data[4];
  v[8] = (double)((unsigned long)v_1[4] ^ (unsigned long)v_2[4]);
  v[9] = v_2[4];
  u_data[5] = u_1_data[5];
  u_data[13] = u_2_data[5];
  v[10] = (double)((unsigned long)v_1[5] ^ (unsigned long)v_2[5]);
  v[11] = v_2[5];
  u_data[6] = u_1_data[6];
  u_data[14] = u_2_data[6];
  v[12] = (double)((unsigned long)v_1[6] ^ (unsigned long)v_2[6]);
  v[13] = v_2[6];
  u_data[7] = u_1_data[7];
  u_data[15] = u_2_data[7];
  v[14] = (double)((unsigned long)v_1[7] ^ (unsigned long)v_2[7]);
  v[15] = v_2[7];
}

/*
 * Arguments    : double u_data[]
 *                int u_size[2]
 *                double v[8]
 * Return Type  : void
 */
static void d_polar_decode(double u_data[], int u_size[2], double v[8])
{
  double u_1_data[4];
  double u_2_data[4];
  double v_1[4];
  double v_2[4];
  int u_1_size[2];
  e_polar_decode(u_1_data, u_1_size, v_1);
  e_polar_decode(u_2_data, u_1_size, v_2);
  u_size[0] = 1;
  u_size[1] = 8;
  u_data[0] = u_1_data[0];
  u_data[4] = u_2_data[0];
  v[0] = (double)((unsigned long)v_1[0] ^ (unsigned long)v_2[0]);
  v[1] = v_2[0];
  u_data[1] = u_1_data[1];
  u_data[5] = u_2_data[1];
  v[2] = (double)((unsigned long)v_1[1] ^ (unsigned long)v_2[1]);
  v[3] = v_2[1];
  u_data[2] = u_1_data[2];
  u_data[6] = u_2_data[2];
  v[4] = (double)((unsigned long)v_1[2] ^ (unsigned long)v_2[2]);
  v[5] = v_2[2];
  u_data[3] = u_1_data[3];
  u_data[7] = u_2_data[3];
  v[6] = (double)((unsigned long)v_1[3] ^ (unsigned long)v_2[3]);
  v[7] = v_2[3];
}

/*
 * Arguments    : double u_data[]
 *                int u_size[2]
 *                double v[4]
 * Return Type  : void
 */
static void e_polar_decode(double u_data[], int u_size[2], double v[4])
{
  u_size[0] = 1;
  u_size[1] = 4;
  u_data[0] = 0.0;
  u_data[2] = 0.0;
  v[0] = 0.0;
  v[1] = 0.0;
  u_data[1] = 0.0;
  u_data[3] = 0.0;
  v[2] = 0.0;
  v[3] = 0.0;
}

/*
 * Arguments    : const double y[64]
 *                const double frozen_bits_indicator[64]
 *                double u_data[]
 *                int u_size[2]
 *                double v[64]
 * Return Type  : void
 */
static void g_polar_decode(const double y[64],
                           const double frozen_bits_indicator[64],
                           double u_data[], int u_size[2], double v[64])
{
  double L_w_odd[32];
  double u_1_data[32];
  double u_2_data[32];
  double v_1[32];
  double v_2[32];
  double b_x;
  double b_y;
  double c_x;
  double result;
  double x;
  int u_1_size[2];
  int b_index;
  int x_tmp;
  for (b_index = 0; b_index < 32; b_index++) {
    x_tmp = (b_index + 1) << 1;
    x = y[x_tmp - 2];
    b_y = y[x_tmp - 1];
    /*  result = log((exp(x+y) + 1)/(exp(x) + exp(y))); */
    /*  result = 2*atanh(tanh(x/2) * tanh(y/2)); */
    /*  function result = f(p, q) */
    /*  result = (p * q + 1)/(p + q); */
    /*  end */
    b_x = x;
    if (x < 0.0) {
      b_x = -1.0;
    } else if (x > 0.0) {
      b_x = 1.0;
    } else if (x == 0.0) {
      b_x = 0.0;
    }
    c_x = b_y;
    if (b_y < 0.0) {
      c_x = -1.0;
    } else if (b_y > 0.0) {
      c_x = 1.0;
    } else if (b_y == 0.0) {
      c_x = 0.0;
    }
    L_w_odd[b_index] = b_x * c_x * fmin(fabs(x), fabs(b_y));
  }
  h_polar_decode(L_w_odd, *(double(*)[32]) & frozen_bits_indicator[0], u_1_data,
                 u_1_size, v_1);
  for (b_index = 0; b_index < 32; b_index++) {
    x_tmp = (b_index + 1) << 1;
    x = y[x_tmp - 2];
    b_y = y[x_tmp - 1];
    /*  function result = g(p, q, u) */
    /*  result = (p^(1-2*u)) * q; */
    /*  end */
    b_x = v_1[b_index];
    if (b_x == 0.0) {
      result = x + b_y;
    } else if (b_x == 1.0) {
      result = b_y - x;
    }
    L_w_odd[b_index] = result;
  }
  h_polar_decode(L_w_odd, *(double(*)[32]) & frozen_bits_indicator[32],
                 u_2_data, u_1_size, v_2);
  u_size[0] = 1;
  u_size[1] = 64;
  for (b_index = 0; b_index < 32; b_index++) {
    u_data[b_index] = u_1_data[b_index];
    u_data[b_index + 32] = u_2_data[b_index];
    x_tmp = (b_index + 1) << 1;
    b_x = v_2[b_index];
    v[x_tmp - 2] = (double)((unsigned long)v_1[b_index] ^ (unsigned long)b_x);
    v[x_tmp - 1] = b_x;
  }
}

/*
 * Arguments    : const double y[32]
 *                const double frozen_bits_indicator[32]
 *                double u_data[]
 *                int u_size[2]
 *                double v[32]
 * Return Type  : void
 */
static void h_polar_decode(const double y[32],
                           const double frozen_bits_indicator[32],
                           double u_data[], int u_size[2], double v[32])
{
  double L_w_odd[16];
  double u_1_data[16];
  double u_2_data[16];
  double v_1[16];
  double v_2[16];
  double b_x;
  double result;
  double x;
  int u_1_size[2];
  /*  result = log((exp(x+y) + 1)/(exp(x) + exp(y))); */
  /*  result = 2*atanh(tanh(x/2) * tanh(y/2)); */
  /*  function result = f(p, q) */
  /*  result = (p * q + 1)/(p + q); */
  /*  end */
  x = y[0];
  if (y[0] < 0.0) {
    x = -1.0;
  } else if (y[0] > 0.0) {
    x = 1.0;
  } else if (y[0] == 0.0) {
    x = 0.0;
  }
  b_x = y[1];
  if (y[1] < 0.0) {
    b_x = -1.0;
  } else if (y[1] > 0.0) {
    b_x = 1.0;
  } else if (y[1] == 0.0) {
    b_x = 0.0;
  }
  L_w_odd[0] = x * b_x * fmin(fabs(y[0]), fabs(y[1]));
  /*  result = log((exp(x+y) + 1)/(exp(x) + exp(y))); */
  /*  result = 2*atanh(tanh(x/2) * tanh(y/2)); */
  /*  function result = f(p, q) */
  /*  result = (p * q + 1)/(p + q); */
  /*  end */
  x = y[2];
  if (y[2] < 0.0) {
    x = -1.0;
  } else if (y[2] > 0.0) {
    x = 1.0;
  } else if (y[2] == 0.0) {
    x = 0.0;
  }
  b_x = y[3];
  if (y[3] < 0.0) {
    b_x = -1.0;
  } else if (y[3] > 0.0) {
    b_x = 1.0;
  } else if (y[3] == 0.0) {
    b_x = 0.0;
  }
  L_w_odd[1] = x * b_x * fmin(fabs(y[2]), fabs(y[3]));
  /*  result = log((exp(x+y) + 1)/(exp(x) + exp(y))); */
  /*  result = 2*atanh(tanh(x/2) * tanh(y/2)); */
  /*  function result = f(p, q) */
  /*  result = (p * q + 1)/(p + q); */
  /*  end */
  x = y[4];
  if (y[4] < 0.0) {
    x = -1.0;
  } else if (y[4] > 0.0) {
    x = 1.0;
  } else if (y[4] == 0.0) {
    x = 0.0;
  }
  b_x = y[5];
  if (y[5] < 0.0) {
    b_x = -1.0;
  } else if (y[5] > 0.0) {
    b_x = 1.0;
  } else if (y[5] == 0.0) {
    b_x = 0.0;
  }
  L_w_odd[2] = x * b_x * fmin(fabs(y[4]), fabs(y[5]));
  /*  result = log((exp(x+y) + 1)/(exp(x) + exp(y))); */
  /*  result = 2*atanh(tanh(x/2) * tanh(y/2)); */
  /*  function result = f(p, q) */
  /*  result = (p * q + 1)/(p + q); */
  /*  end */
  x = y[6];
  if (y[6] < 0.0) {
    x = -1.0;
  } else if (y[6] > 0.0) {
    x = 1.0;
  } else if (y[6] == 0.0) {
    x = 0.0;
  }
  b_x = y[7];
  if (y[7] < 0.0) {
    b_x = -1.0;
  } else if (y[7] > 0.0) {
    b_x = 1.0;
  } else if (y[7] == 0.0) {
    b_x = 0.0;
  }
  L_w_odd[3] = x * b_x * fmin(fabs(y[6]), fabs(y[7]));
  /*  result = log((exp(x+y) + 1)/(exp(x) + exp(y))); */
  /*  result = 2*atanh(tanh(x/2) * tanh(y/2)); */
  /*  function result = f(p, q) */
  /*  result = (p * q + 1)/(p + q); */
  /*  end */
  x = y[8];
  if (y[8] < 0.0) {
    x = -1.0;
  } else if (y[8] > 0.0) {
    x = 1.0;
  } else if (y[8] == 0.0) {
    x = 0.0;
  }
  b_x = y[9];
  if (y[9] < 0.0) {
    b_x = -1.0;
  } else if (y[9] > 0.0) {
    b_x = 1.0;
  } else if (y[9] == 0.0) {
    b_x = 0.0;
  }
  L_w_odd[4] = x * b_x * fmin(fabs(y[8]), fabs(y[9]));
  /*  result = log((exp(x+y) + 1)/(exp(x) + exp(y))); */
  /*  result = 2*atanh(tanh(x/2) * tanh(y/2)); */
  /*  function result = f(p, q) */
  /*  result = (p * q + 1)/(p + q); */
  /*  end */
  x = y[10];
  if (y[10] < 0.0) {
    x = -1.0;
  } else if (y[10] > 0.0) {
    x = 1.0;
  } else if (y[10] == 0.0) {
    x = 0.0;
  }
  b_x = y[11];
  if (y[11] < 0.0) {
    b_x = -1.0;
  } else if (y[11] > 0.0) {
    b_x = 1.0;
  } else if (y[11] == 0.0) {
    b_x = 0.0;
  }
  L_w_odd[5] = x * b_x * fmin(fabs(y[10]), fabs(y[11]));
  /*  result = log((exp(x+y) + 1)/(exp(x) + exp(y))); */
  /*  result = 2*atanh(tanh(x/2) * tanh(y/2)); */
  /*  function result = f(p, q) */
  /*  result = (p * q + 1)/(p + q); */
  /*  end */
  x = y[12];
  if (y[12] < 0.0) {
    x = -1.0;
  } else if (y[12] > 0.0) {
    x = 1.0;
  } else if (y[12] == 0.0) {
    x = 0.0;
  }
  b_x = y[13];
  if (y[13] < 0.0) {
    b_x = -1.0;
  } else if (y[13] > 0.0) {
    b_x = 1.0;
  } else if (y[13] == 0.0) {
    b_x = 0.0;
  }
  L_w_odd[6] = x * b_x * fmin(fabs(y[12]), fabs(y[13]));
  /*  result = log((exp(x+y) + 1)/(exp(x) + exp(y))); */
  /*  result = 2*atanh(tanh(x/2) * tanh(y/2)); */
  /*  function result = f(p, q) */
  /*  result = (p * q + 1)/(p + q); */
  /*  end */
  x = y[14];
  if (y[14] < 0.0) {
    x = -1.0;
  } else if (y[14] > 0.0) {
    x = 1.0;
  } else if (y[14] == 0.0) {
    x = 0.0;
  }
  b_x = y[15];
  if (y[15] < 0.0) {
    b_x = -1.0;
  } else if (y[15] > 0.0) {
    b_x = 1.0;
  } else if (y[15] == 0.0) {
    b_x = 0.0;
  }
  L_w_odd[7] = x * b_x * fmin(fabs(y[14]), fabs(y[15]));
  /*  result = log((exp(x+y) + 1)/(exp(x) + exp(y))); */
  /*  result = 2*atanh(tanh(x/2) * tanh(y/2)); */
  /*  function result = f(p, q) */
  /*  result = (p * q + 1)/(p + q); */
  /*  end */
  x = y[16];
  if (y[16] < 0.0) {
    x = -1.0;
  } else if (y[16] > 0.0) {
    x = 1.0;
  } else if (y[16] == 0.0) {
    x = 0.0;
  }
  b_x = y[17];
  if (y[17] < 0.0) {
    b_x = -1.0;
  } else if (y[17] > 0.0) {
    b_x = 1.0;
  } else if (y[17] == 0.0) {
    b_x = 0.0;
  }
  L_w_odd[8] = x * b_x * fmin(fabs(y[16]), fabs(y[17]));
  /*  result = log((exp(x+y) + 1)/(exp(x) + exp(y))); */
  /*  result = 2*atanh(tanh(x/2) * tanh(y/2)); */
  /*  function result = f(p, q) */
  /*  result = (p * q + 1)/(p + q); */
  /*  end */
  x = y[18];
  if (y[18] < 0.0) {
    x = -1.0;
  } else if (y[18] > 0.0) {
    x = 1.0;
  } else if (y[18] == 0.0) {
    x = 0.0;
  }
  b_x = y[19];
  if (y[19] < 0.0) {
    b_x = -1.0;
  } else if (y[19] > 0.0) {
    b_x = 1.0;
  } else if (y[19] == 0.0) {
    b_x = 0.0;
  }
  L_w_odd[9] = x * b_x * fmin(fabs(y[18]), fabs(y[19]));
  /*  result = log((exp(x+y) + 1)/(exp(x) + exp(y))); */
  /*  result = 2*atanh(tanh(x/2) * tanh(y/2)); */
  /*  function result = f(p, q) */
  /*  result = (p * q + 1)/(p + q); */
  /*  end */
  x = y[20];
  if (y[20] < 0.0) {
    x = -1.0;
  } else if (y[20] > 0.0) {
    x = 1.0;
  } else if (y[20] == 0.0) {
    x = 0.0;
  }
  b_x = y[21];
  if (y[21] < 0.0) {
    b_x = -1.0;
  } else if (y[21] > 0.0) {
    b_x = 1.0;
  } else if (y[21] == 0.0) {
    b_x = 0.0;
  }
  L_w_odd[10] = x * b_x * fmin(fabs(y[20]), fabs(y[21]));
  /*  result = log((exp(x+y) + 1)/(exp(x) + exp(y))); */
  /*  result = 2*atanh(tanh(x/2) * tanh(y/2)); */
  /*  function result = f(p, q) */
  /*  result = (p * q + 1)/(p + q); */
  /*  end */
  x = y[22];
  if (y[22] < 0.0) {
    x = -1.0;
  } else if (y[22] > 0.0) {
    x = 1.0;
  } else if (y[22] == 0.0) {
    x = 0.0;
  }
  b_x = y[23];
  if (y[23] < 0.0) {
    b_x = -1.0;
  } else if (y[23] > 0.0) {
    b_x = 1.0;
  } else if (y[23] == 0.0) {
    b_x = 0.0;
  }
  L_w_odd[11] = x * b_x * fmin(fabs(y[22]), fabs(y[23]));
  /*  result = log((exp(x+y) + 1)/(exp(x) + exp(y))); */
  /*  result = 2*atanh(tanh(x/2) * tanh(y/2)); */
  /*  function result = f(p, q) */
  /*  result = (p * q + 1)/(p + q); */
  /*  end */
  x = y[24];
  if (y[24] < 0.0) {
    x = -1.0;
  } else if (y[24] > 0.0) {
    x = 1.0;
  } else if (y[24] == 0.0) {
    x = 0.0;
  }
  b_x = y[25];
  if (y[25] < 0.0) {
    b_x = -1.0;
  } else if (y[25] > 0.0) {
    b_x = 1.0;
  } else if (y[25] == 0.0) {
    b_x = 0.0;
  }
  L_w_odd[12] = x * b_x * fmin(fabs(y[24]), fabs(y[25]));
  /*  result = log((exp(x+y) + 1)/(exp(x) + exp(y))); */
  /*  result = 2*atanh(tanh(x/2) * tanh(y/2)); */
  /*  function result = f(p, q) */
  /*  result = (p * q + 1)/(p + q); */
  /*  end */
  x = y[26];
  if (y[26] < 0.0) {
    x = -1.0;
  } else if (y[26] > 0.0) {
    x = 1.0;
  } else if (y[26] == 0.0) {
    x = 0.0;
  }
  b_x = y[27];
  if (y[27] < 0.0) {
    b_x = -1.0;
  } else if (y[27] > 0.0) {
    b_x = 1.0;
  } else if (y[27] == 0.0) {
    b_x = 0.0;
  }
  L_w_odd[13] = x * b_x * fmin(fabs(y[26]), fabs(y[27]));
  /*  result = log((exp(x+y) + 1)/(exp(x) + exp(y))); */
  /*  result = 2*atanh(tanh(x/2) * tanh(y/2)); */
  /*  function result = f(p, q) */
  /*  result = (p * q + 1)/(p + q); */
  /*  end */
  x = y[28];
  if (y[28] < 0.0) {
    x = -1.0;
  } else if (y[28] > 0.0) {
    x = 1.0;
  } else if (y[28] == 0.0) {
    x = 0.0;
  }
  b_x = y[29];
  if (y[29] < 0.0) {
    b_x = -1.0;
  } else if (y[29] > 0.0) {
    b_x = 1.0;
  } else if (y[29] == 0.0) {
    b_x = 0.0;
  }
  L_w_odd[14] = x * b_x * fmin(fabs(y[28]), fabs(y[29]));
  /*  result = log((exp(x+y) + 1)/(exp(x) + exp(y))); */
  /*  result = 2*atanh(tanh(x/2) * tanh(y/2)); */
  /*  function result = f(p, q) */
  /*  result = (p * q + 1)/(p + q); */
  /*  end */
  x = y[30];
  if (y[30] < 0.0) {
    x = -1.0;
  } else if (y[30] > 0.0) {
    x = 1.0;
  } else if (y[30] == 0.0) {
    x = 0.0;
  }
  b_x = y[31];
  if (y[31] < 0.0) {
    b_x = -1.0;
  } else if (y[31] > 0.0) {
    b_x = 1.0;
  } else if (y[31] == 0.0) {
    b_x = 0.0;
  }
  L_w_odd[15] = x * b_x * fmin(fabs(y[30]), fabs(y[31]));
  i_polar_decode(L_w_odd, *(double(*)[16]) & frozen_bits_indicator[0], u_1_data,
                 u_1_size, v_1);
  /*  function result = g(p, q, u) */
  /*  result = (p^(1-2*u)) * q; */
  /*  end */
  if (v_1[0] == 0.0) {
    result = y[0] + y[1];
  } else if (v_1[0] == 1.0) {
    result = y[1] - y[0];
  }
  L_w_odd[0] = result;
  /*  function result = g(p, q, u) */
  /*  result = (p^(1-2*u)) * q; */
  /*  end */
  if (v_1[1] == 0.0) {
    result = y[2] + y[3];
  } else if (v_1[1] == 1.0) {
    result = y[3] - y[2];
  }
  L_w_odd[1] = result;
  /*  function result = g(p, q, u) */
  /*  result = (p^(1-2*u)) * q; */
  /*  end */
  if (v_1[2] == 0.0) {
    result = y[4] + y[5];
  } else if (v_1[2] == 1.0) {
    result = y[5] - y[4];
  }
  L_w_odd[2] = result;
  /*  function result = g(p, q, u) */
  /*  result = (p^(1-2*u)) * q; */
  /*  end */
  if (v_1[3] == 0.0) {
    result = y[6] + y[7];
  } else if (v_1[3] == 1.0) {
    result = y[7] - y[6];
  }
  L_w_odd[3] = result;
  /*  function result = g(p, q, u) */
  /*  result = (p^(1-2*u)) * q; */
  /*  end */
  if (v_1[4] == 0.0) {
    result = y[8] + y[9];
  } else if (v_1[4] == 1.0) {
    result = y[9] - y[8];
  }
  L_w_odd[4] = result;
  /*  function result = g(p, q, u) */
  /*  result = (p^(1-2*u)) * q; */
  /*  end */
  if (v_1[5] == 0.0) {
    result = y[10] + y[11];
  } else if (v_1[5] == 1.0) {
    result = y[11] - y[10];
  }
  L_w_odd[5] = result;
  /*  function result = g(p, q, u) */
  /*  result = (p^(1-2*u)) * q; */
  /*  end */
  if (v_1[6] == 0.0) {
    result = y[12] + y[13];
  } else if (v_1[6] == 1.0) {
    result = y[13] - y[12];
  }
  L_w_odd[6] = result;
  /*  function result = g(p, q, u) */
  /*  result = (p^(1-2*u)) * q; */
  /*  end */
  if (v_1[7] == 0.0) {
    result = y[14] + y[15];
  } else if (v_1[7] == 1.0) {
    result = y[15] - y[14];
  }
  L_w_odd[7] = result;
  /*  function result = g(p, q, u) */
  /*  result = (p^(1-2*u)) * q; */
  /*  end */
  if (v_1[8] == 0.0) {
    result = y[16] + y[17];
  } else if (v_1[8] == 1.0) {
    result = y[17] - y[16];
  }
  L_w_odd[8] = result;
  /*  function result = g(p, q, u) */
  /*  result = (p^(1-2*u)) * q; */
  /*  end */
  if (v_1[9] == 0.0) {
    result = y[18] + y[19];
  } else if (v_1[9] == 1.0) {
    result = y[19] - y[18];
  }
  L_w_odd[9] = result;
  /*  function result = g(p, q, u) */
  /*  result = (p^(1-2*u)) * q; */
  /*  end */
  if (v_1[10] == 0.0) {
    result = y[20] + y[21];
  } else if (v_1[10] == 1.0) {
    result = y[21] - y[20];
  }
  L_w_odd[10] = result;
  /*  function result = g(p, q, u) */
  /*  result = (p^(1-2*u)) * q; */
  /*  end */
  if (v_1[11] == 0.0) {
    result = y[22] + y[23];
  } else if (v_1[11] == 1.0) {
    result = y[23] - y[22];
  }
  L_w_odd[11] = result;
  /*  function result = g(p, q, u) */
  /*  result = (p^(1-2*u)) * q; */
  /*  end */
  if (v_1[12] == 0.0) {
    result = y[24] + y[25];
  } else if (v_1[12] == 1.0) {
    result = y[25] - y[24];
  }
  L_w_odd[12] = result;
  /*  function result = g(p, q, u) */
  /*  result = (p^(1-2*u)) * q; */
  /*  end */
  if (v_1[13] == 0.0) {
    result = y[26] + y[27];
  } else if (v_1[13] == 1.0) {
    result = y[27] - y[26];
  }
  L_w_odd[13] = result;
  /*  function result = g(p, q, u) */
  /*  result = (p^(1-2*u)) * q; */
  /*  end */
  if (v_1[14] == 0.0) {
    result = y[28] + y[29];
  } else if (v_1[14] == 1.0) {
    result = y[29] - y[28];
  }
  L_w_odd[14] = result;
  /*  function result = g(p, q, u) */
  /*  result = (p^(1-2*u)) * q; */
  /*  end */
  if (v_1[15] == 0.0) {
    result = y[30] + y[31];
  } else if (v_1[15] == 1.0) {
    result = y[31] - y[30];
  }
  L_w_odd[15] = result;
  i_polar_decode(L_w_odd, *(double(*)[16]) & frozen_bits_indicator[16],
                 u_2_data, u_1_size, v_2);
  u_size[0] = 1;
  u_size[1] = 32;
  u_data[0] = u_1_data[0];
  u_data[16] = u_2_data[0];
  v[0] = (double)((unsigned long)v_1[0] ^ (unsigned long)v_2[0]);
  v[1] = v_2[0];
  u_data[1] = u_1_data[1];
  u_data[17] = u_2_data[1];
  v[2] = (double)((unsigned long)v_1[1] ^ (unsigned long)v_2[1]);
  v[3] = v_2[1];
  u_data[2] = u_1_data[2];
  u_data[18] = u_2_data[2];
  v[4] = (double)((unsigned long)v_1[2] ^ (unsigned long)v_2[2]);
  v[5] = v_2[2];
  u_data[3] = u_1_data[3];
  u_data[19] = u_2_data[3];
  v[6] = (double)((unsigned long)v_1[3] ^ (unsigned long)v_2[3]);
  v[7] = v_2[3];
  u_data[4] = u_1_data[4];
  u_data[20] = u_2_data[4];
  v[8] = (double)((unsigned long)v_1[4] ^ (unsigned long)v_2[4]);
  v[9] = v_2[4];
  u_data[5] = u_1_data[5];
  u_data[21] = u_2_data[5];
  v[10] = (double)((unsigned long)v_1[5] ^ (unsigned long)v_2[5]);
  v[11] = v_2[5];
  u_data[6] = u_1_data[6];
  u_data[22] = u_2_data[6];
  v[12] = (double)((unsigned long)v_1[6] ^ (unsigned long)v_2[6]);
  v[13] = v_2[6];
  u_data[7] = u_1_data[7];
  u_data[23] = u_2_data[7];
  v[14] = (double)((unsigned long)v_1[7] ^ (unsigned long)v_2[7]);
  v[15] = v_2[7];
  u_data[8] = u_1_data[8];
  u_data[24] = u_2_data[8];
  v[16] = (double)((unsigned long)v_1[8] ^ (unsigned long)v_2[8]);
  v[17] = v_2[8];
  u_data[9] = u_1_data[9];
  u_data[25] = u_2_data[9];
  v[18] = (double)((unsigned long)v_1[9] ^ (unsigned long)v_2[9]);
  v[19] = v_2[9];
  u_data[10] = u_1_data[10];
  u_data[26] = u_2_data[10];
  v[20] = (double)((unsigned long)v_1[10] ^ (unsigned long)v_2[10]);
  v[21] = v_2[10];
  u_data[11] = u_1_data[11];
  u_data[27] = u_2_data[11];
  v[22] = (double)((unsigned long)v_1[11] ^ (unsigned long)v_2[11]);
  v[23] = v_2[11];
  u_data[12] = u_1_data[12];
  u_data[28] = u_2_data[12];
  v[24] = (double)((unsigned long)v_1[12] ^ (unsigned long)v_2[12]);
  v[25] = v_2[12];
  u_data[13] = u_1_data[13];
  u_data[29] = u_2_data[13];
  v[26] = (double)((unsigned long)v_1[13] ^ (unsigned long)v_2[13]);
  v[27] = v_2[13];
  u_data[14] = u_1_data[14];
  u_data[30] = u_2_data[14];
  v[28] = (double)((unsigned long)v_1[14] ^ (unsigned long)v_2[14]);
  v[29] = v_2[14];
  u_data[15] = u_1_data[15];
  u_data[31] = u_2_data[15];
  v[30] = (double)((unsigned long)v_1[15] ^ (unsigned long)v_2[15]);
  v[31] = v_2[15];
}

/*
 * Arguments    : const double y[16]
 *                const double frozen_bits_indicator[16]
 *                double u_data[]
 *                int u_size[2]
 *                double v[16]
 * Return Type  : void
 */
static void i_polar_decode(const double y[16],
                           const double frozen_bits_indicator[16],
                           double u_data[], int u_size[2], double v[16])
{
  double L_w_odd[8];
  double u_1_data[8];
  double u_2_data[8];
  double v_1[8];
  double v_2[8];
  double b_x;
  double result;
  double x;
  int u_1_size[2];
  /*  result = log((exp(x+y) + 1)/(exp(x) + exp(y))); */
  /*  result = 2*atanh(tanh(x/2) * tanh(y/2)); */
  /*  function result = f(p, q) */
  /*  result = (p * q + 1)/(p + q); */
  /*  end */
  x = y[0];
  if (y[0] < 0.0) {
    x = -1.0;
  } else if (y[0] > 0.0) {
    x = 1.0;
  } else if (y[0] == 0.0) {
    x = 0.0;
  }
  b_x = y[1];
  if (y[1] < 0.0) {
    b_x = -1.0;
  } else if (y[1] > 0.0) {
    b_x = 1.0;
  } else if (y[1] == 0.0) {
    b_x = 0.0;
  }
  L_w_odd[0] = x * b_x * fmin(fabs(y[0]), fabs(y[1]));
  /*  result = log((exp(x+y) + 1)/(exp(x) + exp(y))); */
  /*  result = 2*atanh(tanh(x/2) * tanh(y/2)); */
  /*  function result = f(p, q) */
  /*  result = (p * q + 1)/(p + q); */
  /*  end */
  x = y[2];
  if (y[2] < 0.0) {
    x = -1.0;
  } else if (y[2] > 0.0) {
    x = 1.0;
  } else if (y[2] == 0.0) {
    x = 0.0;
  }
  b_x = y[3];
  if (y[3] < 0.0) {
    b_x = -1.0;
  } else if (y[3] > 0.0) {
    b_x = 1.0;
  } else if (y[3] == 0.0) {
    b_x = 0.0;
  }
  L_w_odd[1] = x * b_x * fmin(fabs(y[2]), fabs(y[3]));
  /*  result = log((exp(x+y) + 1)/(exp(x) + exp(y))); */
  /*  result = 2*atanh(tanh(x/2) * tanh(y/2)); */
  /*  function result = f(p, q) */
  /*  result = (p * q + 1)/(p + q); */
  /*  end */
  x = y[4];
  if (y[4] < 0.0) {
    x = -1.0;
  } else if (y[4] > 0.0) {
    x = 1.0;
  } else if (y[4] == 0.0) {
    x = 0.0;
  }
  b_x = y[5];
  if (y[5] < 0.0) {
    b_x = -1.0;
  } else if (y[5] > 0.0) {
    b_x = 1.0;
  } else if (y[5] == 0.0) {
    b_x = 0.0;
  }
  L_w_odd[2] = x * b_x * fmin(fabs(y[4]), fabs(y[5]));
  /*  result = log((exp(x+y) + 1)/(exp(x) + exp(y))); */
  /*  result = 2*atanh(tanh(x/2) * tanh(y/2)); */
  /*  function result = f(p, q) */
  /*  result = (p * q + 1)/(p + q); */
  /*  end */
  x = y[6];
  if (y[6] < 0.0) {
    x = -1.0;
  } else if (y[6] > 0.0) {
    x = 1.0;
  } else if (y[6] == 0.0) {
    x = 0.0;
  }
  b_x = y[7];
  if (y[7] < 0.0) {
    b_x = -1.0;
  } else if (y[7] > 0.0) {
    b_x = 1.0;
  } else if (y[7] == 0.0) {
    b_x = 0.0;
  }
  L_w_odd[3] = x * b_x * fmin(fabs(y[6]), fabs(y[7]));
  /*  result = log((exp(x+y) + 1)/(exp(x) + exp(y))); */
  /*  result = 2*atanh(tanh(x/2) * tanh(y/2)); */
  /*  function result = f(p, q) */
  /*  result = (p * q + 1)/(p + q); */
  /*  end */
  x = y[8];
  if (y[8] < 0.0) {
    x = -1.0;
  } else if (y[8] > 0.0) {
    x = 1.0;
  } else if (y[8] == 0.0) {
    x = 0.0;
  }
  b_x = y[9];
  if (y[9] < 0.0) {
    b_x = -1.0;
  } else if (y[9] > 0.0) {
    b_x = 1.0;
  } else if (y[9] == 0.0) {
    b_x = 0.0;
  }
  L_w_odd[4] = x * b_x * fmin(fabs(y[8]), fabs(y[9]));
  /*  result = log((exp(x+y) + 1)/(exp(x) + exp(y))); */
  /*  result = 2*atanh(tanh(x/2) * tanh(y/2)); */
  /*  function result = f(p, q) */
  /*  result = (p * q + 1)/(p + q); */
  /*  end */
  x = y[10];
  if (y[10] < 0.0) {
    x = -1.0;
  } else if (y[10] > 0.0) {
    x = 1.0;
  } else if (y[10] == 0.0) {
    x = 0.0;
  }
  b_x = y[11];
  if (y[11] < 0.0) {
    b_x = -1.0;
  } else if (y[11] > 0.0) {
    b_x = 1.0;
  } else if (y[11] == 0.0) {
    b_x = 0.0;
  }
  L_w_odd[5] = x * b_x * fmin(fabs(y[10]), fabs(y[11]));
  /*  result = log((exp(x+y) + 1)/(exp(x) + exp(y))); */
  /*  result = 2*atanh(tanh(x/2) * tanh(y/2)); */
  /*  function result = f(p, q) */
  /*  result = (p * q + 1)/(p + q); */
  /*  end */
  x = y[12];
  if (y[12] < 0.0) {
    x = -1.0;
  } else if (y[12] > 0.0) {
    x = 1.0;
  } else if (y[12] == 0.0) {
    x = 0.0;
  }
  b_x = y[13];
  if (y[13] < 0.0) {
    b_x = -1.0;
  } else if (y[13] > 0.0) {
    b_x = 1.0;
  } else if (y[13] == 0.0) {
    b_x = 0.0;
  }
  L_w_odd[6] = x * b_x * fmin(fabs(y[12]), fabs(y[13]));
  /*  result = log((exp(x+y) + 1)/(exp(x) + exp(y))); */
  /*  result = 2*atanh(tanh(x/2) * tanh(y/2)); */
  /*  function result = f(p, q) */
  /*  result = (p * q + 1)/(p + q); */
  /*  end */
  x = y[14];
  if (y[14] < 0.0) {
    x = -1.0;
  } else if (y[14] > 0.0) {
    x = 1.0;
  } else if (y[14] == 0.0) {
    x = 0.0;
  }
  b_x = y[15];
  if (y[15] < 0.0) {
    b_x = -1.0;
  } else if (y[15] > 0.0) {
    b_x = 1.0;
  } else if (y[15] == 0.0) {
    b_x = 0.0;
  }
  L_w_odd[7] = x * b_x * fmin(fabs(y[14]), fabs(y[15]));
  j_polar_decode(L_w_odd, *(double(*)[8]) & frozen_bits_indicator[0], u_1_data,
                 u_1_size, v_1);
  /*  function result = g(p, q, u) */
  /*  result = (p^(1-2*u)) * q; */
  /*  end */
  if (v_1[0] == 0.0) {
    result = y[0] + y[1];
  } else if (v_1[0] == 1.0) {
    result = y[1] - y[0];
  }
  L_w_odd[0] = result;
  /*  function result = g(p, q, u) */
  /*  result = (p^(1-2*u)) * q; */
  /*  end */
  if (v_1[1] == 0.0) {
    result = y[2] + y[3];
  } else if (v_1[1] == 1.0) {
    result = y[3] - y[2];
  }
  L_w_odd[1] = result;
  /*  function result = g(p, q, u) */
  /*  result = (p^(1-2*u)) * q; */
  /*  end */
  if (v_1[2] == 0.0) {
    result = y[4] + y[5];
  } else if (v_1[2] == 1.0) {
    result = y[5] - y[4];
  }
  L_w_odd[2] = result;
  /*  function result = g(p, q, u) */
  /*  result = (p^(1-2*u)) * q; */
  /*  end */
  if (v_1[3] == 0.0) {
    result = y[6] + y[7];
  } else if (v_1[3] == 1.0) {
    result = y[7] - y[6];
  }
  L_w_odd[3] = result;
  /*  function result = g(p, q, u) */
  /*  result = (p^(1-2*u)) * q; */
  /*  end */
  if (v_1[4] == 0.0) {
    result = y[8] + y[9];
  } else if (v_1[4] == 1.0) {
    result = y[9] - y[8];
  }
  L_w_odd[4] = result;
  /*  function result = g(p, q, u) */
  /*  result = (p^(1-2*u)) * q; */
  /*  end */
  if (v_1[5] == 0.0) {
    result = y[10] + y[11];
  } else if (v_1[5] == 1.0) {
    result = y[11] - y[10];
  }
  L_w_odd[5] = result;
  /*  function result = g(p, q, u) */
  /*  result = (p^(1-2*u)) * q; */
  /*  end */
  if (v_1[6] == 0.0) {
    result = y[12] + y[13];
  } else if (v_1[6] == 1.0) {
    result = y[13] - y[12];
  }
  L_w_odd[6] = result;
  /*  function result = g(p, q, u) */
  /*  result = (p^(1-2*u)) * q; */
  /*  end */
  if (v_1[7] == 0.0) {
    result = y[14] + y[15];
  } else if (v_1[7] == 1.0) {
    result = y[15] - y[14];
  }
  L_w_odd[7] = result;
  j_polar_decode(L_w_odd, *(double(*)[8]) & frozen_bits_indicator[8], u_2_data,
                 u_1_size, v_2);
  u_size[0] = 1;
  u_size[1] = 16;
  u_data[0] = u_1_data[0];
  u_data[8] = u_2_data[0];
  v[0] = (double)((unsigned long)v_1[0] ^ (unsigned long)v_2[0]);
  v[1] = v_2[0];
  u_data[1] = u_1_data[1];
  u_data[9] = u_2_data[1];
  v[2] = (double)((unsigned long)v_1[1] ^ (unsigned long)v_2[1]);
  v[3] = v_2[1];
  u_data[2] = u_1_data[2];
  u_data[10] = u_2_data[2];
  v[4] = (double)((unsigned long)v_1[2] ^ (unsigned long)v_2[2]);
  v[5] = v_2[2];
  u_data[3] = u_1_data[3];
  u_data[11] = u_2_data[3];
  v[6] = (double)((unsigned long)v_1[3] ^ (unsigned long)v_2[3]);
  v[7] = v_2[3];
  u_data[4] = u_1_data[4];
  u_data[12] = u_2_data[4];
  v[8] = (double)((unsigned long)v_1[4] ^ (unsigned long)v_2[4]);
  v[9] = v_2[4];
  u_data[5] = u_1_data[5];
  u_data[13] = u_2_data[5];
  v[10] = (double)((unsigned long)v_1[5] ^ (unsigned long)v_2[5]);
  v[11] = v_2[5];
  u_data[6] = u_1_data[6];
  u_data[14] = u_2_data[6];
  v[12] = (double)((unsigned long)v_1[6] ^ (unsigned long)v_2[6]);
  v[13] = v_2[6];
  u_data[7] = u_1_data[7];
  u_data[15] = u_2_data[7];
  v[14] = (double)((unsigned long)v_1[7] ^ (unsigned long)v_2[7]);
  v[15] = v_2[7];
}

/*
 * Arguments    : const double y[8]
 *                const double frozen_bits_indicator[8]
 *                double u_data[]
 *                int u_size[2]
 *                double v[8]
 * Return Type  : void
 */
static void j_polar_decode(const double y[8],
                           const double frozen_bits_indicator[8],
                           double u_data[], int u_size[2], double v[8])
{
  double L_w_odd[4];
  double u_1_data[4];
  double u_2_data[4];
  double v_1[4];
  double v_2[4];
  double b_x;
  double result;
  double x;
  int u_1_size[2];
  /*  result = log((exp(x+y) + 1)/(exp(x) + exp(y))); */
  /*  result = 2*atanh(tanh(x/2) * tanh(y/2)); */
  /*  function result = f(p, q) */
  /*  result = (p * q + 1)/(p + q); */
  /*  end */
  x = y[0];
  if (y[0] < 0.0) {
    x = -1.0;
  } else if (y[0] > 0.0) {
    x = 1.0;
  } else if (y[0] == 0.0) {
    x = 0.0;
  }
  b_x = y[1];
  if (y[1] < 0.0) {
    b_x = -1.0;
  } else if (y[1] > 0.0) {
    b_x = 1.0;
  } else if (y[1] == 0.0) {
    b_x = 0.0;
  }
  L_w_odd[0] = x * b_x * fmin(fabs(y[0]), fabs(y[1]));
  /*  result = log((exp(x+y) + 1)/(exp(x) + exp(y))); */
  /*  result = 2*atanh(tanh(x/2) * tanh(y/2)); */
  /*  function result = f(p, q) */
  /*  result = (p * q + 1)/(p + q); */
  /*  end */
  x = y[2];
  if (y[2] < 0.0) {
    x = -1.0;
  } else if (y[2] > 0.0) {
    x = 1.0;
  } else if (y[2] == 0.0) {
    x = 0.0;
  }
  b_x = y[3];
  if (y[3] < 0.0) {
    b_x = -1.0;
  } else if (y[3] > 0.0) {
    b_x = 1.0;
  } else if (y[3] == 0.0) {
    b_x = 0.0;
  }
  L_w_odd[1] = x * b_x * fmin(fabs(y[2]), fabs(y[3]));
  /*  result = log((exp(x+y) + 1)/(exp(x) + exp(y))); */
  /*  result = 2*atanh(tanh(x/2) * tanh(y/2)); */
  /*  function result = f(p, q) */
  /*  result = (p * q + 1)/(p + q); */
  /*  end */
  x = y[4];
  if (y[4] < 0.0) {
    x = -1.0;
  } else if (y[4] > 0.0) {
    x = 1.0;
  } else if (y[4] == 0.0) {
    x = 0.0;
  }
  b_x = y[5];
  if (y[5] < 0.0) {
    b_x = -1.0;
  } else if (y[5] > 0.0) {
    b_x = 1.0;
  } else if (y[5] == 0.0) {
    b_x = 0.0;
  }
  L_w_odd[2] = x * b_x * fmin(fabs(y[4]), fabs(y[5]));
  /*  result = log((exp(x+y) + 1)/(exp(x) + exp(y))); */
  /*  result = 2*atanh(tanh(x/2) * tanh(y/2)); */
  /*  function result = f(p, q) */
  /*  result = (p * q + 1)/(p + q); */
  /*  end */
  x = y[6];
  if (y[6] < 0.0) {
    x = -1.0;
  } else if (y[6] > 0.0) {
    x = 1.0;
  } else if (y[6] == 0.0) {
    x = 0.0;
  }
  b_x = y[7];
  if (y[7] < 0.0) {
    b_x = -1.0;
  } else if (y[7] > 0.0) {
    b_x = 1.0;
  } else if (y[7] == 0.0) {
    b_x = 0.0;
  }
  L_w_odd[3] = x * b_x * fmin(fabs(y[6]), fabs(y[7]));
  k_polar_decode(L_w_odd, *(double(*)[4]) & frozen_bits_indicator[0], u_1_data,
                 u_1_size, v_1);
  /*  function result = g(p, q, u) */
  /*  result = (p^(1-2*u)) * q; */
  /*  end */
  if (v_1[0] == 0.0) {
    result = y[0] + y[1];
  } else if (v_1[0] == 1.0) {
    result = y[1] - y[0];
  }
  L_w_odd[0] = result;
  /*  function result = g(p, q, u) */
  /*  result = (p^(1-2*u)) * q; */
  /*  end */
  if (v_1[1] == 0.0) {
    result = y[2] + y[3];
  } else if (v_1[1] == 1.0) {
    result = y[3] - y[2];
  }
  L_w_odd[1] = result;
  /*  function result = g(p, q, u) */
  /*  result = (p^(1-2*u)) * q; */
  /*  end */
  if (v_1[2] == 0.0) {
    result = y[4] + y[5];
  } else if (v_1[2] == 1.0) {
    result = y[5] - y[4];
  }
  L_w_odd[2] = result;
  /*  function result = g(p, q, u) */
  /*  result = (p^(1-2*u)) * q; */
  /*  end */
  if (v_1[3] == 0.0) {
    result = y[6] + y[7];
  } else if (v_1[3] == 1.0) {
    result = y[7] - y[6];
  }
  L_w_odd[3] = result;
  k_polar_decode(L_w_odd, *(double(*)[4]) & frozen_bits_indicator[4], u_2_data,
                 u_1_size, v_2);
  u_size[0] = 1;
  u_size[1] = 8;
  u_data[0] = u_1_data[0];
  u_data[4] = u_2_data[0];
  v[0] = (double)((unsigned long)v_1[0] ^ (unsigned long)v_2[0]);
  v[1] = v_2[0];
  u_data[1] = u_1_data[1];
  u_data[5] = u_2_data[1];
  v[2] = (double)((unsigned long)v_1[1] ^ (unsigned long)v_2[1]);
  v[3] = v_2[1];
  u_data[2] = u_1_data[2];
  u_data[6] = u_2_data[2];
  v[4] = (double)((unsigned long)v_1[2] ^ (unsigned long)v_2[2]);
  v[5] = v_2[2];
  u_data[3] = u_1_data[3];
  u_data[7] = u_2_data[3];
  v[6] = (double)((unsigned long)v_1[3] ^ (unsigned long)v_2[3]);
  v[7] = v_2[3];
}

/*
 * Arguments    : const double y[4]
 *                const double frozen_bits_indicator[4]
 *                double u_data[]
 *                int u_size[2]
 *                double v[4]
 * Return Type  : void
 */
static void k_polar_decode(const double y[4],
                           const double frozen_bits_indicator[4],
                           double u_data[], int u_size[2], double v[4])
{
  double L_w_odd[2];
  double u_1_data[2];
  double u_2_data[2];
  double v_1_data[2];
  double v_2_data[2];
  double b_x;
  double result;
  double x;
  int u_1_size[2];
  int v_1_size[2];
  /*  result = log((exp(x+y) + 1)/(exp(x) + exp(y))); */
  /*  result = 2*atanh(tanh(x/2) * tanh(y/2)); */
  /*  function result = f(p, q) */
  /*  result = (p * q + 1)/(p + q); */
  /*  end */
  x = y[0];
  if (y[0] < 0.0) {
    x = -1.0;
  } else if (y[0] > 0.0) {
    x = 1.0;
  } else if (y[0] == 0.0) {
    x = 0.0;
  }
  b_x = y[1];
  if (y[1] < 0.0) {
    b_x = -1.0;
  } else if (y[1] > 0.0) {
    b_x = 1.0;
  } else if (y[1] == 0.0) {
    b_x = 0.0;
  }
  L_w_odd[0] = x * b_x * fmin(fabs(y[0]), fabs(y[1]));
  /*  result = log((exp(x+y) + 1)/(exp(x) + exp(y))); */
  /*  result = 2*atanh(tanh(x/2) * tanh(y/2)); */
  /*  function result = f(p, q) */
  /*  result = (p * q + 1)/(p + q); */
  /*  end */
  x = y[2];
  if (y[2] < 0.0) {
    x = -1.0;
  } else if (y[2] > 0.0) {
    x = 1.0;
  } else if (y[2] == 0.0) {
    x = 0.0;
  }
  b_x = y[3];
  if (y[3] < 0.0) {
    b_x = -1.0;
  } else if (y[3] > 0.0) {
    b_x = 1.0;
  } else if (y[3] == 0.0) {
    b_x = 0.0;
  }
  L_w_odd[1] = x * b_x * fmin(fabs(y[2]), fabs(y[3]));
  l_polar_decode(L_w_odd, *(double(*)[2]) & frozen_bits_indicator[0], u_1_data,
                 u_1_size, v_1_data, v_1_size);
  /*  function result = g(p, q, u) */
  /*  result = (p^(1-2*u)) * q; */
  /*  end */
  if (v_1_data[0] == 0.0) {
    result = y[0] + y[1];
  } else if (v_1_data[0] == 1.0) {
    result = y[1] - y[0];
  }
  L_w_odd[0] = result;
  /*  function result = g(p, q, u) */
  /*  result = (p^(1-2*u)) * q; */
  /*  end */
  if (v_1_data[1] == 0.0) {
    result = y[2] + y[3];
  } else if (v_1_data[1] == 1.0) {
    result = y[3] - y[2];
  }
  L_w_odd[1] = result;
  l_polar_decode(L_w_odd, *(double(*)[2]) & frozen_bits_indicator[2], u_2_data,
                 u_1_size, v_2_data, v_1_size);
  u_size[0] = 1;
  u_size[1] = 4;
  u_data[0] = u_1_data[0];
  u_data[2] = u_2_data[0];
  v[0] = (double)((unsigned long)v_1_data[0] ^ (unsigned long)v_2_data[0]);
  v[1] = v_2_data[0];
  u_data[1] = u_1_data[1];
  u_data[3] = u_2_data[1];
  v[2] = (double)((unsigned long)v_1_data[1] ^ (unsigned long)v_2_data[1]);
  v[3] = v_2_data[1];
}

/*
 * Arguments    : const double y[2]
 *                const double frozen_bits_indicator[2]
 *                double u_data[]
 *                int u_size[2]
 *                double v_data[]
 *                int v_size[2]
 * Return Type  : void
 */
static void l_polar_decode(const double y[2],
                           const double frozen_bits_indicator[2],
                           double u_data[], int u_size[2], double v_data[],
                           int v_size[2])
{
  double L_u_2;
  double b_x;
  double x;
  /*  result = log((exp(x+y) + 1)/(exp(x) + exp(y))); */
  /*  result = 2*atanh(tanh(x/2) * tanh(y/2)); */
  /*  function result = f(p, q) */
  /*  result = (p * q + 1)/(p + q); */
  /*  end */
  x = y[0];
  if (y[0] < 0.0) {
    x = -1.0;
  } else if (y[0] > 0.0) {
    x = 1.0;
  } else if (y[0] == 0.0) {
    x = 0.0;
  }
  b_x = y[1];
  if (y[1] < 0.0) {
    b_x = -1.0;
  } else if (y[1] > 0.0) {
    b_x = 1.0;
  } else if (y[1] == 0.0) {
    b_x = 0.0;
  }
  u_size[0] = 1;
  u_size[1] = 2;
  if (frozen_bits_indicator[0] == 1.0) {
    u_data[0] = 0.0;
  } else {
    u_data[0] = !(x * b_x * fmin(fabs(y[0]), fabs(y[1])) >= 0.0);
  }
  /*  function result = g(p, q, u) */
  /*  result = (p^(1-2*u)) * q; */
  /*  end */
  if (u_data[0] == 0.0) {
    L_u_2 = y[0] + y[1];
  } else if (u_data[0] == 1.0) {
    L_u_2 = y[1] - y[0];
  }
  if (frozen_bits_indicator[1] == 1.0) {
    u_data[1] = 0.0;
  } else {
    u_data[1] = !(L_u_2 >= 0.0);
  }
  v_size[0] = 1;
  v_size[1] = 2;
  v_data[0] =
      (double)((unsigned long)(signed char)u_data[0] ^ (signed char)u_data[1]);
  v_data[1] = u_data[1];
}

/*
 * Arguments    : const double y[128]
 *                const double frozen_bits_indicator[128]
 *                double u_data[]
 *                int u_size[2]
 *                double v[128]
 * Return Type  : void
 */
void f_polar_decode(const double y[128],
                    const double frozen_bits_indicator[128], double u_data[],
                    int u_size[2], double v[128])
{
  double L_w_odd[64];
  double u_1_data[64];
  double u_2_data[64];
  double v_1[64];
  double v_2[64];
  double b_x;
  double b_y;
  double c_x;
  double result;
  double x;
  int u_1_size[2];
  int b_index;
  int x_tmp;
  for (b_index = 0; b_index < 64; b_index++) {
    x_tmp = (b_index + 1) << 1;
    x = y[x_tmp - 2];
    b_y = y[x_tmp - 1];
    /*  result = log((exp(x+y) + 1)/(exp(x) + exp(y))); */
    /*  result = 2*atanh(tanh(x/2) * tanh(y/2)); */
    /*  function result = f(p, q) */
    /*  result = (p * q + 1)/(p + q); */
    /*  end */
    b_x = x;
    if (x < 0.0) {
      b_x = -1.0;
    } else if (x > 0.0) {
      b_x = 1.0;
    } else if (x == 0.0) {
      b_x = 0.0;
    }
    c_x = b_y;
    if (b_y < 0.0) {
      c_x = -1.0;
    } else if (b_y > 0.0) {
      c_x = 1.0;
    } else if (b_y == 0.0) {
      c_x = 0.0;
    }
    L_w_odd[b_index] = b_x * c_x * fmin(fabs(x), fabs(b_y));
  }
  g_polar_decode(L_w_odd, *(double(*)[64]) & frozen_bits_indicator[0], u_1_data,
                 u_1_size, v_1);
  for (b_index = 0; b_index < 64; b_index++) {
    x_tmp = (b_index + 1) << 1;
    x = y[x_tmp - 2];
    b_y = y[x_tmp - 1];
    /*  function result = g(p, q, u) */
    /*  result = (p^(1-2*u)) * q; */
    /*  end */
    b_x = v_1[b_index];
    if (b_x == 0.0) {
      result = x + b_y;
    } else if (b_x == 1.0) {
      result = b_y - x;
    }
    L_w_odd[b_index] = result;
  }
  g_polar_decode(L_w_odd, *(double(*)[64]) & frozen_bits_indicator[64],
                 u_2_data, u_1_size, v_2);
  u_size[0] = 1;
  u_size[1] = 128;
  for (b_index = 0; b_index < 64; b_index++) {
    u_data[b_index] = u_1_data[b_index];
    u_data[b_index + 64] = u_2_data[b_index];
    x_tmp = (b_index + 1) << 1;
    b_x = v_2[b_index];
    v[x_tmp - 2] = (double)((unsigned long)v_1[b_index] ^ (unsigned long)b_x);
    v[x_tmp - 1] = b_x;
  }
}

/*
 * Arguments    : double u_data[]
 *                int u_size[2]
 *                double v[64]
 * Return Type  : void
 */
void polar_decode(double u_data[], int u_size[2], double v[64])
{
  double u_1_data[32];
  double u_2_data[32];
  double v_1[32];
  double v_2[32];
  double d;
  int u_1_size[2];
  int b_index;
  int v_tmp;
  b_polar_decode(u_1_data, u_1_size, v_1);
  b_polar_decode(u_2_data, u_1_size, v_2);
  u_size[0] = 1;
  u_size[1] = 64;
  for (b_index = 0; b_index < 32; b_index++) {
    u_data[b_index] = u_1_data[b_index];
    u_data[b_index + 32] = u_2_data[b_index];
    v_tmp = (b_index + 1) << 1;
    d = v_2[b_index];
    v[v_tmp - 2] = (double)((unsigned long)v_1[b_index] ^ (unsigned long)d);
    v[v_tmp - 1] = d;
  }
}

/*
 * File trailer for polar_decode.c
 *
 * [EOF]
 */

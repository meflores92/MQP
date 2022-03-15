/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: nrDCIEncode.c
 *
 * MATLAB Coder version            : 5.2
 * C/C++ source code generated on  : 20-Feb-2022 08:55:56
 */

/* Include Files */
#include "nrDCIEncode.h"
#include "log2.h"
#include "mod.h"
#include "operation_data.h"
#include "operation_emxutil.h"
#include "operation_rtwutil.h"
#include "operation_types.h"
#include "rt_nonfinite.h"
#include "unique.h"
#include "rt_nonfinite.h"
#include <math.h>
#include <string.h>

/* Function Definitions */
/*
 * Arguments    : const signed char dciBits[39]
 *                unsigned int rnti
 *                emxArray_int8_T *dciCW
 * Return Type  : void
 */
void nrDCIEncode(const signed char dciBits[39], unsigned int rnti,
                 emxArray_int8_T *dciCW)
{
  static const double b[24] = {1.1920928955078125E-7,
                               2.384185791015625E-7,
                               4.76837158203125E-7,
                               9.5367431640625E-7,
                               1.9073486328125E-6,
                               3.814697265625E-6,
                               7.62939453125E-6,
                               1.52587890625E-5,
                               3.0517578125E-5,
                               6.103515625E-5,
                               0.0001220703125,
                               0.000244140625,
                               0.00048828125,
                               0.0009765625,
                               0.001953125,
                               0.00390625,
                               0.0078125,
                               0.015625,
                               0.03125,
                               0.0625,
                               0.125,
                               0.25,
                               0.5,
                               1.0};
  static const short qSeq[512] = {
      0,   1,   2,   4,   8,   16,  32,  3,   5,   64,  9,   6,   17,  10,  18,
      128, 12,  33,  65,  20,  256, 34,  24,  36,  7,   129, 66,  11,  40,  68,
      130, 19,  13,  48,  14,  72,  257, 21,  132, 35,  258, 26,  80,  37,  25,
      22,  136, 260, 264, 38,  96,  67,  41,  144, 28,  69,  42,  49,  74,  272,
      160, 288, 192, 70,  44,  131, 81,  50,  73,  15,  320, 133, 52,  23,  134,
      384, 76,  137, 82,  56,  27,  97,  39,  259, 84,  138, 145, 261, 29,  43,
      98,  88,  140, 30,  146, 71,  262, 265, 161, 45,  100, 51,  148, 46,  75,
      266, 273, 104, 162, 53,  193, 152, 77,  164, 268, 274, 54,  83,  57,  112,
      135, 78,  289, 194, 85,  276, 58,  168, 139, 99,  86,  60,  280, 89,  290,
      196, 141, 101, 147, 176, 142, 321, 31,  200, 90,  292, 322, 263, 149, 102,
      105, 304, 296, 163, 92,  47,  267, 385, 324, 208, 386, 150, 153, 165, 106,
      55,  328, 113, 154, 79,  269, 108, 224, 166, 195, 270, 275, 291, 59,  169,
      114, 277, 156, 87,  197, 116, 170, 61,  281, 278, 177, 293, 388, 91,  198,
      172, 120, 201, 336, 62,  282, 143, 103, 178, 294, 93,  202, 323, 392, 297,
      107, 180, 151, 209, 284, 94,  204, 298, 400, 352, 325, 155, 210, 305, 300,
      109, 184, 115, 167, 225, 326, 306, 157, 329, 110, 117, 212, 171, 330, 226,
      387, 308, 216, 416, 271, 279, 158, 337, 118, 332, 389, 173, 121, 199, 179,
      228, 338, 312, 390, 174, 393, 283, 122, 448, 353, 203, 63,  340, 394, 181,
      295, 285, 232, 124, 205, 182, 286, 299, 354, 211, 401, 185, 396, 344, 240,
      206, 95,  327, 402, 356, 307, 301, 417, 213, 186, 404, 227, 418, 302, 360,
      111, 331, 214, 309, 188, 449, 217, 408, 229, 159, 420, 310, 333, 119, 339,
      218, 368, 230, 391, 313, 450, 334, 233, 175, 123, 341, 220, 314, 424, 395,
      355, 287, 183, 234, 125, 342, 316, 241, 345, 452, 397, 403, 207, 432, 357,
      187, 236, 126, 242, 398, 346, 456, 358, 405, 303, 244, 189, 361, 215, 348,
      419, 406, 464, 362, 409, 219, 311, 421, 410, 231, 248, 369, 190, 364, 335,
      480, 315, 221, 370, 422, 425, 451, 235, 412, 343, 372, 317, 222, 426, 453,
      237, 433, 347, 243, 454, 318, 376, 428, 238, 359, 457, 399, 434, 349, 245,
      458, 363, 127, 191, 407, 436, 465, 246, 350, 460, 249, 411, 365, 440, 374,
      423, 466, 250, 371, 481, 413, 366, 468, 429, 252, 373, 482, 427, 414, 223,
      472, 455, 377, 435, 319, 484, 430, 488, 239, 378, 459, 437, 380, 461, 496,
      351, 467, 438, 251, 462, 442, 441, 469, 247, 367, 253, 375, 444, 470, 483,
      415, 485, 473, 474, 254, 379, 431, 489, 486, 476, 439, 490, 463, 381, 497,
      492, 443, 382, 498, 445, 471, 500, 446, 475, 487, 504, 255, 477, 491, 478,
      383, 493, 499, 502, 494, 501, 447, 505, 506, 479, 508, 495, 503, 507, 509,
      510, 511};
  static const signed char ak0[4] = {1, 1, 0, 1};
  cell_wrap_8 allG[9];
  emxArray_boolean_T *x;
  emxArray_int16_T *qFtmp;
  emxArray_int8_T *A;
  emxArray_int8_T *c_y;
  emxArray_int8_T *encOut;
  emxArray_real_T *b_jn;
  emxArray_real_T *b_qFtmp;
  emxArray_real_T *b_y;
  emxArray_real_T *r;
  double dividendBlk[25];
  double remBits[25];
  double j;
  int b_i1;
  int b_j;
  int c;
  int i;
  int i1;
  int i2;
  int k;
  int kidx;
  short c_jn[512];
  short qF[449];
  short qI[63];
  short b_i;
  signed char u[512];
  signed char inPad[87];
  signed char blk[63];
  signed char in[63];
  boolean_T blkcrcL[87];
  boolean_T exitg1;
  boolean_T exitg2;
  boolean_T y;
  for (i = 0; i < 24; i++) {
    blk[i] = 1;
  }
  for (i = 0; i < 39; i++) {
    blk[i + 24] = dciBits[i];
  }
  for (i = 0; i < 63; i++) {
    b_i = (short)(blk[i] != 0);
    qI[i] = b_i;
    inPad[i] = (signed char)b_i;
  }
  remBits[0] = 0.0;
  for (i = 0; i < 24; i++) {
    inPad[i + 63] = 0;
    remBits[i + 1] = inPad[i];
  }
  for (i = 0; i < 63; i++) {
    memcpy(&dividendBlk[0], &remBits[1], 24U * sizeof(double));
    dividendBlk[24] = inPad[i + 24];
    if (dividendBlk[0] == 1.0) {
      for (k = 0; k < 25; k++) {
        j = (double)iv5[k] + dividendBlk[k];
        dividendBlk[k] = j;
        remBits[k] = rt_remd_snf(j, 2.0);
      }
    } else {
      memcpy(&remBits[0], &dividendBlk[0], 25U * sizeof(double));
    }
    blkcrcL[i] = (qI[i] != 0);
  }
  for (i = 0; i < 24; i++) {
    blkcrcL[i + 63] = (remBits[i + 1] != 0.0);
  }
  if (rnti != 0U) {
    for (k = 0; k < 24; k++) {
      blkcrcL[k + 63] = ((remBits[k + 1] != 0.0) !=
                         (rt_remd_snf(floor((double)rnti * b[k]), 2.0) > 0.0));
    }
  }
  for (i = 0; i < 39; i++) {
    in[i] = blk[i + 24];
  }
  for (i = 0; i < 24; i++) {
    in[i + 39] = (signed char)blkcrcL[i + 63];
  }
  for (kidx = 0; kidx < 512; kidx++) {
    if (kidx == 0) {
      c = 0;
    } else {
      c = (int)fmod(kidx, 16.0);
    }
    c_jn[kidx] =
        (short)((iv3[(int)floor(32.0 * (double)kidx / 512.0)] << 4) + c);
  }
  emxInit_int16_T(&qFtmp, 2);
  qFtmp->size[0] = 0;
  qFtmp->size[1] = 1;
  emxInit_real_T(&b_jn, 1);
  for (i = 0; i < 80; i++) {
    kidx = qFtmp->size[0];
    i1 = b_jn->size[0];
    b_jn->size[0] = qFtmp->size[0] + 1;
    emxEnsureCapacity_real_T(b_jn, i1);
    for (i1 = 0; i1 < kidx; i1++) {
      b_jn->data[i1] = qFtmp->data[i1];
    }
    b_jn->data[qFtmp->size[0]] = c_jn[i];
    i1 = qFtmp->size[0] * qFtmp->size[1];
    qFtmp->size[0] = b_jn->size[0];
    qFtmp->size[1] = 1;
    emxEnsureCapacity_int16_T(qFtmp, i1);
    kidx = b_jn->size[0];
    for (i1 = 0; i1 < kidx; i1++) {
      qFtmp->data[i1] = (short)b_jn->data[i1];
    }
  }
  emxInit_real_T(&b_qFtmp, 1);
  i1 = b_qFtmp->size[0];
  b_qFtmp->size[0] = qFtmp->size[0] + 168;
  emxEnsureCapacity_real_T(b_qFtmp, i1);
  kidx = qFtmp->size[0];
  for (i1 = 0; i1 < kidx; i1++) {
    b_qFtmp->data[i1] = qFtmp->data[i1];
  }
  for (i1 = 0; i1 < 168; i1++) {
    b_qFtmp->data[i1 + qFtmp->size[0]] = i1;
  }
  emxFree_int16_T(&qFtmp);
  unique_vector(b_qFtmp, b_jn);
  emxFree_real_T(&b_qFtmp);
  memset(&qI[0], 0, 63U * sizeof(short));
  j = 0.0;
  i = 0;
  emxInit_boolean_T(&x, 1);
  exitg1 = false;
  while ((!exitg1) && (i < 512)) {
    i1 = x->size[0];
    x->size[0] = b_jn->size[0];
    emxEnsureCapacity_boolean_T(x, i1);
    kidx = b_jn->size[0];
    for (i1 = 0; i1 < kidx; i1++) {
      x->data[i1] = (qSeq[511 - i] == b_jn->data[i1]);
    }
    y = false;
    c = 1;
    exitg2 = false;
    while ((!exitg2) && (c <= x->size[0])) {
      if (!x->data[c - 1]) {
        c++;
      } else {
        y = true;
        exitg2 = true;
      }
    }
    if (y) {
      i++;
    } else {
      j++;
      qI[(int)j - 1] = qSeq[511 - i];
      if (j == 63.0) {
        exitg1 = true;
      } else {
        i++;
      }
    }
  }
  emxFree_boolean_T(&x);
  memset(&qF[0], 0, 449U * sizeof(short));
  c = -1;
  for (i = 0; i < 512; i++) {
    j = 0.0;
    b_j = 0;
    while ((b_j < 63) && (qI[b_j] != qSeq[i])) {
      j++;
      b_j++;
    }
    if (j == 63.0) {
      c++;
      qF[c] = qSeq[i];
    }
    c_jn[i] = 0;
  }
  for (i = 0; i < 449; i++) {
    c_jn[qF[i]] = 1;
  }
  c = 0;
  for (i = 0; i < 512; i++) {
    u[i] = 0;
    if (c_jn[i] == 0) {
      u[i] = in[iv4[c]];
      c++;
    }
  }
  emxInitMatrix_cell_wrap_8(allG);
  i1 = allG[1].f1->size[0] * allG[1].f1->size[1];
  allG[1].f1->size[0] = 4;
  allG[1].f1->size[1] = 4;
  emxEnsureCapacity_real_T(allG[1].f1, i1);
  allG[1].f1->data[0] = 0.0;
  allG[1].f1->data[1] = 0.0;
  allG[1].f1->data[2] = 0.0;
  allG[1].f1->data[3] = 0.0;
  allG[1].f1->data[4] = 0.0;
  allG[1].f1->data[5] = 0.0;
  allG[1].f1->data[6] = 0.0;
  allG[1].f1->data[7] = 0.0;
  allG[1].f1->data[8] = 0.0;
  allG[1].f1->data[9] = 0.0;
  allG[1].f1->data[10] = 0.0;
  allG[1].f1->data[11] = 0.0;
  allG[1].f1->data[12] = 0.0;
  allG[1].f1->data[13] = 0.0;
  allG[1].f1->data[14] = 0.0;
  allG[1].f1->data[15] = 0.0;
  i1 = allG[2].f1->size[0] * allG[2].f1->size[1];
  allG[2].f1->size[0] = 8;
  allG[2].f1->size[1] = 8;
  emxEnsureCapacity_real_T(allG[2].f1, i1);
  for (i1 = 0; i1 < 64; i1++) {
    allG[2].f1->data[i1] = 0.0;
  }
  i1 = allG[3].f1->size[0] * allG[3].f1->size[1];
  allG[3].f1->size[0] = 16;
  allG[3].f1->size[1] = 16;
  emxEnsureCapacity_real_T(allG[3].f1, i1);
  for (i1 = 0; i1 < 256; i1++) {
    allG[3].f1->data[i1] = 0.0;
  }
  i1 = allG[4].f1->size[0] * allG[4].f1->size[1];
  allG[4].f1->size[0] = 32;
  allG[4].f1->size[1] = 32;
  emxEnsureCapacity_real_T(allG[4].f1, i1);
  for (i1 = 0; i1 < 1024; i1++) {
    allG[4].f1->data[i1] = 0.0;
  }
  i1 = allG[5].f1->size[0] * allG[5].f1->size[1];
  allG[5].f1->size[0] = 64;
  allG[5].f1->size[1] = 64;
  emxEnsureCapacity_real_T(allG[5].f1, i1);
  for (i1 = 0; i1 < 4096; i1++) {
    allG[5].f1->data[i1] = 0.0;
  }
  i1 = allG[6].f1->size[0] * allG[6].f1->size[1];
  allG[6].f1->size[0] = 128;
  allG[6].f1->size[1] = 128;
  emxEnsureCapacity_real_T(allG[6].f1, i1);
  for (i1 = 0; i1 < 16384; i1++) {
    allG[6].f1->data[i1] = 0.0;
  }
  i1 = allG[7].f1->size[0] * allG[7].f1->size[1];
  allG[7].f1->size[0] = 256;
  allG[7].f1->size[1] = 256;
  emxEnsureCapacity_real_T(allG[7].f1, i1);
  for (i1 = 0; i1 < 65536; i1++) {
    allG[7].f1->data[i1] = 0.0;
  }
  i1 = allG[8].f1->size[0] * allG[8].f1->size[1];
  allG[8].f1->size[0] = 512;
  allG[8].f1->size[1] = 512;
  emxEnsureCapacity_real_T(allG[8].f1, i1);
  for (i1 = 0; i1 < 262144; i1++) {
    allG[8].f1->data[i1] = 0.0;
  }
  i1 = allG[0].f1->size[0] * allG[0].f1->size[1];
  allG[0].f1->size[0] = 2;
  allG[0].f1->size[1] = 2;
  emxEnsureCapacity_real_T(allG[0].f1, i1);
  allG[0].f1->data[0] = 1.0;
  allG[0].f1->data[1] = 1.0;
  allG[0].f1->data[2] = 0.0;
  allG[0].f1->data[3] = 1.0;
  emxInit_int8_T(&A, 2);
  for (i = 0; i < 8; i++) {
    i1 = A->size[0] * A->size[1];
    A->size[0] = allG[i].f1->size[0];
    A->size[1] = allG[i].f1->size[1];
    emxEnsureCapacity_int8_T(A, i1);
    kidx = allG[i].f1->size[0] * allG[i].f1->size[1];
    for (i1 = 0; i1 < kidx; i1++) {
      A->data[i1] = (signed char)allG[i].f1->data[i1];
    }
    i1 = allG[i].f1->size[0];
    i2 = allG[i].f1->size[1];
    c = allG[i + 1].f1->size[0] * allG[i + 1].f1->size[1];
    allG[i + 1].f1->size[0] = allG[i].f1->size[0] << 1;
    allG[i + 1].f1->size[1] = allG[i].f1->size[1] << 1;
    emxEnsureCapacity_real_T(allG[i + 1].f1, c);
    kidx = -1;
    for (k = 0; k < i2; k++) {
      for (b_j = 0; b_j < 2; b_j++) {
        for (b_i1 = 0; b_i1 < i1; b_i1++) {
          c = b_j << 1;
          allG[i + 1].f1->data[kidx + 1] =
              A->data[b_i1 + A->size[0] * k] * ak0[c];
          allG[i + 1].f1->data[kidx + 2] =
              A->data[b_i1 + A->size[0] * k] * ak0[c + 1];
          kidx += 2;
        }
      }
    }
  }
  emxFree_int8_T(&A);
  emxInit_real_T(&b_y, 2);
  kidx = allG[8].f1->size[1];
  i1 = b_y->size[0] * b_y->size[1];
  b_y->size[0] = 1;
  b_y->size[1] = allG[8].f1->size[1];
  emxEnsureCapacity_real_T(b_y, i1);
  for (b_j = 0; b_j < kidx; b_j++) {
    c = b_j * allG[8].f1->size[0];
    j = 0.0;
    for (k = 0; k < 512; k++) {
      j += (double)(u[k] * (signed char)allG[8].f1->data[c + k]);
    }
    b_y->data[b_j] = j;
  }
  emxFreeMatrix_cell_wrap_8(allG);
  emxInit_real_T(&r, 2);
  i1 = r->size[0] * r->size[1];
  r->size[0] = 1;
  r->size[1] = b_y->size[1];
  emxEnsureCapacity_real_T(r, i1);
  c = b_y->size[1];
  for (k = 0; k < c; k++) {
    j = b_y->data[k];
    if (rtIsInf(j)) {
      j = rtNaN;
    } else if (j == 0.0) {
      j = 0.0;
    } else {
      j = fmod(j, 2.0);
      if (j == 0.0) {
        j = 0.0;
      }
    }
    r->data[k] = j;
  }
  emxFree_real_T(&b_y);
  emxInit_int8_T(&encOut, 1);
  i1 = encOut->size[0];
  encOut->size[0] = r->size[1];
  emxEnsureCapacity_int8_T(encOut, i1);
  kidx = r->size[1];
  for (i1 = 0; i1 < kidx; i1++) {
    encOut->data[i1] = (signed char)r->data[i1];
  }
  emxFree_real_T(&r);
  b_log2(encOut->size[0]);
  b_log2(encOut->size[0]);
  c = encOut->size[0];
  i1 = b_jn->size[0];
  b_jn->size[0] = encOut->size[0];
  emxEnsureCapacity_real_T(b_jn, i1);
  kidx = encOut->size[0];
  for (i1 = 0; i1 < kidx; i1++) {
    b_jn->data[i1] = 0.0;
  }
  i1 = encOut->size[0];
  for (kidx = 0; kidx < i1; kidx++) {
    j = (double)c / 32.0;
    b_jn->data[kidx] =
        (double)iv3[(int)(floor(32.0 * (double)kidx / (double)c) + 1.0) - 1] *
            j +
        b_mod(kidx, j);
  }
  emxInit_int8_T(&c_y, 1);
  i1 = c_y->size[0];
  c_y->size[0] = b_jn->size[0];
  emxEnsureCapacity_int8_T(c_y, i1);
  kidx = b_jn->size[0];
  for (i1 = 0; i1 < kidx; i1++) {
    c_y->data[i1] = encOut->data[(int)(b_jn->data[i1] + 1.0) - 1];
  }
  i1 = dciCW->size[0];
  dciCW->size[0] = 432;
  emxEnsureCapacity_int8_T(dciCW, i1);
  for (i1 = 0; i1 < 432; i1++) {
    dciCW->data[i1] = 0;
  }
  if (432 >= encOut->size[0]) {
    for (k = 0; k < 432; k++) {
      dciCW->data[k] =
          encOut->data[(int)(b_jn->data[(int)(b_mod(k, encOut->size[0]) + 1.0) -
                                        1] +
                             1.0) -
                       1];
    }
  } else {
    if (b_jn->size[0] - 431 > b_jn->size[0]) {
      i1 = 0;
      i2 = 0;
    } else {
      i1 = b_jn->size[0] - 432;
      i2 = b_jn->size[0];
    }
    kidx = i2 - i1;
    i2 = dciCW->size[0];
    dciCW->size[0] = kidx;
    emxEnsureCapacity_int8_T(dciCW, i2);
    for (i2 = 0; i2 < kidx; i2++) {
      dciCW->data[i2] = c_y->data[i1 + i2];
    }
  }
  emxFree_real_T(&b_jn);
  emxFree_int8_T(&c_y);
  emxFree_int8_T(&encOut);
}

/*
 * File trailer for nrDCIEncode.c
 *
 * [EOF]
 */

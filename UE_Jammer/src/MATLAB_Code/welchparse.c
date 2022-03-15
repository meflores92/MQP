/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: welchparse.c
 *
 * MATLAB Coder version            : 5.2
 * C/C++ source code generated on  : 20-Feb-2022 08:55:56
 */

/* Include Files */
#include "welchparse.h"
#include "operation_data.h"
#include "operation_internal_types.h"
#include "rt_nonfinite.h"
#include <string.h>

/* Type Definitions */
#ifndef struct_emxArray_char_T_1x8
#define struct_emxArray_char_T_1x8
struct emxArray_char_T_1x8 {
  char data[8];
  int size[2];
};
#endif /* struct_emxArray_char_T_1x8 */
#ifndef typedef_emxArray_char_T_1x8
#define typedef_emxArray_char_T_1x8
typedef struct emxArray_char_T_1x8 emxArray_char_T_1x8;
#endif /* typedef_emxArray_char_T_1x8 */

#ifndef typedef_cell_wrap_71
#define typedef_cell_wrap_71
typedef struct {
  emxArray_char_T_1x8 f1;
} cell_wrap_71;
#endif /* typedef_cell_wrap_71 */

/* Function Definitions */
/*
 * Arguments    : const cell_wrap_66 *x1
 *                creal32_T x[921600]
 *                char options_range[8]
 * Return Type  : void
 */
void welchparse(const cell_wrap_66 *x1, creal32_T x[921600],
                char options_range[8])
{
  static const char b_cv[8] = {'c', 'e', 'n', 't', 'e', 'r', 'e', 'd'};
  cell_wrap_71 exclusiveOpts[14];
  cell_wrap_71 unnamed_idx_0;
  cell_wrap_71 unnamed_idx_1;
  cell_wrap_71 unnamed_idx_10;
  cell_wrap_71 unnamed_idx_11;
  cell_wrap_71 unnamed_idx_12;
  cell_wrap_71 unnamed_idx_13;
  cell_wrap_71 unnamed_idx_2;
  cell_wrap_71 unnamed_idx_3;
  cell_wrap_71 unnamed_idx_4;
  cell_wrap_71 unnamed_idx_5;
  cell_wrap_71 unnamed_idx_6;
  cell_wrap_71 unnamed_idx_7;
  cell_wrap_71 unnamed_idx_8;
  cell_wrap_71 unnamed_idx_9;
  int exitg1;
  int exitg2;
  int i;
  int kstr;
  boolean_T b_bool[3];
  boolean_T exitg3;
  boolean_T y;
  memcpy(&x[0], &x1->f1[0], 921600U * sizeof(creal32_T));
  unnamed_idx_0.f1.size[0] = 1;
  unnamed_idx_0.f1.size[1] = 4;
  unnamed_idx_0.f1.data[0] = 'h';
  unnamed_idx_0.f1.data[1] = 'a';
  unnamed_idx_0.f1.data[2] = 'l';
  unnamed_idx_0.f1.data[3] = 'f';
  unnamed_idx_1.f1.size[0] = 1;
  unnamed_idx_1.f1.size[1] = 8;
  unnamed_idx_1.f1.data[0] = 'o';
  unnamed_idx_1.f1.data[1] = 'n';
  unnamed_idx_1.f1.data[2] = 'e';
  unnamed_idx_1.f1.data[3] = 's';
  unnamed_idx_1.f1.data[4] = 'i';
  unnamed_idx_1.f1.data[5] = 'd';
  unnamed_idx_1.f1.data[6] = 'e';
  unnamed_idx_1.f1.data[7] = 'd';
  unnamed_idx_2.f1.size[0] = 1;
  unnamed_idx_2.f1.size[1] = 5;
  unnamed_idx_3.f1.size[0] = 1;
  unnamed_idx_3.f1.size[1] = 5;
  unnamed_idx_4.f1.size[0] = 1;
  unnamed_idx_4.f1.size[1] = 5;
  unnamed_idx_2.f1.data[0] = 'a';
  unnamed_idx_3.f1.data[0] = 'a';
  unnamed_idx_4.f1.data[0] = 'u';
  unnamed_idx_2.f1.data[1] = 'd';
  unnamed_idx_3.f1.data[1] = 'd';
  unnamed_idx_4.f1.data[1] = 'n';
  unnamed_idx_2.f1.data[2] = 'a';
  unnamed_idx_3.f1.data[2] = 'a';
  unnamed_idx_4.f1.data[2] = 'i';
  unnamed_idx_2.f1.data[3] = 'p';
  unnamed_idx_3.f1.data[3] = 'p';
  unnamed_idx_4.f1.data[3] = 't';
  unnamed_idx_2.f1.data[4] = 't';
  unnamed_idx_3.f1.data[4] = 't';
  unnamed_idx_4.f1.data[4] = 'y';
  unnamed_idx_5.f1.size[0] = 1;
  unnamed_idx_5.f1.size[1] = 4;
  unnamed_idx_5.f1.data[0] = 'h';
  unnamed_idx_5.f1.data[1] = 'a';
  unnamed_idx_5.f1.data[2] = 'l';
  unnamed_idx_5.f1.data[3] = 'f';
  unnamed_idx_6.f1.size[0] = 1;
  unnamed_idx_6.f1.size[1] = 8;
  unnamed_idx_6.f1.data[0] = 'o';
  unnamed_idx_6.f1.data[1] = 'n';
  unnamed_idx_6.f1.data[2] = 'e';
  unnamed_idx_6.f1.data[3] = 's';
  unnamed_idx_6.f1.data[4] = 'i';
  unnamed_idx_6.f1.data[5] = 'd';
  unnamed_idx_6.f1.data[6] = 'e';
  unnamed_idx_6.f1.data[7] = 'd';
  unnamed_idx_7.f1.size[0] = 1;
  unnamed_idx_7.f1.size[1] = 5;
  unnamed_idx_7.f1.data[0] = 'w';
  unnamed_idx_7.f1.data[1] = 'h';
  unnamed_idx_7.f1.data[2] = 'o';
  unnamed_idx_7.f1.data[3] = 'l';
  unnamed_idx_7.f1.data[4] = 'e';
  unnamed_idx_8.f1.size[0] = 1;
  unnamed_idx_8.f1.size[1] = 8;
  unnamed_idx_8.f1.data[0] = 't';
  unnamed_idx_8.f1.data[1] = 'w';
  unnamed_idx_8.f1.data[2] = 'o';
  unnamed_idx_8.f1.data[3] = 's';
  unnamed_idx_8.f1.data[4] = 'i';
  unnamed_idx_8.f1.data[5] = 'd';
  unnamed_idx_8.f1.data[6] = 'e';
  unnamed_idx_8.f1.data[7] = 'd';
  unnamed_idx_9.f1.size[0] = 1;
  unnamed_idx_9.f1.size[1] = 5;
  unnamed_idx_10.f1.size[0] = 1;
  unnamed_idx_10.f1.size[1] = 5;
  unnamed_idx_11.f1.size[0] = 1;
  unnamed_idx_11.f1.size[1] = 5;
  unnamed_idx_9.f1.data[0] = 'u';
  unnamed_idx_10.f1.data[0] = 'e';
  unnamed_idx_11.f1.data[0] = 'e';
  unnamed_idx_9.f1.data[1] = 'n';
  unnamed_idx_10.f1.data[1] = 'i';
  unnamed_idx_11.f1.data[1] = 'i';
  unnamed_idx_9.f1.data[2] = 'i';
  unnamed_idx_10.f1.data[2] = 'g';
  unnamed_idx_11.f1.data[2] = 'g';
  unnamed_idx_9.f1.data[3] = 't';
  unnamed_idx_10.f1.data[3] = 'e';
  unnamed_idx_11.f1.data[3] = 'e';
  unnamed_idx_9.f1.data[4] = 'y';
  unnamed_idx_10.f1.data[4] = 'n';
  unnamed_idx_11.f1.data[4] = 'n';
  unnamed_idx_12.f1.size[0] = 1;
  unnamed_idx_12.f1.size[1] = 8;
  unnamed_idx_13.f1.size[0] = 1;
  unnamed_idx_13.f1.size[1] = 8;
  unnamed_idx_12.f1.data[0] = 'c';
  unnamed_idx_13.f1.data[0] = 'c';
  unnamed_idx_12.f1.data[1] = 'e';
  unnamed_idx_13.f1.data[1] = 'e';
  unnamed_idx_12.f1.data[2] = 'n';
  unnamed_idx_13.f1.data[2] = 'n';
  unnamed_idx_12.f1.data[3] = 't';
  unnamed_idx_13.f1.data[3] = 't';
  unnamed_idx_12.f1.data[4] = 'e';
  unnamed_idx_13.f1.data[4] = 'e';
  unnamed_idx_12.f1.data[5] = 'r';
  unnamed_idx_13.f1.data[5] = 'r';
  unnamed_idx_12.f1.data[6] = 'e';
  unnamed_idx_13.f1.data[6] = 'e';
  unnamed_idx_12.f1.data[7] = 'd';
  unnamed_idx_13.f1.data[7] = 'd';
  exclusiveOpts[0] = unnamed_idx_0;
  exclusiveOpts[1] = unnamed_idx_1;
  exclusiveOpts[2] = unnamed_idx_2;
  exclusiveOpts[3] = unnamed_idx_3;
  exclusiveOpts[4] = unnamed_idx_4;
  exclusiveOpts[5] = unnamed_idx_5;
  exclusiveOpts[6] = unnamed_idx_6;
  exclusiveOpts[7] = unnamed_idx_7;
  exclusiveOpts[8] = unnamed_idx_8;
  exclusiveOpts[9] = unnamed_idx_9;
  exclusiveOpts[10] = unnamed_idx_10;
  exclusiveOpts[11] = unnamed_idx_11;
  exclusiveOpts[12] = unnamed_idx_12;
  exclusiveOpts[13] = unnamed_idx_13;
  i = 0;
  do {
    exitg1 = 0;
    if (i < 7) {
      b_bool[0] = false;
      b_bool[1] = false;
      if (exclusiveOpts[i].f1.size[1] == 8) {
        kstr = 0;
        do {
          exitg2 = 0;
          if (kstr < 8) {
            if (cv[(int)exclusiveOpts[i].f1.data[kstr]] !=
                cv[(int)b_cv[kstr]]) {
              exitg2 = 1;
            } else {
              kstr++;
            }
          } else {
            b_bool[1] = true;
            exitg2 = 1;
          }
        } while (exitg2 == 0);
      }
      b_bool[2] = false;
      y = false;
      kstr = 0;
      exitg3 = false;
      while ((!exitg3) && (kstr <= 2)) {
        if (!b_bool[kstr]) {
          kstr++;
        } else {
          y = true;
          exitg3 = true;
        }
      }
      if (y) {
        b_bool[0] = false;
        b_bool[1] = false;
        if (exclusiveOpts[i + 7].f1.size[1] == 8) {
          kstr = 0;
          do {
            exitg2 = 0;
            if (kstr < 8) {
              if (cv[(int)exclusiveOpts[i + 7].f1.data[kstr]] !=
                  cv[(int)b_cv[kstr]]) {
                exitg2 = 1;
              } else {
                kstr++;
              }
            } else {
              b_bool[1] = true;
              exitg2 = 1;
            }
          } while (exitg2 == 0);
        }
        b_bool[2] = false;
        y = false;
        kstr = 0;
        exitg3 = false;
        while ((!exitg3) && (kstr <= 2)) {
          if (!b_bool[kstr]) {
            kstr++;
          } else {
            y = true;
            exitg3 = true;
          }
        }
        if (y) {
          exitg1 = 1;
        } else {
          i++;
        }
      } else {
        i++;
      }
    } else {
      options_range[0] = 't';
      options_range[1] = 'w';
      options_range[2] = 'o';
      options_range[3] = 's';
      options_range[4] = 'i';
      options_range[5] = 'd';
      options_range[6] = 'e';
      options_range[7] = 'd';
      exitg1 = 1;
    }
  } while (exitg1 == 0);
}

/*
 * File trailer for welchparse.c
 *
 * [EOF]
 */

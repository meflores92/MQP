/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: eml_setop.c
 *
 * MATLAB Coder version            : 5.2
 * C/C++ source code generated on  : 19-Jan-2022 16:08:45
 */

/* Include Files */
#include "eml_setop.h"
#include "operation_data.h"
#include <math.h>

/* Function Declarations */
static double skip_to_last_equal_value(int *k, const double x[1536]);

/* Function Definitions */
/*
 * Arguments    : int *k
 *                const double x[1536]
 * Return Type  : double
 */
static double skip_to_last_equal_value(int *k, const double x[1536])
{
  double absx;
  double xk;
  int exponent;
  boolean_T exitg1;
  xk = x[*k - 1];
  exitg1 = false;
  while ((!exitg1) && (*k < 1536)) {
    absx = fabs(xk / 2.0);
    if (absx <= 2.2250738585072014E-308) {
      absx = 4.94065645841247E-324;
    } else {
      frexp(absx, &exponent);
      absx = ldexp(1.0, exponent - 53);
    }
    if (fabs(xk - x[*k]) < absx) {
      (*k)++;
    } else {
      exitg1 = true;
    }
  }
  return xk;
}

/*
 * Arguments    : double c_data[]
 *                int *c_size
 *                int ia_data[]
 *                int *ia_size
 *                int *ib_size
 * Return Type  : void
 */
void do_vectors(double c_data[], int *c_size, int ia_data[], int *ia_size,
                int *ib_size)
{
  double dv[1536];
  double ak;
  int a;
  int b_exponent;
  int b_ialast;
  int exponent;
  int i;
  int iafirst;
  int ialast;
  int iblast;
  int nc;
  int nia;
  boolean_T exitg1;
  *ib_size = 0;
  nc = -1;
  nia = -1;
  iafirst = 0;
  ialast = 1;
  iblast = 1;
  while ((ialast <= 1536) && (iblast <= 1296)) {
    b_ialast = ialast;
    for (a = 0; a < 1536; a++) {
      dv[a] = (double)a + 1.0;
    }
    ak = skip_to_last_equal_value(&b_ialast, dv);
    ialast = b_ialast;
    a = iv1[iblast - 1];
    exitg1 = false;
    while ((!exitg1) && (iblast < 1296)) {
      frexp((double)a / 2.0, &b_exponent);
      if (fabs((double)(a - iv1[iblast])) < ldexp(1.0, b_exponent - 53)) {
        iblast++;
      } else {
        exitg1 = true;
      }
    }
    frexp((double)a / 2.0, &exponent);
    if (fabs((double)a - ak) < ldexp(1.0, exponent - 53)) {
      ialast = b_ialast + 1;
      iafirst = b_ialast;
      iblast++;
    } else if (ak < a) {
      nc++;
      nia++;
      c_data[nc] = ak;
      ia_data[nia] = iafirst + 1;
      ialast = b_ialast + 1;
      iafirst = b_ialast;
    } else {
      iblast++;
    }
  }
  if (ialast <= 1536) {
#pragma omp parallel for num_threads(omp_get_max_threads()) private(i)

    for (i = 0; i < 1536; i++) {
      dv[i] = (double)i + 1.0;
    }
  }
  while (ialast <= 1536) {
    a = ialast;
    ak = skip_to_last_equal_value(&a, dv);
    nc++;
    nia++;
    c_data[nc] = ak;
    ia_data[nia] = iafirst + 1;
    ialast = a + 1;
    iafirst = a;
  }
  if (1 > nia + 1) {
    nia = -1;
  }
  *ia_size = nia + 1;
  if (1 > nc + 1) {
    nc = -1;
  }
  *c_size = nc + 1;
}

/*
 * File trailer for eml_setop.c
 *
 * [EOF]
 */

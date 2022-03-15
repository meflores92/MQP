/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: toc.c
 *
 * MATLAB Coder version            : 5.2
 * C/C++ source code generated on  : 18-Jan-2022 10:16:38
 */

/* Include Files */
#include "toc.h"
#include "get_tic_data.h"
#include "timeKeeper.h"
#include "coder_posix_time.h"
#include <stdio.h>

/* Function Definitions */
/*
 * Arguments    : void
 * Return Type  : void
 */
void toc(void)
{
  coderTimespec b_timespec;
  double tstart_tv_nsec;
  double tstart_tv_sec;
  b_timeKeeper(&tstart_tv_sec, &tstart_tv_nsec);
  if (!freq_not_empty) {
    freq_not_empty = true;
    coderInitTimeFunctions(&freq);
  }
  coderTimeClockGettimeMonotonic(&b_timespec, freq);
  printf("Elapsed time is %f seconds\n",
         (b_timespec.tv_sec - tstart_tv_sec) +
             (b_timespec.tv_nsec - tstart_tv_nsec) / 1.0E+9);
  fflush(stdout);
}

/*
 * File trailer for toc.c
 *
 * [EOF]
 */

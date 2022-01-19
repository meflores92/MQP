/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: operation.c
 *
 * MATLAB Coder version            : 5.2
 * C/C++ source code generated on  : 19-Jan-2022 16:08:45
 */

/* Include Files */
#include "operation.h"
#include "SyncFrame.h"
#include "SyncFrameInit.h"
#include "operation_data.h"
#include "operation_initialize.h"

/* Function Definitions */
/*
 * Arguments    : const creal32_T rxWaveform[2754621]
 *                double *offset1
 *                double *offset2
 * Return Type  : void
 */
void operation(const creal32_T rxWaveform[2754621], double *offset1,
               double *offset2)
{
  if (!isInitialized_operation) {
    operation_initialize();
  }
  *offset1 = SyncFrameInit(*(creal32_T(*)[921600]) & rxWaveform[0]);
  *offset2 = SyncFrame(*(creal32_T(*)[6584]) & rxWaveform[0]);
}

/*
 * File trailer for operation.c
 *
 * [EOF]
 */

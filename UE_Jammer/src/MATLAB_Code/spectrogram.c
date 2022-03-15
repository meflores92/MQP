/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: spectrogram.c
 *
 * MATLAB Coder version            : 5.2
 * C/C++ source code generated on  : 20-Feb-2022 08:55:56
 */

/* Include Files */
#include "spectrogram.h"
#include "computeDFT.h"
#include "operation_emxutil.h"
#include "operation_internal_types.h"
#include "operation_types.h"
#include "pspectrogram.h"
#include "rt_nonfinite.h"
#include "welchparse.h"
#include <string.h>

/* Function Definitions */
/*
 * Arguments    : const creal32_T x[921600]
 *                emxArray_creal32_T *varargout_1
 *                float varargout_2_data[]
 *                int *varargout_2_size
 *                float varargout_3[600]
 *                emxArray_real32_T *varargout_4
 * Return Type  : void
 */
void spectrogram(const creal32_T x[921600], emxArray_creal32_T *varargout_1,
                 float varargout_2_data[], int *varargout_2_size,
                 float varargout_3[600], emxArray_real32_T *varargout_4)
{
  static cell_wrap_66 r;
  static creal32_T xw[921600];
  static const double dv[600] = {
      1.6666666666666667E-5,  5.0E-5,  8.3333333333333331E-5,
      0.00011666666666666667, 0.00015, 0.00018333333333333334,
      0.00021666666666666666, 0.00025, 0.00028333333333333335,
      0.00031666666666666665, 0.00035, 0.00038333333333333334,
      0.00041666666666666669, 0.00045, 0.00048333333333333334,
      0.00051666666666666668, 0.00055, 0.00058333333333333338,
      0.00061666666666666662, 0.00065, 0.00068333333333333332,
      0.00071666666666666667, 0.00075, 0.00078333333333333336,
      0.00081666666666666671, 0.00085, 0.0008833333333333333,
      0.00091666666666666665, 0.00095, 0.00098333333333333324,
      0.0010166666666666666,  0.00105, 0.0010833333333333333,
      0.0011166666666666666,  0.00115, 0.0011833333333333333,
      0.0012166666666666667,  0.00125, 0.0012833333333333334,
      0.0013166666666666667,  0.00135, 0.0013833333333333334,
      0.0014166666666666668,  0.00145, 0.0014833333333333332,
      0.0015166666666666666,  0.00155, 0.0015833333333333333,
      0.0016166666666666666,  0.00165, 0.0016833333333333333,
      0.0017166666666666667,  0.00175, 0.0017833333333333334,
      0.0018166666666666667,  0.00185, 0.0018833333333333334,
      0.0019166666666666666,  0.00195, 0.0019833333333333335,
      0.0020166666666666666,  0.00205, 0.0020833333333333333,
      0.0021166666666666669,  0.00215, 0.0021833333333333331,
      0.0022166666666666667,  0.00225, 0.0022833333333333334,
      0.0023166666666666665,  0.00235, 0.0023833333333333332,
      0.0024166666666666668,  0.00245, 0.0024833333333333335,
      0.0025166666666666666,  0.00255, 0.0025833333333333333,
      0.0026166666666666669,  0.00265, 0.0026833333333333331,
      0.0027166666666666667,  0.00275, 0.0027833333333333334,
      0.0028166666666666665,  0.00285, 0.0028833333333333332,
      0.0029166666666666668,  0.00295, 0.0029833333333333335,
      0.0030166666666666666,  0.00305, 0.0030833333333333333,
      0.0031166666666666665,  0.00315, 0.0031833333333333332,
      0.0032166666666666667,  0.00325, 0.0032833333333333334,
      0.0033166666666666665,  0.00335, 0.0033833333333333332,
      0.0034166666666666668,  0.00345, 0.0034833333333333335,
      0.0035166666666666666,  0.00355, 0.0035833333333333333,
      0.0036166666666666665,  0.00365, 0.0036833333333333332,
      0.0037166666666666667,  0.00375, 0.0037833333333333334,
      0.0038166666666666666,  0.00385, 0.0038833333333333333,
      0.0039166666666666664,  0.00395, 0.0039833333333333335,
      0.0040166666666666666,  0.00405, 0.0040833333333333329,
      0.0041166666666666669,  0.00415, 0.0041833333333333332,
      0.0042166666666666663,  0.00425, 0.0042833333333333334,
      0.0043166666666666666,  0.00435, 0.0043833333333333337,
      0.0044166666666666668,  0.00445, 0.0044833333333333331,
      0.0045166666666666671,  0.00455, 0.0045833333333333334,
      0.0046166666666666665,  0.00465, 0.0046833333333333336,
      0.0047166666666666668,  0.00475, 0.004783333333333333,
      0.004816666666666667,   0.00485, 0.0048833333333333333,
      0.0049166666666666664,  0.00495, 0.0049833333333333335,
      0.0050166666666666667,  0.00505, 0.0050833333333333329,
      0.0051166666666666669,  0.00515, 0.0051833333333333332,
      0.0052166666666666663,  0.00525, 0.0052833333333333335,
      0.0053166666666666666,  0.00535, 0.0053833333333333337,
      0.0054166666666666669,  0.00545, 0.0054833333333333331,
      0.0055166666666666662,  0.00555, 0.0055833333333333334,
      0.0056166666666666665,  0.00565, 0.0056833333333333336,
      0.0057166666666666668,  0.00575, 0.005783333333333333,
      0.005816666666666667,   0.00585, 0.0058833333333333333,
      0.0059166666666666664,  0.00595, 0.0059833333333333336,
      0.0060166666666666667,  0.00605, 0.006083333333333333,
      0.006116666666666667,   0.00615, 0.0061833333333333332,
      0.0062166666666666663,  0.00625, 0.0062833333333333335,
      0.0063166666666666666,  0.00635, 0.0063833333333333337,
      0.0064166666666666669,  0.00645, 0.0064833333333333331,
      0.0065166666666666663,  0.00655, 0.0065833333333333334,
      0.0066166666666666665,  0.00665, 0.0066833333333333337,
      0.0067166666666666668,  0.00675, 0.0067833333333333331,
      0.0068166666666666671,  0.00685, 0.0068833333333333333,
      0.0069166666666666664,  0.00695, 0.0069833333333333336,
      0.0070166666666666667,  0.00705, 0.007083333333333333,
      0.007116666666666667,   0.00715, 0.0071833333333333332,
      0.0072166666666666664,  0.00725, 0.0072833333333333335,
      0.0073166666666666666,  0.00735, 0.0073833333333333338,
      0.0074166666666666669,  0.00745, 0.0074833333333333332,
      0.0075166666666666663,  0.00755, 0.0075833333333333334,
      0.0076166666666666666,  0.00765, 0.0076833333333333337,
      0.0077166666666666668,  0.00775, 0.0077833333333333331,
      0.0078166666666666662,  0.00785, 0.0078833333333333325,
      0.0079166666666666673,  0.00795, 0.0079833333333333336,
      0.0080166666666666667,  0.00805, 0.008083333333333333,
      0.0081166666666666661,  0.00815, 0.0081833333333333341,
      0.0082166666666666673,  0.00825, 0.0082833333333333335,
      0.0083166666666666667,  0.00835, 0.0083833333333333329,
      0.008416666666666666,   0.00845, 0.008483333333333334,
      0.0085166666666666672,  0.00855, 0.0085833333333333334,
      0.0086166666666666666,  0.00865, 0.0086833333333333328,
      0.008716666666666666,   0.00875, 0.008783333333333334,
      0.0088166666666666671,  0.00885, 0.0088833333333333334,
      0.0089166666666666665,  0.00895, 0.0089833333333333328,
      0.0090166666666666659,  0.00905, 0.0090833333333333339,
      0.009116666666666667,   0.00915, 0.0091833333333333333,
      0.0092166666666666664,  0.00925, 0.0092833333333333327,
      0.0093166666666666658,  0.00935, 0.0093833333333333338,
      0.0094166666666666669,  0.00945, 0.0094833333333333332,
      0.0095166666666666663,  0.00955, 0.0095833333333333326,
      0.0096166666666666675,  0.00965, 0.0096833333333333337,
      0.0097166666666666669,  0.00975, 0.0097833333333333331,
      0.0098166666666666662,  0.00985, 0.0098833333333333325,
      0.0099166666666666674,  0.00995, 0.0099833333333333336,
      0.010016666666666667,   0.01005, 0.010083333333333333,
      0.010116666666666666,   0.01015, 0.010183333333333334,
      0.010216666666666667,   0.01025, 0.010283333333333334,
      0.010316666666666667,   0.01035, 0.010383333333333333,
      0.010416666666666666,   0.01045, 0.010483333333333334,
      0.010516666666666667,   0.01055, 0.010583333333333333,
      0.010616666666666667,   0.01065, 0.010683333333333333,
      0.010716666666666666,   0.01075, 0.010783333333333334,
      0.010816666666666667,   0.01085, 0.010883333333333333,
      0.010916666666666667,   0.01095, 0.010983333333333333,
      0.011016666666666666,   0.01105, 0.011083333333333334,
      0.011116666666666667,   0.01115, 0.011183333333333333,
      0.011216666666666666,   0.01125, 0.011283333333333333,
      0.011316666666666666,   0.01135, 0.011383333333333334,
      0.011416666666666667,   0.01145, 0.011483333333333333,
      0.011516666666666666,   0.01155, 0.011583333333333333,
      0.011616666666666668,   0.01165, 0.011683333333333334,
      0.011716666666666667,   0.01175, 0.011783333333333333,
      0.011816666666666666,   0.01185, 0.011883333333333333,
      0.011916666666666667,   0.01195, 0.011983333333333334,
      0.012016666666666667,   0.01205, 0.012083333333333333,
      0.012116666666666666,   0.01215, 0.012183333333333332,
      0.012216666666666667,   0.01225, 0.012283333333333334,
      0.012316666666666667,   0.01235, 0.012383333333333333,
      0.012416666666666666,   0.01245, 0.012483333333333334,
      0.012516666666666667,   0.01255, 0.012583333333333334,
      0.012616666666666667,   0.01265, 0.012683333333333333,
      0.012716666666666666,   0.01275, 0.012783333333333334,
      0.012816666666666667,   0.01285, 0.012883333333333333,
      0.012916666666666667,   0.01295, 0.012983333333333333,
      0.013016666666666666,   0.01305, 0.013083333333333334,
      0.013116666666666667,   0.01315, 0.013183333333333333,
      0.013216666666666666,   0.01325, 0.013283333333333333,
      0.013316666666666666,   0.01335, 0.013383333333333334,
      0.013416666666666667,   0.01345, 0.013483333333333333,
      0.013516666666666666,   0.01355, 0.013583333333333333,
      0.013616666666666666,   0.01365, 0.013683333333333334,
      0.013716666666666667,   0.01375, 0.013783333333333333,
      0.013816666666666666,   0.01385, 0.013883333333333333,
      0.013916666666666667,   0.01395, 0.013983333333333334,
      0.014016666666666667,   0.01405, 0.014083333333333333,
      0.014116666666666666,   0.01415, 0.014183333333333333,
      0.014216666666666667,   0.01425, 0.014283333333333334,
      0.014316666666666667,   0.01435, 0.014383333333333333,
      0.014416666666666666,   0.01445, 0.014483333333333334,
      0.014516666666666667,   0.01455, 0.014583333333333334,
      0.014616666666666667,   0.01465, 0.014683333333333333,
      0.014716666666666666,   0.01475, 0.014783333333333334,
      0.014816666666666667,   0.01485, 0.014883333333333333,
      0.014916666666666667,   0.01495, 0.014983333333333333,
      0.015016666666666666,   0.01505, 0.015083333333333334,
      0.015116666666666667,   0.01515, 0.015183333333333333,
      0.015216666666666667,   0.01525, 0.015283333333333333,
      0.015316666666666666,   0.01535, 0.015383333333333334,
      0.015416666666666667,   0.01545, 0.015483333333333333,
      0.015516666666666666,   0.01555, 0.015583333333333333,
      0.015616666666666666,   0.01565, 0.015683333333333334,
      0.015716666666666667,   0.01575, 0.015783333333333333,
      0.015816666666666666,   0.01585, 0.015883333333333333,
      0.015916666666666666,   0.01595, 0.015983333333333332,
      0.016016666666666665,   0.01605, 0.016083333333333335,
      0.016116666666666668,   0.01615, 0.016183333333333334,
      0.016216666666666667,   0.01625, 0.016283333333333334,
      0.016316666666666667,   0.01635, 0.016383333333333333,
      0.016416666666666666,   0.01645, 0.016483333333333332,
      0.016516666666666666,   0.01655, 0.016583333333333332,
      0.016616666666666665,   0.01665, 0.016683333333333335,
      0.016716666666666668,   0.01675, 0.016783333333333334,
      0.016816666666666667,   0.01685, 0.016883333333333334,
      0.016916666666666667,   0.01695, 0.016983333333333333,
      0.017016666666666666,   0.01705, 0.017083333333333332,
      0.017116666666666665,   0.01715, 0.017183333333333332,
      0.017216666666666668,   0.01725, 0.017283333333333335,
      0.017316666666666668,   0.01735, 0.017383333333333334,
      0.017416666666666667,   0.01745, 0.017483333333333333,
      0.017516666666666666,   0.01755, 0.017583333333333333,
      0.017616666666666666,   0.01765, 0.017683333333333332,
      0.017716666666666665,   0.01775, 0.017783333333333335,
      0.017816666666666668,   0.01785, 0.017883333333333334,
      0.017916666666666668,   0.01795, 0.017983333333333334,
      0.018016666666666667,   0.01805, 0.018083333333333333,
      0.018116666666666666,   0.01815, 0.018183333333333333,
      0.018216666666666666,   0.01825, 0.018283333333333332,
      0.018316666666666665,   0.01835, 0.018383333333333335,
      0.018416666666666668,   0.01845, 0.018483333333333334,
      0.018516666666666667,   0.01855, 0.018583333333333334,
      0.018616666666666667,   0.01865, 0.018683333333333333,
      0.018716666666666666,   0.01875, 0.018783333333333332,
      0.018816666666666666,   0.01885, 0.018883333333333332,
      0.018916666666666665,   0.01895, 0.018983333333333335,
      0.019016666666666668,   0.01905, 0.019083333333333334,
      0.019116666666666667,   0.01915, 0.019183333333333333,
      0.019216666666666667,   0.01925, 0.019283333333333333,
      0.019316666666666666,   0.01935, 0.019383333333333332,
      0.019416666666666665,   0.01945, 0.019483333333333332,
      0.019516666666666668,   0.01955, 0.019583333333333335,
      0.019616666666666668,   0.01965, 0.019683333333333334,
      0.019716666666666667,   0.01975, 0.019783333333333333,
      0.019816666666666666,   0.01985, 0.019883333333333333,
      0.019916666666666666,   0.01995, 0.019983333333333332};
  b_struct_T options;
  emxArray_creal32_T *b_y1;
  double f_data[1536];
  int i;
  int iCol;
  int k;
  memcpy(&r.f1[0], &x[0], 921600U * sizeof(creal32_T));
  welchparse(&r, xw, options.range);
  options.range[0] = 't';
  options.range[1] = 'w';
  options.range[2] = 'o';
  options.range[3] = 's';
  options.range[4] = 'i';
  options.range[5] = 'd';
  options.range[6] = 'e';
  options.range[7] = 'd';
  for (iCol = 0; iCol < 600; iCol++) {
    i = 1536 * iCol;
    for (k = 0; k < 1536; k++) {
      xw[k + 1536 * iCol] = xw[k + i];
    }
  }
  emxInit_creal32_T(&b_y1, 2);
  computeDFT(xw, b_y1, f_data, &iCol);
  formatSpectrogram(b_y1, f_data, &iCol, options, varargout_1);
  compute_PSD(varargout_1, f_data, &iCol, options, varargout_4);
  centerOutputs(varargout_1, f_data, &iCol, varargout_4);
  *varargout_2_size = iCol;
  emxFree_creal32_T(&b_y1);
  for (i = 0; i < iCol; i++) {
    varargout_2_data[i] = (float)f_data[i];
  }
  for (i = 0; i < 600; i++) {
    varargout_3[i] = (float)dv[i];
  }
}

/*
 * File trailer for spectrogram.c
 *
 * [EOF]
 */

/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: eml_setop.c
 *
 * MATLAB Coder version            : 5.2
 * C/C++ source code generated on  : 20-Feb-2022 08:55:56
 */

/* Include Files */
#include "eml_setop.h"
#include "operation_data.h"
#include "rt_nonfinite.h"
#include "rt_nonfinite.h"
#include <math.h>
#include <string.h>

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
    if ((!rtIsInf(absx)) && (!rtIsNaN(absx))) {
      if (absx <= 2.2250738585072014E-308) {
        absx = 4.94065645841247E-324;
      } else {
        frexp(absx, &exponent);
        absx = ldexp(1.0, exponent - 53);
      }
    } else {
      absx = rtNaN;
    }
    if ((fabs(xk - x[*k]) < absx) ||
        (rtIsInf(x[*k]) && rtIsInf(xk) && ((x[*k] > 0.0) == (xk > 0.0)))) {
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
void b_do_vectors(double c_data[], int *c_size, int ia_data[], int *ia_size,
                  int *ib_size)
{
  double dv[1536];
  double ak;
  int a;
  int b_exponent;
  int b_ialast;
  int exponent;
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
    a = iv7[iblast - 1];
    exitg1 = false;
    while ((!exitg1) && (iblast < 1296)) {
      frexp((double)a / 2.0, &b_exponent);
      if (fabs((double)(a - iv7[iblast])) < ldexp(1.0, b_exponent - 53)) {
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
    for (a = 0; a < 1536; a++) {
      dv[a] = (double)a + 1.0;
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
  static const short x[912] = {
      1,    2,    3,    4,    5,    6,    7,    8,    9,    10,   11,   12,
      13,   14,   15,   16,   17,   18,   19,   20,   21,   22,   23,   24,
      25,   26,   27,   28,   29,   30,   31,   32,   33,   34,   35,   36,
      37,   38,   39,   40,   41,   42,   43,   44,   45,   46,   47,   48,
      49,   50,   51,   52,   53,   54,   55,   56,   57,   58,   59,   60,
      61,   62,   63,   64,   65,   66,   67,   68,   69,   70,   71,   72,
      73,   74,   75,   76,   77,   78,   79,   80,   81,   82,   83,   84,
      85,   86,   87,   88,   89,   90,   91,   92,   93,   94,   95,   96,
      97,   98,   99,   100,  101,  102,  103,  104,  105,  106,  107,  108,
      109,  110,  111,  112,  113,  114,  115,  116,  117,  118,  119,  120,
      121,  122,  123,  124,  125,  126,  127,  128,  129,  130,  131,  132,
      133,  134,  135,  136,  137,  138,  139,  140,  141,  142,  143,  144,
      145,  146,  147,  148,  149,  150,  151,  152,  153,  154,  155,  156,
      157,  158,  159,  160,  161,  162,  163,  164,  165,  166,  167,  168,
      169,  170,  171,  172,  173,  174,  175,  176,  177,  178,  179,  180,
      181,  182,  183,  184,  185,  186,  187,  188,  189,  190,  191,  192,
      193,  194,  195,  196,  197,  198,  199,  200,  201,  202,  203,  204,
      205,  206,  207,  208,  209,  210,  211,  212,  213,  214,  215,  216,
      217,  218,  219,  220,  221,  222,  223,  224,  225,  226,  227,  228,
      229,  230,  231,  232,  233,  234,  235,  236,  237,  238,  239,  240,
      241,  242,  243,  244,  245,  246,  247,  248,  249,  250,  251,  252,
      253,  254,  255,  256,  257,  258,  259,  260,  261,  262,  263,  264,
      265,  266,  267,  268,  269,  270,  271,  272,  273,  274,  275,  276,
      277,  278,  279,  280,  281,  282,  283,  284,  285,  286,  287,  288,
      289,  290,  291,  292,  293,  294,  295,  296,  297,  298,  299,  300,
      301,  302,  303,  304,  305,  306,  307,  308,  309,  310,  311,  312,
      313,  314,  315,  316,  317,  318,  319,  320,  321,  322,  323,  324,
      325,  326,  327,  328,  329,  330,  331,  332,  333,  334,  335,  336,
      337,  338,  339,  340,  341,  342,  343,  344,  345,  346,  347,  348,
      349,  350,  351,  352,  353,  354,  355,  356,  357,  358,  359,  360,
      361,  362,  363,  364,  365,  366,  367,  368,  369,  370,  371,  372,
      373,  374,  375,  376,  377,  378,  379,  380,  381,  382,  383,  384,
      385,  386,  387,  388,  389,  390,  391,  392,  393,  394,  395,  396,
      397,  398,  399,  400,  401,  402,  403,  404,  405,  406,  407,  408,
      409,  410,  411,  412,  413,  414,  415,  416,  417,  418,  419,  420,
      421,  422,  423,  424,  425,  426,  427,  428,  429,  430,  431,  432,
      433,  434,  435,  436,  437,  438,  439,  440,  441,  442,  443,  444,
      445,  446,  447,  448,  449,  450,  451,  452,  453,  454,  455,  456,
      1081, 1082, 1083, 1084, 1085, 1086, 1087, 1088, 1089, 1090, 1091, 1092,
      1093, 1094, 1095, 1096, 1097, 1098, 1099, 1100, 1101, 1102, 1103, 1104,
      1105, 1106, 1107, 1108, 1109, 1110, 1111, 1112, 1113, 1114, 1115, 1116,
      1117, 1118, 1119, 1120, 1121, 1122, 1123, 1124, 1125, 1126, 1127, 1128,
      1129, 1130, 1131, 1132, 1133, 1134, 1135, 1136, 1137, 1138, 1139, 1140,
      1141, 1142, 1143, 1144, 1145, 1146, 1147, 1148, 1149, 1150, 1151, 1152,
      1153, 1154, 1155, 1156, 1157, 1158, 1159, 1160, 1161, 1162, 1163, 1164,
      1165, 1166, 1167, 1168, 1169, 1170, 1171, 1172, 1173, 1174, 1175, 1176,
      1177, 1178, 1179, 1180, 1181, 1182, 1183, 1184, 1185, 1186, 1187, 1188,
      1189, 1190, 1191, 1192, 1193, 1194, 1195, 1196, 1197, 1198, 1199, 1200,
      1201, 1202, 1203, 1204, 1205, 1206, 1207, 1208, 1209, 1210, 1211, 1212,
      1213, 1214, 1215, 1216, 1217, 1218, 1219, 1220, 1221, 1222, 1223, 1224,
      1225, 1226, 1227, 1228, 1229, 1230, 1231, 1232, 1233, 1234, 1235, 1236,
      1237, 1238, 1239, 1240, 1241, 1242, 1243, 1244, 1245, 1246, 1247, 1248,
      1249, 1250, 1251, 1252, 1253, 1254, 1255, 1256, 1257, 1258, 1259, 1260,
      1261, 1262, 1263, 1264, 1265, 1266, 1267, 1268, 1269, 1270, 1271, 1272,
      1273, 1274, 1275, 1276, 1277, 1278, 1279, 1280, 1281, 1282, 1283, 1284,
      1285, 1286, 1287, 1288, 1289, 1290, 1291, 1292, 1293, 1294, 1295, 1296,
      1297, 1298, 1299, 1300, 1301, 1302, 1303, 1304, 1305, 1306, 1307, 1308,
      1309, 1310, 1311, 1312, 1313, 1314, 1315, 1316, 1317, 1318, 1319, 1320,
      1321, 1322, 1323, 1324, 1325, 1326, 1327, 1328, 1329, 1330, 1331, 1332,
      1333, 1334, 1335, 1336, 1337, 1338, 1339, 1340, 1341, 1342, 1343, 1344,
      1345, 1346, 1347, 1348, 1349, 1350, 1351, 1352, 1353, 1354, 1355, 1356,
      1357, 1358, 1359, 1360, 1361, 1362, 1363, 1364, 1365, 1366, 1367, 1368,
      1369, 1370, 1371, 1372, 1373, 1374, 1375, 1376, 1377, 1378, 1379, 1380,
      1381, 1382, 1383, 1384, 1385, 1386, 1387, 1388, 1389, 1390, 1391, 1392,
      1393, 1394, 1395, 1396, 1397, 1398, 1399, 1400, 1401, 1402, 1403, 1404,
      1405, 1406, 1407, 1408, 1409, 1410, 1411, 1412, 1413, 1414, 1415, 1416,
      1417, 1418, 1419, 1420, 1421, 1422, 1423, 1424, 1425, 1426, 1427, 1428,
      1429, 1430, 1431, 1432, 1433, 1434, 1435, 1436, 1437, 1438, 1439, 1440,
      1441, 1442, 1443, 1444, 1445, 1446, 1447, 1448, 1449, 1450, 1451, 1452,
      1453, 1454, 1455, 1456, 1457, 1458, 1459, 1460, 1461, 1462, 1463, 1464,
      1465, 1466, 1467, 1468, 1469, 1470, 1471, 1472, 1473, 1474, 1475, 1476,
      1477, 1478, 1479, 1480, 1481, 1482, 1483, 1484, 1485, 1486, 1487, 1488,
      1489, 1490, 1491, 1492, 1493, 1494, 1495, 1496, 1497, 1498, 1499, 1500,
      1501, 1502, 1503, 1504, 1505, 1506, 1507, 1508, 1509, 1510, 1511, 1512,
      1513, 1514, 1515, 1516, 1517, 1518, 1519, 1520, 1521, 1522, 1523, 1524,
      1525, 1526, 1527, 1528, 1529, 1530, 1531, 1532, 1533, 1534, 1535, 1536};
  double dv[1536];
  double ak;
  int a;
  int b_exponent;
  int b_ialast;
  int exponent;
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
  while ((ialast <= 1536) && (iblast <= 912)) {
    b_ialast = ialast;
    for (a = 0; a < 1536; a++) {
      dv[a] = (double)a + 1.0;
    }
    ak = skip_to_last_equal_value(&b_ialast, dv);
    ialast = b_ialast;
    a = x[iblast - 1];
    exitg1 = false;
    while ((!exitg1) && (iblast < 912)) {
      frexp((double)a / 2.0, &b_exponent);
      if (fabs((double)(a - x[iblast])) < ldexp(1.0, b_exponent - 53)) {
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
    for (a = 0; a < 1536; a++) {
      dv[a] = (double)a + 1.0;
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

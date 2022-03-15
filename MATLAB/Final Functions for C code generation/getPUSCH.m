%This function returns a 5G waveform with energy at the freaquency
%locations indicated by RBStart and LRB
function waveform = getPUSCH(RBStart,LRB)
coder.inline('never');
rxSampleRate= 46.08e6;
waveform = cast(complex(ones(1536,1)), 'single');

%OFDM Modulate parameters
prmStr.NumSymbols =1;
prmStr.NumTransmitAntennas=1;
prmStr.FFTLength=1536;
prmStr.CyclicPrefixLength=0;
prmStr.NullIndices = [1;2;3;4;5;6;7;8;9;10;11;12;13;14;15;16;17;18;19;20;21;22;23;24;25;26;27;28;29;30;31;32;33;34;35;36;37;38;39;40;41;42;43;44;45;46;47;48;49;50;51;52;53;54;55;56;57;58;59;60;61;62;63;64;65;66;67;68;69;70;71;72;73;74;75;76;77;78;79;80;81;82;83;84;85;86;87;88;89;90;91;92;93;94;95;96;97;98;99;100;101;102;103;104;105;106;107;108;109;110;111;112;113;114;115;116;117;118;119;120;121;122;123;124;125;126;127;128;129;130;131;132;1405;1406;1407;1408;1409;1410;1411;1412;1413;1414;1415;1416;1417;1418;1419;1420;1421;1422;1423;1424;1425;1426;1427;1428;1429;1430;1431;1432;1433;1434;1435;1436;1437;1438;1439;1440;1441;1442;1443;1444;1445;1446;1447;1448;1449;1450;1451;1452;1453;1454;1455;1456;1457;1458;1459;1460;1461;1462;1463;1464;1465;1466;1467;1468;1469;1470;1471;1472;1473;1474;1475;1476;1477;1478;1479;1480;1481;1482;1483;1484;1485;1486;1487;1488;1489;1490;1491;1492;1493;1494;1495;1496;1497;1498;1499;1500;1501;1502;1503;1504;1505;1506;1507;1508;1509;1510;1511;1512;1513;1514;1515;1516;1517;1518;1519;1520;1521;1522;1523;1524;1525;1526;1527;1528;1529;1530;1531;1532;1533;1534;1535;1536];
prmStr.hasPilots = 0;
prmStr.PilotIndices =[];
prmStr.Pilots= [];

%Create freaquency modulation grid 
grid = zeros(1536, 1);
grid(133+(RBStart*12):(LRB*12)+132+(RBStart*12), :) = ones((LRB*12), 1);

%optimized version of OFDMModulator().
waveform(1:1536) = cast(modulateOpt(grid,prmStr), 'single');
 
end

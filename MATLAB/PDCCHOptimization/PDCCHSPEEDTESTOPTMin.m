%#codgen
function decoded = PDCCHSPEEDTESTOPTMin(RNTI, in)
rxSampleRate = 46.08e6;
samplesPerSymbol = rxSampleRate*(0.001/2)/14;
rx = coder.load('Corrected_Waveform', 'rxWaveformn');
rxWaveform = rx.rxWaveformn;


info.NSubcarriers = 624;
info.Nfft = 1536;
info.SubcarrierSpacing = 30;
info.SampleRate = rxSampleRate;
%info.CyclicPrefixLengths = [132,108,108,108,108,108,108,108,108,108,108,108,108,108,132,108,108,108,108,108,108,108,108,108,108,108,108,108];
info.CyclicPrefixLengths = 132;
info.SymbolsPerSlot = 1;
info.SlotsPerSubframe = 2;
%info.SymbolPhases = [-65140.9236721844,-876441.518498480,-1687742.11332478,-2499042.70815107,-3310343.30297737,-4121643.89780367,-4932944.49262996,-5744245.08745626,-6555545.68228255,-7366846.27710885,-8178146.87193515,-8989447.46676144,-9800748.06158774,-10612048.6564140,-11435193.0555444,-12246493.6503707,-13057794.2451970,-13869094.8400233,-14680395.4348495,-15491696.0296758,-16302996.6245021,-17114297.2193284,-17925597.8141547,-18736898.4089810,-19548199.0038073,-20359499.5986336,-21170800.1934599,-21982100.7882862];
info.SymbolPhases = -65140.9236721844;
info.CyclicPrefixFraction = 0.5;
info.Resampling.L = 1;
info.Resampling.M = 1;
info.Resampling.TW = 1;
info.Resampling.AStop =1;

% Carrier configuration
carrier = nrCarrierConfig;
carrier.NCellID = 0;                    % Cell identity
carrier.SubcarrierSpacing = 30;         % Carrier/BWP Subcarrier spacing
carrier.CyclicPrefix = 'normal';        % Cyclic prefix
carrier.NSlot      = floor(in/14);  %37;           % Slot counter
carrier.NFrame     = 79;                % Frame counter
carrier.NStartGrid = 0;                 % Carrier offset
carrier.NSizeGrid  = 48;                % Size of carrier in RB

% CORESET configuration
coreset = nrCORESETConfig;
coreset.CORESETID = 0;                  % CORESET ID (0...11)
coreset.FrequencyResources = ones(1,8); % 6 RB sized
coreset.Duration = 1;                   % CORESET symbol duration (1,2,3)
coreset.CCEREGMapping = 'interleaved';  % CORESET Mapping
coreset.REGBundleSize = 6;              % L (2,6) or (3,6)
coreset.InterleaverSize = 2;            % R (2,3,6)
coreset.ShiftIndex = carrier.NCellID;   % default to NCellID

% Search space configuration
ss = nrSearchSpaceConfig;
ss.SearchSpaceID = 2;
ss.CORESETID = 0;                       % Associated CORESET ID (0...11)
ss.SearchSpaceType = 'ue';              % 'ue', 'common'
ss.StartSymbolWithinSlot = 0;           % Starting symbol in slot
ss.SlotPeriodAndOffset = [1 0];         % Search space period and offset
ss.Duration = 1;                        % Search space duration in slots
ss.NumCandidates = [0 0 1 0 0];         % For (1,2,4,8,16) levels respectively

% PDCCH configuration
pdcch = nrPDCCHConfig;
pdcch.NStartBWP  = 0;                  % BWP offset wrt CRB 0
pdcch.NSizeBWP   = 48;                  % Size of BWP in resource blocks
pdcch.CORESET = coreset;                % Associated CORESET
pdcch.SearchSpace = ss;                 % Associated SearchSpace
pdcch.RNTI = 0;                         % C-RNTI
pdcch.DMRSScramblingID = [];            % Use carrier.NCellID instead
pdcch.AggregationLevel = 4;             % Number of CCEs in PDCCH (1,2,4,8,16)
pdcch.AllocatedCandidate = 1;           % 1-based scalar




listLen = 8;                         % polar decoding list length
%nID = carrier.NCellID;

% Get all possible candidates


% Loop over all supported aggregation levels


decoded = false;
rxWaveN = rxWaveform(round((in-1)*samplesPerSymbol)+1:round(samplesPerSymbol*(in+13)));


[allInd,~,~] = nrPDCCHSpaceOpt(carrier,pdcch);
% Loop over all candidates at each aggregation level
alIdx = 3;
cIdx = 1;

% sum1 = 0;
% for x=1:100
% tic
% tmp = nrOFDMDemodulateOpt(rxWaveN,info,carrier.NSlot, 14, 1);
% %tmp =nrOFDMDemodulate(rxWaveN, 52, 30, 0,'SampleRate',rxSampleRate,'CarrierFrequency',3.6192e9);
% a =toc;
% sum1 = sum1+a;
% end
% sum1/100

tic
tmp = nrOFDMDemodulateOpt(rxWaveN(1:1800),info,carrier.NSlot, 1, 1);
%tmp = tmp(:,1);


[tmp, ~] = nrExtractResourcesOpt(allInd{alIdx}(:,cIdx),tmp);



tmp = nrPDCCHDecodeOpt(tmp,0,RNTI);



[~,errFlag] = nrDCIDecode(tmp,39,listLen, RNTI);
% Decode

toc
if sum(errFlag) == 0
    decoded = true;
end

end
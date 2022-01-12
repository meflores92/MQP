%#codgen
function PDCCHSPEEDTEST(RNTI, in)
rxSampleRate = 46.08e6;
samplesPerSymbol = rxSampleRate*(0.001/2)/14;
rx = coder.load('Corrected_Waveform', 'rxWaveformn');
rxWaveform = rx.rxWaveformn;


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
tic
% Get all possible candidates


% Loop over all supported aggregation levels


decoded = false;
rxWaveN = rxWaveform(round((in-1)*samplesPerSymbol)+1:round(samplesPerSymbol*(in+13))+10);

tic
[allInd,allDMRS,allDMRSInd] = nrPDCCHSpace(carrier,pdcch);
% Loop over all candidates at each aggregation level
alIdx = 3;
cIdx = 1;

% Get candidate
cSymIdx = allInd{alIdx}(:,cIdx);
cDMRS = allDMRS{alIdx}(:,cIdx);
cDMRSInd = allDMRSInd{alIdx}(:,cIdx);


% OFDM demodulate the carrier
rxCarrGrid = nrOFDMDemodulate(rxWaveN, 52, 30, 0,...
    'SampleRate',rxSampleRate,'CarrierFrequency',3.6192e9);

% Channel estimate
[hest,nVar] = nrChannelEstimate(carrier,rxCarrGrid,cDMRSInd,cDMRS);
[rxSym,pdcchHest] = nrExtractResources(cSymIdx,rxCarrGrid,hest);

% Equalization
[pdcchEq,csi] = nrEqualizeMMSE(rxSym,pdcchHest,nVar);



rxCW = nrPDCCHDecode(pdcchEq,0,RNTI,nVar);

[~,errFlag] = nrDCIDecode(rxCW,39,listLen, RNTI);
% Decode

if isequal(errFlag,0)
    decoded = true;
end

toc
end
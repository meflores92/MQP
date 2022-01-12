
%rng(211);           % Set RNG state for repeatability
EbNo = 100;
total = 0;
for x = 1:100
    [right, error] = decodeAccuracy(EbNo);
    total = error+ total;
end
average = total/100;
average

function [right, error] = decodeAccuracy(EbNo)
%Netowrk Parameters
nID = 23;           % pdcch-DMRS-ScramblingID
rnti = 100;         % C-RNTI for PDCCH in a UE-specific search space
K = 74;             % Number of DCI message bits
E = 288;            % Number of bits for PDCCH resources

%Generate random DCI packet and encode the packet
dciBits = randi([0 1],K,1,'int8');
dciCW = nrDCIEncode(dciBits,rnti,E);

%Map to resource grid
sym = nrPDCCH(dciCW,nID,rnti);

%EbNo = 13;                       % in dB
bps = 2;                        % bits per symbol, 2 for QPSK
EsNo = EbNo + 10*log10(bps);
snrdB = EsNo + 10*log10(K/E);

%Simullate over the air transmission
rxSym = awgn(sym,snrdB,'measured');
noiseVar = 10.^(-snrdB/10);     % assumes unit signal power
  
%Demodulate signal
recDCICW = nrSymbolDemodulate(rxSym,'QPSK',noiseVar);

%Descramble
listLen = 8;  
cSeq = nrPDCCHPRBS(nID,rnti,length(dciCW),'MappingType','binary', ...
        'OutputDataType',class(rxSym));
dciCW = cSeq.*recDCICW;

%Parameters for rate recovery
E = length(dciCW);
K = K+24;            % K includes CRC bits
nMax = 9;               % for downlink
N = nr5g.internal.polar.getN(K,E,nMax);
m = log2(N);

% Rate recovery, Section 7.3.4, [1]
recBlk = nrRateRecoverPolar(dciCW,K,N);


%Soft bit to hard bit conversion
data = [];
for x = 1:length(recBlk)
    if recBlk(x) > 0
        data(x) = 0;
    else
        data(x) = 1;
    end
end

%check bit locations 
right = 0;
error = 0;
for x = 1:2:64
    if xor(xor(data(x), data(x+128)),xor(data(x+64), data(x+64+128))) == xor(xor(data(x+1), data(x+128+1)),xor(data(x+64+1), data(x+64+128+1)))
        right = 1+right;
    else
        error = error +1;
    end 
end
end


%rng(211);           % Set RNG state for repeatability


%Constants
nID = 0;           % pdcch-DMRS-ScramblingID
rnti = 0;         % C-RNTI for PDCCH in a UE-specific search space
K = 140;             % Number of DCI message bits
E = 432;            % Number of bits for PDCCH resources
nMax = 9;               % for downlink
N = nr5g.internal.polar.getN(K+24,E,nMax);
dciBits = randi([0 1],K,1,'int8');

%Encode DCI
dciCW = nrDCIEncode(dciBits,rnti,E);
sym = nrPDCCH(dciCW,nID,rnti);


%Decode DCI
recDCICW = nrSymbolDemodulate(sym,'QPSK');

% Section 7.3.2.3 Descrambling
cSeq = nrPDCCHPRBS(nID,rnti,length(recDCICW),'MappingType','signed', ...
    'OutputDataType',class(sym));
dciCW = cSeq.*recDCICW;
%dciCW = recDCICW

recBlk = nrRateRecoverPolar(cast(dciCW, 'double'),K+24,N);

for x = 1:length(recBlk)
    if recBlk(x) >= 0
        data(x) = 0;
    else
        data(x) = 1;
    end
end


%Check output
right = 0;
error = 0;
% for x = 1:2:64
%     if xor(xor(data(x), data(x+128)),xor(data(x+64), data(x+64+128))) == xor(xor(data(x+1), data(x+128+1)),xor(data(x+64+1), data(x+64+128+1)))
%         right = 1+right;
%     else
%         error = error +1;
%     end
% end

for x = 81:112
    if xor(xor(data(x), data(x+256)), xor(data(x+128), data(x+128+256))) == 0
        right = 1+right;
    else
        error = error+1;
    end
end

right
error



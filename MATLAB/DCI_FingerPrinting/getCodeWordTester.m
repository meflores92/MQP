%rng(211);           % Set RNG state for repeatability

nID = 0;           % pdcch-DMRS-ScramblingID
rnti = 0x3c41;         % C-RNTI for PDCCH in a UE-specific search space
K = 39;             % Number of DCI message bits
E = 432;            % Number of bits for PDCCH resources

%table = load('RNTI_TABLE_2_16','RNTI_ID_MATRIX');
%RNTI_TABLE = RNTI_ID_MATRIX;

dciBits = randi([0 1],K,1,'int8');
dciCW = nrDCIEncode(dciBits,rnti,E);
sym = nrPDCCH(dciCW,nID,rnti);


cw = getCodeWord(sym, 39, nID, rnti)
% if cw == RNTI_TABLE(1 , 1)
%     sprintf('Yes')
% end

rxCW = nrPDCCHDecode(sym,nID,rnti);
listLen = 8;                    % polar decoding list length
[decDCIBits,mask] = nrDCIDecode(rxCW,K,listLen,rnti);

isequal(mask,0)
isequal(decDCIBits,dciBits)


function CodeWord = getCodeWord(sym, Kout, nID, rnti)

recDCICW = cast(nrSymbolDemodulate(sym,'QPSK', 'DecisionType', 'hard'), 'double');

E = length(recDCICW);
K = Kout+24;            % K includes CRC bits
nMax = 9;               % for downlink
N = nr5g.internal.polar.getN(K,E,nMax);

% Rate recovery, Section 7.3.4, [1]
data = nrRateRecoverPolar(recDCICW,K,N);



CodeWord = 0;
for x = 81:112
    if xor(xor(data(x), data(x+256)), xor(data(x+128), data(x+128+256))) == 0
        CodeWord = CodeWord + 2^(x-81);
    end
end
end

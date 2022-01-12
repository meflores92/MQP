function nrPolarDecodeme(in,K,E,L,varargin)
%nrPolarDecode Polar decode
%   DECBITS = nrPolarDecode(REC,K,E,L) decodes the rate-recovered input,
%   REC, for a (N,K) Polar code, using a CRC-aided successive-cancellation
%   list decoder, with the list length specified by L. The input REC is a
%   column vector of length N (a power of 2), representing the
%   log-likelihood ratios as soft inputs to the decoder. K is the number of
%   message bits, and E is the rate-matched output length. The output
%   DECBITS is a column vector of length K.
%
%   DECBITS = nrPolarDecode(...,PADCRC) specifies whether the input was
%   prepadded by ones prior to the CRC encoding with all-zeros register
%   state on the transmit end. PADCRC must be a boolean scalar where for a
%   true value, the input is assumed to be prepadded with ones, while a
%   false value indicates no prepadding was used. The default is false.
%
%   DECBITS = nrPolarDecode(REC,K,E,L,NMAX,IIL,CRCLEN) specifies the three
%   parameter set of: NMAX (an integer value of either 9 or 10), IIL (a
%   boolean scalar) and CRCLEN (an integer value one of 24, 11 or 6). The
%   allowed value sets of {9,true,24} and {10,false,11},{10,false,6} for
%   {NMAX,IIL,CRCLEN} apply for downlink and uplink configurations. When
%   the three parameters are not specified, the value set for the downlink
%   configuration is used. PADCRC is assumed false for this syntax.
%
%   % Example:
%   % Transmit polar-encoded block of data and decode using
%   % successive-cancellation list decoder.
%
%   K = 132;            % Message length
%   E = 256;            % Rate matched output length
%   nVar = 1.0;         % Noise variance
%   L = 8;              % Decoding list length
%
%   % Object construction
%   chan   = comm.AWGNChannel('NoiseMethod','Variance','Variance',nVar);
%
%   % Simulate a frame
%   msg    = randi([0 1],K,1,'int8');               % Generate random message
%   enc    = nrPolarEncode(msg,E);                  % Polar encode
%   mod    = nrSymbolModulate(enc,'QPSK');          % Modulate
%   rSig   = chan(mod);                             % Add WGN
%   rxLLR  = nrSymbolDemodulate(rSig,'QPSK',nVar);  % Soft demodulate
%   rxBits = nrPolarDecode(rxLLR,K,E,L);            % Polar decode
%
%   % Get bit errors
%   numBitErrs = biterr(rxBits, msg);
%   disp(['Number of bit errors: ' num2str(numBitErrs)]);
%
%   See also nrPolarEncode, nrRateRecoverPolar, nrCRCDecode, nrDCIDecode,
%   nrBCHDecode, nrUCIDecode.

%   Copyright 2018 The MathWorks, Inc.

%#codegen

    narginchk(4,7);

    if nargin==4
        % Downlink parameters
        % nrPolarDecode(in,K,E,L)
        nMax = 9;
        iIL = true;
        crcLen = 24;

        padCRC = false;           % default, for BCH and UCI

    elseif nargin == 5
        % Downlink parameters for which padCRC applies
        % nrPolarDecode(in,K,E,L,padCRC)
        nMax = 9;
        iIL = true;
        crcLen = 24;

        padCRC = varargin{1};     % differs for BCH and DCI

    elseif nargin == 6
        coder.internal.errorIf(1,'nr5g:nrPolar:InvalidNumInputsDec');

    elseif nargin == 7
        % nrPolarDecode(in,K,E,L,nMax,iIL,crcLen)
        nMax = varargin{1};
        iIL = varargin{2};
        crcLen = varargin{3};

        padCRC = false;           % default, for BCH and UCI
    end

    % Input is a single code block and assumes CRC bits are included in K
    % output
    validateInputs(in,K,E,L,nMax,iIL,crcLen,padCRC);

    % F accounts for nPC bits, if present
    [F,qPC] = nr5g.internal.polar.construct(K,E,nMax);

    % CA-SCL decode
    lclPolarDecode(in,F,L,iIL,crcLen,padCRC,qPC);

%     % Remove nPC bits from output, if present
%     if ~isempty(qPC)
%         % Extract the information only bits
%         qI = find(F==0)-1;
%         k = 1;
%         out = zeros(length(outkpc)-3,1);
%         for idx = 1:length(qI)
%             if ~any(qI(idx)==qPC)
%                 out(k) = outkpc(idx);
%                 k = k+1;
%             end
%         end
%     else
%         out = outkpc;
%     end
end

function validateInputs(in,K,E,L,nMax,iIL,crcLen,padCRC)
% Check inputs

    fcnName = 'nrPolarDecode';

    % Validate rate-recovered input for a single code-block
    validateattributes(in,{'single','double'},{'real','column'}, ...
        fcnName,'REC');
    N = length(in);
    coder.internal.errorIf( floor(log2(N))~=log2(N), ...
        'nr5g:nrPolar:InvalidInputDecLength');

    % Validate the number of message bits which must be less than N
    validateattributes(K,{'numeric'}, ...
        {'real','scalar','integer','nonempty','finite','<',N}, ...
        fcnName,'K');

    % Validate base-2 logarithm of rate-recovered input's maximum length
    % (9 or 10)
    validateattributes(nMax,{'numeric'},{'scalar','integer'}, ...
        fcnName,'NMAX');
    coder.internal.errorIf( ~any(nMax == [9 10]),'nr5g:nrPolar:InvalidnMax');

    % Validate output deinterleaving flag
    validateattributes(iIL,{'logical'},{'scalar'},fcnName,'IIL');

    % Validate the number of appended CRC bits (6, 11 or 24)
    validateattributes(crcLen,{'numeric'},{'scalar','integer','nonempty', ...
        'finite'},fcnName,'CRCLEN');
    coder.internal.errorIf( ~any(crcLen == [6 11 24]), ...
        'nr5g:nrPolar:InvalidCRCLen');

    % Validate CRC prepadding flag
    validateattributes(padCRC,{'logical'},{'scalar'},fcnName,'PADCRC');

    % A restriction for downlink (for up to 12 bits padding)
    % length K must be greater than or equal to 36 and less than or equal
    % to 164
    coder.internal.errorIf( nMax==9 && iIL && crcLen==24 && ...
        (K < 36 || K > 164), 'nr5g:nrPolar:InvalidKDL');

    % A restriction for uplink (for CA-Polar)
    % length K must be greater than 30 and less than or equal to 1023
    coder.internal.errorIf( nMax==10 && ~iIL && crcLen==11 && ~padCRC && ...
        (K <= 30 || K > 1023), 'nr5g:nrPolar:InvalidKUL');

    % A restriction for uplink (for PC-Polar)
    % length K must be greater than 17 and less than or equal to 25
    coder.internal.errorIf( nMax==10 && ~iIL && crcLen==6 && ~padCRC && ...
        (K < 18 || K > 25), 'nr5g:nrPolar:InvalidKULPC');

    % Validate rate-matched output length which must be less than or equal
    % to 8192 and greater than K
    validateattributes(E,{'numeric'}, ...
        {'real','scalar','integer','nonempty','finite','>',K,'<=',8192}, ...
        fcnName,'E');

    % Validate decoding list length
    nr5g.internal.validateParameters('ListLength',L,fcnName);

end

function lclPolarDecode(in,F,L,iIL,crcLen,padCRC,qPC)

    % References:
    % [1] Tal, I, and Vardy, A., "List decoding of Polar Codes", IEEE
    % Transactions on Information Theory, vol. 61, No. 5, pp. 2213-2226,
    % May 2015.
    % [2] Stimming, A. B., Parizi, M. B., and Burg, A., "LLR-Based
    % Successive Cancellation List Decoding of Polar Codes", IEEE
    % Transaction on Signal Processing, vol. 63, No. 19, pp.5165-5179,
    % 2015.

    % Setup
    N = length(F);
    m = log2(N);
    K = sum(F==0);      % includes nPC bits as well

    % CRCs as per TS 38.212, Section 5.1
    if crcLen == 24         % '24C', downlink
        polyStr = '24C';
    elseif crcLen == 11     % '11', uplink
        polyStr = '11';
    else % crcLen == 6      % '6', uplink
        polyStr = '6';
    end

    br = zeros(N,1);
    for idxBit = 0:N-1
        % 0-based indexing
        br(idxBit+1) = polarBitReverse(idxBit,m);
    end

    if iIL
        piInterl = nr5g.internal.polar.interleaveMap(K);
    else
        piInterl = (0:K-1).';
    end

    % Initialize core
    [sttStr, arrayPtrLLR, arrayPtrC] = initializeDataStructures(N,L,m);
    [iniPathIdx, sttStr] = assignInitialPath(sttStr);
    [sp, sttStr, arrayPtrLLR, arrayPtrC] = getArrayPtrP(sttStr, ...
        arrayPtrLLR, arrayPtrC, 1, iniPathIdx);
    arrayPtrLLR{1,sp}(:,1) = in(br+1);  % LLRs
    mplus1 = m+1;

    % Main loop
    for phase = 1:N
        [sttStr, arrayPtrLLR, arrayPtrC] = recursivelyCalcP(sttStr, ...
            arrayPtrLLR, arrayPtrC, mplus1, phase);

        pm2 = mod(phase-1,2);
        if F(phase)==1
            % Path for frozen (and punctured) bits
            for pathIdx = 1:L
                if ~sttStr.activePath(pathIdx)
                    continue;
                end
                [sc, sttStr, arrayPtrLLR, arrayPtrC] = getArrayPtrC(sttStr, ...
                    arrayPtrLLR, arrayPtrC, mplus1, pathIdx);
                arrayPtrC{mplus1,sc}(1,pm2+1) = 0; % set to 0

                sttStr.llrPathMetric(pathIdx) = ...
                    sttStr.llrPathMetric(pathIdx) + ...
                    log(1 + exp(-arrayPtrLLR{mplus1,sc}(1,1)));
            end
        else % Path for info bits
            [sttStr, arrayPtrLLR, arrayPtrC] = contPathsUnfrozenBit(sttStr, ...
                arrayPtrLLR, arrayPtrC, phase);
        end

        if pm2==1
            [sttStr, arrayPtrLLR, arrayPtrC] = recursivelyUpdateC(sttStr, ...
                arrayPtrLLR, arrayPtrC, mplus1, phase);
        end
    end

    % Return the best codeword in the list. Use CRC checks, if enabled
    pathIdx1 = 1;
    p1 = realmax;
    crcCW = false;
    for pathIdx = 1:L
%         if ~sttStr.activePath(pathIdx)
%             continue;
%         end

        [sc, sttStr, arrayPtrLLR, arrayPtrC] = getArrayPtrC(sttStr, ...
            arrayPtrLLR, arrayPtrC, mplus1, pathIdx);
        if crcLen>0
            canCW = sttStr.savedCWs(:,sc);  % N, with frozen bits
            canMsg = canCW(F==0,1);         % K bits only (with nPC)
            
            canMsg(piInterl+1) = canMsg;    % deinterleave (for k+nPC)

            if ~isempty(qPC)
                % Extract the info only bits, minus the PC ones
                qI = find(F==0)-1;
                k = 1;
                out = zeros(length(canMsg)-length(qPC),1);
                for idx = 1:length(qI)
                    if ~any(qI(idx)==qPC)
                        out(k) = canMsg(idx);
                        k = k+1;
                    end
                end
            else
                out = canMsg;
            end

            % Check CRC: errFlag is 1 for error, 0 for no errors
            if padCRC  % prepad with ones
                padCRCMsg = [ones(crcLen,1); out];
            else
                padCRCMsg = out;
            end
            [~, errFlag] = nrCRCDecode(padCRCMsg,polyStr);
            if errFlag      % ~0 => fail
                continue;   % move to next path
            end
        end
        crcCW = true;
        if p1 > sttStr.llrPathMetric(pathIdx)
            p1 = sttStr.llrPathMetric(pathIdx);
            pathIdx1 = pathIdx;
        end
    end

%     if ~crcCW   % no codeword found which passes crcCheck
%         pathIdx1 = 1;
%         p1 = realmax;
%         for pathIdx = 1:L
%             if ~sttStr.activePath(pathIdx)
%                 continue;
%             end
% 
%             if p1 > sttStr.llrPathMetric(pathIdx)
%                 p1 = sttStr.llrPathMetric(pathIdx);
%                 pathIdx1 = pathIdx;
%             end
%         end
%     end
% 
%     % Get decoded bits
%     [sc, sttStr] = getArrayPtrC(sttStr,arrayPtrLLR,arrayPtrC,mplus1, ...
%                                 pathIdx1);
%     decCW = sttStr.savedCWs(:,sc);      % N, with frozen bits
%     dec = decCW(F==0,1);                % K, info + nPC bits only
%     dec(piInterl+1) = dec;              % Deinterleave output, K+nPC

end

function [sttStr, arrayPtrLLR, arrayPtrC] = contPathsUnfrozenBit(sttStr, ...
    arrayPtrLLR, arrayPtrC, phase)
    % Input:
    %   phase: phase phi, 1-based, 1:2^m, or 1:N

    % Populate probForks
    probForks = -realmax*ones(sttStr.L,2);
    i = 0;
    mplus1 = sttStr.m+1;
    for pathIdx = 1:sttStr.L
        if sttStr.activePath(pathIdx)
            [sp, sttStr, arrayPtrLLR, arrayPtrC] = getArrayPtrP(sttStr, ...
                arrayPtrLLR, arrayPtrC, mplus1, pathIdx);
            probForks(pathIdx,1) = - (sttStr.llrPathMetric(pathIdx) ...
                + log(1+exp(-(arrayPtrLLR{mplus1,sp}(1,1)))));
            probForks(pathIdx,2) = - (sttStr.llrPathMetric(pathIdx) ...
                + log(1+exp((arrayPtrLLR{mplus1,sp}(1,1)))));
            i = i+1;
        end
    end

    rho = min(2*i,sttStr.L);
    contForks = zeros(sttStr.L,2);
    % Populate contForks such that contForks(l,b) is true iff
    % probForks(l,b) is one of rho largest entries in probForks.
    prob = sort(probForks(:), 'descend');
    if rho>0
        threshold = prob(rho);
    else
        threshold = prob(1); % Largest
    end
    numPop = 0;
    for pathIdx = 1:sttStr.L
        for bIdx = 1:2
            if numPop==rho
                break;
            end
            if probForks(pathIdx,bIdx)>threshold
                contForks(pathIdx,bIdx) = 1;
                numPop = numPop+1;
            end
        end
    end

    if numPop<rho
        for pathIdx = 1:sttStr.L
            for bIdx = 1:2
                if numPop==rho
                    break;
                end
                if probForks(pathIdx,bIdx)==threshold
                    contForks(pathIdx,bIdx) = 1;
                    numPop = numPop+1;
                end
            end
        end
    end

    % First, kill-off non-continuing paths
    for pathIdx = 1:sttStr.L
        if ~sttStr.activePath(pathIdx)
            continue;
        end
        if contForks(pathIdx,1)==0 && contForks(pathIdx,2)==0
            sttStr = killPath(sttStr, pathIdx);
        end
    end

    % Continue relevant paths, duplicating if necessary
    pm2 = mod(phase-1,2);
    for pathIdx = 1:sttStr.L
        if contForks(pathIdx,1)==0 && contForks(pathIdx,2)==0
            % Both forks are bad
            continue;
        end

        [sc, sttStr, arrayPtrLLR, arrayPtrC] = getArrayPtrC(sttStr, ...
            arrayPtrLLR, arrayPtrC, mplus1, pathIdx);
        if contForks(pathIdx,1)==1 && contForks(pathIdx,2)==1
            % Both forks are good
            arrayPtrC{mplus1,sc}(1,pm2+1) = 0;
            sttStr.savedCWs(phase,sc) = 0;

            [pathIdx1, sttStr] = clonePath(sttStr, pathIdx);
            [sc2, sttStr, arrayPtrLLR, arrayPtrC] = getArrayPtrC(sttStr, ...
                arrayPtrLLR, arrayPtrC, mplus1, pathIdx1);
            sttStr.savedCWs(1:phase-1,sc2) = sttStr.savedCWs(1:phase-1,sc);

            arrayPtrC{mplus1,sc2}(1,pm2+1) = 1;
            sttStr.savedCWs(phase,sc2) = 1;
            sttStr.llrPathMetric(pathIdx) = sttStr.llrPathMetric(pathIdx) ...
                + log(1 + exp(-arrayPtrLLR{mplus1,sc}(1,1)));
            sttStr.llrPathMetric(pathIdx1) = sttStr.llrPathMetric(pathIdx1) ...
                + log(1 + exp(arrayPtrLLR{mplus1,sc2}(1,1)));
        else
            % Exactly one fork is good
            if contForks(pathIdx,1)==1
                arrayPtrC{mplus1,sc}(1,pm2+1) = 0;
                sttStr.savedCWs(phase,sc) = 0;
                sttStr.llrPathMetric(pathIdx) = sttStr.llrPathMetric(pathIdx) ...
                    + log(1 + exp(-arrayPtrLLR{mplus1,sc}(1,1)));
            else
                arrayPtrC{mplus1,sc}(1,pm2+1) = 1;
                sttStr.savedCWs(phase,sc) = 1;
                sttStr.llrPathMetric(pathIdx) = sttStr.llrPathMetric(pathIdx) ...
                    + log(1 + exp(arrayPtrLLR{mplus1,sc}(1,1)));
            end
        end
    end

end

% Mid-level fcns
function [sttStr, arrayPtrLLR, arrayPtrC] = recursivelyCalcP(sttStr, ...
    arrayPtrLLR, arrayPtrC, layer, phase)
    % Input:
    %   layer: layer lambda, 1-based, 1:m+1
    %   phase: phase phi, 1-based, 1:2^layer or 1:N

    if layer==1
        return;
    end
    psi = floor((phase-1)/2)+1;
    pm2 = mod(phase-1,2);
    if pm2==0
        [sttStr, arrayPtrLLR, arrayPtrC] = recursivelyCalcP(sttStr, ...
            arrayPtrLLR, arrayPtrC, layer-1, psi);
    end

    expm = 2^(sttStr.m-layer+1);
    for pathIdx = 1:sttStr.L
        if ~sttStr.activePath(pathIdx)
            continue;
        end

        [sp, sttStr, arrayPtrLLR, arrayPtrC] = getArrayPtrP(sttStr, ...
            arrayPtrLLR, arrayPtrC, layer, pathIdx);
        [spminus1, sttStr, arrayPtrLLR, arrayPtrC] = getArrayPtrP( ...
            sttStr, arrayPtrLLR, arrayPtrC, layer-1, pathIdx);
        [sc, sttStr, arrayPtrLLR, arrayPtrC] = getArrayPtrC(sttStr, ...
            arrayPtrLLR, arrayPtrC, layer, pathIdx);
        for beta = 0:expm-1
            if pm2==0
                % LLR
                aa = arrayPtrLLR{layer-1,spminus1}( 2*beta+1,1 );
                bb = arrayPtrLLR{layer-1,spminus1}( 2*beta+2,1 );
                % Consider a table look-up instead
                if max( abs(aa), abs(bb)) < 40
                    % Equation 8a, Stimming
                    arrayPtrLLR{layer,sp}(beta+1,1) = ...
                        log( (exp(aa+bb)+1)/(exp(aa) + exp(bb)) );
                else
                    % Equation 9, Stimming
                    arrayPtrLLR{layer,sp}(beta+1,1) = ...
                        sign(aa)*sign(bb)*min(abs(aa), abs(bb));
                end
            else
                u1 = arrayPtrC{layer,sc}(beta+1,1);
                % Equation 8b, Stimming
                arrayPtrLLR{layer,sp}(beta+1,1) = (-1)^u1 * ...
                    arrayPtrLLR{layer-1,spminus1}( 2*beta+1,1 ) + ...
                    arrayPtrLLR{layer-1,spminus1}( 2*beta+2,1 );
            end
        end
    end

end

function [sttStr, arrayPtrLLR, arrayPtrC] = recursivelyUpdateC(sttStr, ...
    arrayPtrLLR, arrayPtrC, layer, phase)
    % Input:
    %   layer: layer lambda, 1-based, 1:m+1
    %   phase: phase phi, 1-based, 1:2^layer or 1:N, must be odd

    psi = floor((phase-1)/2);
    pm2 = mod(psi,2);
    expm = 2^(sttStr.m-layer+1);
    for pathIdx = 1:sttStr.L
        if ~sttStr.activePath(pathIdx)
            continue;
        end
        [sc, sttStr, arrayPtrLLR, arrayPtrC] = getArrayPtrC(sttStr, ...
            arrayPtrLLR, arrayPtrC, layer, pathIdx);
        [scminus1, sttStr, arrayPtrLLR, arrayPtrC] = getArrayPtrC( ...
            sttStr, arrayPtrLLR, arrayPtrC, layer-1, pathIdx);
        for beta = 0:expm-1
            arrayPtrC{layer-1,scminus1}(2*beta+1,pm2+1) = ...
                xor(arrayPtrC{layer,sc}(beta+1,1), ...
                arrayPtrC{layer,sc}(beta+1,2));
            arrayPtrC{layer-1,scminus1}(2*beta+2,pm2+1) = ...
                arrayPtrC{layer,sc}(beta+1,2);
        end
    end

    if pm2==1
        [sttStr, arrayPtrLLR, arrayPtrC] = recursivelyUpdateC(sttStr, ...
            arrayPtrLLR, arrayPtrC, layer-1, psi+1);
    end

end

% Low-level fcns
function [sttStr, arrayPtrLLR, arrayPtrC] = initializeDataStructures(N,L,m)
    % Indices are all 1-based MATLAB indices.

    sttStr.L = L;
    sttStr.m = m;                   % log2(N)

    maxMplus1 = 11;                 % for N=1024
    % Limited for now to the input L value only. Can be parameterized to be
    % the maximum value of L, similar to maxMplus1.
    coder.varsize('parrayPtrLLR',[maxMplus1 L], [1 0]);
    coder.varsize('parrayPtrC',[maxMplus1 L], [1 0]);

    mplus1 = m+1;
    parrayPtrLLR = cell(mplus1,L);  % store arrays
    parrayPtrC = cell(mplus1,L);    % store arrays
    coder.unroll(false);            % Allows maxMplus1, mplus1 to differ
    for layer = 1:mplus1
        expm = 2^(mplus1-layer);
        coder.unroll(false);
        for s = 1:L
            parrayPtrLLR{layer,s} = zeros(expm,1);
            parrayPtrC{layer,s} = zeros(expm,2); % binary-valued: 0,1
        end
    end
    % An extra layer of in-direction is needed for codegen
    arrayPtrLLR = parrayPtrLLR;           % (m+1)-by-L
    arrayPtrC = parrayPtrC;               % (m+1)-by-L

    sttStr.llrPathMetric = zeros(L,1);

    sttStr.pathIdxToArrayIdx = ones(mplus1,L);   % (m+1)-by-L

    sttStr.inactiveArrayIndices = zeros(mplus1,L);
    sttStr.inactiveArrayIndicesLen = zeros(mplus1,1);
    for layer = 1:mplus1
        sttStr.inactiveArrayIndices(layer,:) = 1:L;
        sttStr.inactiveArrayIndicesLen(layer,1) = L;
    end
    sttStr.arrayReferenceCount = zeros(mplus1,L);

    sttStr.inactivePathIndices = (1:L).';     % all paths are inactive
    sttStr.inactivePathIndicesLen = L;        % 1-by-1, stack depth
    sttStr.activePath = zeros(L,1,'logical'); % no paths are active

    sttStr.savedCWs = zeros(N,L);             % saved codewords

end

function [pathIdx, sttStr] = assignInitialPath(sttStr)
    % Output:
    %   pathIdx: initial path index l, 1-based, 1:L

    pathIdx = sttStr.inactivePathIndices(sttStr.inactivePathIndicesLen,1);
    sttStr.inactivePathIndicesLen = sttStr.inactivePathIndicesLen-1;
    sttStr.activePath(pathIdx) = true;

    % Associate arrays with path index
    for layer = 1:sttStr.m+1
        s = sttStr.inactiveArrayIndices(layer, ...
            sttStr.inactiveArrayIndicesLen(layer));
        sttStr.inactiveArrayIndicesLen(layer) = ...
            sttStr.inactiveArrayIndicesLen(layer)-1;

        sttStr.pathIdxToArrayIdx(layer,pathIdx) = s;
        sttStr.arrayReferenceCount(layer,pathIdx) = 1;
    end

end

function [clPathIdx, sttStr] = clonePath(sttStr, pathIdx)
    % Input:
    %   pathIdx: path index to clone, l, 1-based, 1:L
    % Output:
    %   clPathIdx: cloned path index, l', 1-based

    clPathIdx = sttStr.inactivePathIndices(sttStr.inactivePathIndicesLen,1);
    sttStr.inactivePathIndicesLen = sttStr.inactivePathIndicesLen-1;
    sttStr.activePath(clPathIdx) = true;
    sttStr.llrPathMetric(clPathIdx) = sttStr.llrPathMetric(pathIdx);

    % Make clPathIdx (l') reference same arrays as pathIdx (l)
    for layer = 1:sttStr.m+1
        s = sttStr.pathIdxToArrayIdx(layer,pathIdx);

        sttStr.pathIdxToArrayIdx(layer,clPathIdx) = s;
        sttStr.arrayReferenceCount(layer,s) = ...
            sttStr.arrayReferenceCount(layer,s)+1;
    end

end

function sttStr = killPath(sttStr, pathIdx)
    % Input:
    %   pathIdx: path index to kill, l, 1-based, 1:L

    % Mark path pathIdx as inactive
    sttStr.activePath(pathIdx) = false;
    sttStr.inactivePathIndicesLen = sttStr.inactivePathIndicesLen+1;
    sttStr.inactivePathIndices(sttStr.inactivePathIndicesLen,1) = pathIdx;
    sttStr.llrPathMetric(pathIdx) = 0;

    % Disassociate arrays with path Idx
    for layer = 1:sttStr.m+1
        s = sttStr.pathIdxToArrayIdx(layer,pathIdx);
        sttStr.arrayReferenceCount(layer,s) = ...
            sttStr.arrayReferenceCount(layer,s)-1;

        if sttStr.arrayReferenceCount(layer,s)==0
            if sttStr.inactiveArrayIndicesLen(layer,1) < sttStr.L
                sttStr.inactiveArrayIndicesLen(layer,1) = ...
                    sttStr.inactiveArrayIndicesLen(layer,1)+1;
            end
            sttStr.inactiveArrayIndices(layer, ...
                sttStr.inactiveArrayIndicesLen(layer,1)) = s;
        end
    end

end

function [s2, sttStr, arrayPtrLLR, arrayPtrC] = getArrayPtrP(sttStr, ...
    arrayPtrLLR, arrayPtrC, layer, pathIdx)
    % Input:
    %   layer:   layer lambda, 1-based, 1:m+1
    %   pathIdx: path index l, 1-based, 1:L
    % Output:
    %   s2: corresponding pathIdx for same layer

    s = sttStr.pathIdxToArrayIdx(layer,pathIdx);
    if sttStr.arrayReferenceCount(layer,s)==1
        s2 = s;
    else
        s2 = sttStr.inactiveArrayIndices(layer, ...
            sttStr.inactiveArrayIndicesLen(layer,1));
        if sttStr.inactiveArrayIndicesLen(layer,1) > 1
            sttStr.inactiveArrayIndicesLen(layer,1) = ...
                sttStr.inactiveArrayIndicesLen(layer,1)-1;
        end

        % deep copy
        arrayPtrLLR{layer,s2} = arrayPtrLLR{layer,s};
        arrayPtrC{layer,s2} = arrayPtrC{layer,s};

        sttStr.arrayReferenceCount(layer,s) = ...
            sttStr.arrayReferenceCount(layer,s)-1;
        sttStr.arrayReferenceCount(layer,s2) = 1;
        sttStr.pathIdxToArrayIdx(layer,pathIdx) = s2;
    end

end

function [s2, sttStr, arrayPtrLLR, arrayPtrC] = getArrayPtrC(sttStr, ...
    arrayPtrLLR, arrayPtrC, layer, pathIdx)
    % Input:
    %   layer:   layer lambda, 1-based, 1:m+1
    %   pathIdx: path index l, 1-based, 1:L
    % Output:
    %   ptrC: corresponding pathIdx for same layer

    s = sttStr.pathIdxToArrayIdx(layer,pathIdx);
    if sttStr.arrayReferenceCount(layer,s)==1
        s2 = s;
    else
        s2 = sttStr.inactiveArrayIndices(layer, ...
            sttStr.inactiveArrayIndicesLen(layer,1));
        if sttStr.inactiveArrayIndicesLen(layer,1) > 1
            sttStr.inactiveArrayIndicesLen(layer,1) = ...
                sttStr.inactiveArrayIndicesLen(layer,1)-1;
        end

        % deep copy
        arrayPtrC{layer,s2} = arrayPtrC{layer,s};
        arrayPtrLLR{layer,s2} = arrayPtrLLR{layer,s};

        sttStr.arrayReferenceCount(layer,s) = ...
            sttStr.arrayReferenceCount(layer,s)-1;
        sttStr.arrayReferenceCount(layer,s2) = 1;
        sttStr.pathIdxToArrayIdx(layer,pathIdx) = s2;
    end

end

function br = polarBitReverse(b,n)
%polarBitReverse Bit-wise reverse input value
%
%   BR = polarBitReverse(B,N) returns the bit-wise reversed-value of B,
%   each represented over N bits.
%
%   % Example:
%   br = polarBitReverse(21,7)
%   %  returns br=84

    % No checking: b, n are scalars, 0<=b<=2^n-1
    br = comm.internal.utilities.bi2deRightMSB( ... % for fliplr
         comm.internal.utilities.de2biBase2LeftMSB(b,n),2);

end

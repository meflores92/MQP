function yout = modulateOpt(gridIn,prmStr)
coder.inline('never');
% COMM.INTERNAL.OFDM.MODULATE OFDM modulation
%
%   Note: This is an internal undocumented function and its API and/or
%   functionality may change in subsequent releases.
%
%   Y = comm.internal.ofdm.modulate(GRIDIN,PRMSTR) performs OFDM modulation
%   on the input GRIDIN, using the parameters specified in the structure
%   PRMSTR, and returns the result in Y. GRIDIN is the fully populated
%   3D input accounting for all data, null and pilot subcarriers.
%
%   PRMSTR must have the following fields:
%       FFTLength
%       CyclicPrefixLength
%       NumSymbols
%       NumTransmitAntennas
%
%   See also comm.internal.ofdm.demodulate, ofdmdemod, ofdmmod.

%   Copyright 2017-2018 The MathWorks, Inc.

%#codegen

    fftLen = prmStr.FFTLength;
    cpLen  = prmStr.CyclicPrefixLength;
    numSym = prmStr.NumSymbols;
    numTx  = prmStr.NumTransmitAntennas;
    typeIn = class(gridIn);
        
    % IFFT shift    
    if isreal(gridIn)
        postShift = complex(ifftshift(gridIn,1),0);
    else
        postShift = ifftshift(gridIn,1);
    end
    
    % IFFT
    %GET FFT
    postIFFT = ifft(postShift,[],1); 
    %DONE FFT
    
    
    % Append cyclic prefix
    if isscalar(cpLen) % same length

        postCP = postIFFT([end-cpLen(1)+(1:cpLen(1)),1:end],:,:);
        yout = reshape(postCP,[(fftLen+cpLen(1))*numSym numTx]);

    else % different lengths per symbol
        
        yout = coder.nullcopy(complex(zeros(fftLen*numSym+sum(cpLen),...
            numTx,typeIn), zeros(fftLen*numSym+sum(cpLen),numTx,typeIn)));
        for symIdx = 1:numSym
            % Use reshape instead of squeeze in case of CP length ==1
            yout(fftLen*(symIdx-1)+sum(cpLen(1:symIdx-1)) + ...
                (1:fftLen+cpLen(symIdx)),:) = ...
                [reshape(postIFFT(end-cpLen(symIdx)+(1:cpLen(symIdx)), ...
                symIdx,:), [cpLen(symIdx) numTx]); ...
                squeeze(postIFFT(:,symIdx,:))];
        end
        
    end
    
end

% [EOF]

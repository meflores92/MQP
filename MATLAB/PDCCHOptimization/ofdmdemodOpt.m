function y = ofdmdemodOpt(x,nfft,cplen,SymbolOffset,NullIndices)
%OFDMDEMOD OFDM demodulate the time-domain input received signal
%
%   Y = OFDMDEMOD(X,NFFT,CPLEN) performs OFDM demodulation on the input X
%   and outputs the result in Y.  Specify X as an Ns-by-Nr matrix of real
%   or complex values. Ns is the number of samples and Nr is the number of
%   receive antennas.  NFFT is the FFT size, specified as a scalar. CPLEN
%   is the cyclic prefix length, specified as a scalar or row vector of
%   integers in the range 0 and NFFT, both inclusive. When CPLEN is
%   specified as a vector, its length equals the number of OFDM symbols,
%   Nsym, in the input X.
%       Y is the complex OFDM demodulated output signal of size
%   NFFT-by-Nsym-by-Nr, where Nsym = Ns/(NFFT+CPLEN) if CPLEN is a scalar, 
%   or Nsym = (Ns-sum(CPLEN))/(NFFT) = length(CPLEN) if CPLEN is a vector.
%
%   Y = OFDMDEMOD(X,NFFT,CPLEN,SYMOFFSET) accepts a scalar or row vector
%   indicating the sampling offset, SYMOFFSET, for every OFDM symbol, with
%   a value between 0 and CPLEN, both inclusive. The OFDM demodulation is
%   performed based on NFFT samples following the offset position. When
%   specified as a scalar, the same offset is used for all symbols. When
%   not specified, SYMOFFSET defaults to an offset equal to the cyclic
%   prefix length.
%
%   Y = OFDMDEMOD(X,NFFT,CPLEN,SYMOFFSET,NULLIDX) also accepts a column
%   vector of FFT indices, NULLIDX, indicating the null carrier locations
%   from 1 to NFFT. For this syntax, the number of rows in the output Y is
%   NFFT-length(NULLIDX). Nulls are removed at the locations indicated by
%   NULLIDX. NULLIDX can be used to account for both guard bands and DC
%   subcarriers. The DC subcarrier is the center of the frequency band and
%   has an index value of (NFFT/2+1) if NFFT is even or (NFFT+1)/2 if NFFT
%   is odd.
%
%   [Y,PILOTS] = OFDMDEMOD(X,NFFT,CPLEN,SYMOFFSET,NULLIDX,PILOTIDX) also
%   accepts a column vector of FFT indices, PILOTIDX, indicating the pilot
%   carrier locations from 1 to NFFT. For this syntax, the number of rows
%   in the output Y is NFFT-length(NULLIDX)-length(PILOTIDX), and accounts
%   for the removal of both pilots and nulls.
%       PILOTS is the pilot subcarrier output signal of size
%   Np-by-Nsym-by-Nr, where Np is equal to length(PILOTIDX). The pilot
%   subcarrier locations are assumed to be the same across each OFDM symbol
%   and transmit antenna.
%
%   % Example: Demodulation with different CP lengths for different symbols
%
%     nfft = 64;
%     cplen = [16 32];
%     nSym = 2;
%     dataIn = complex(randn(nfft,nSym),randn(nfft,nSym));
%     y1 = ofdmmod(dataIn,nfft,cplen);
%
%     x1 = ofdmdemod(y1,nfft,cplen);
%     max(x1-dataIn)
%
%   % Example: Demodulation with nulls and pilots unpacking
%
%     nfft     = 64;
%     cplen    = 16;
%     nSym     = 10;
%     nullIdx  = [1:6 33 64-4:64]';
%     pilotIdx = [12 26 40 54]';
%     numDataCarrs = nfft-length(nullIdx)-length(pilotIdx);
%     dataIn = complex(randn(numDataCarrs,nSym),randn(numDataCarrs,nSym));
%     pilots = repmat(pskmod((0:3).',4),1,nSym);
%     y2 = ofdmmod(dataIn,nfft,cplen,nullIdx,pilotIdx,pilots);
%
%     symOffset = cplen;
%     [x2,rxPilots] = ofdmdemod(y2,nfft,cplen,symOffset,nullIdx,pilotIdx);
%     max(x2-dataIn)
%     max(rxPilots-pilots)
%
%   See also ofdmmod, comm.OFDMDemodulator.

%   Copyright 2017-2018 The MathWorks, Inc.
    
%#codegen

narginchk(3,6);

% parse, validate inputs, set up processing

%[prmStr,dataIdx] = setup(x,nfft,cplen,varargin{:});



prmStr.NumReceiveAntennas = 1;
prmStr.FFTLength = 1536;
prmStr.NumSymbols = 1;
prmStr.NullIndices = NullIndices;
prmStr.hasPilots = 0;
prmStr.PilotIndices = [];
prmStr.SymbolOffset = SymbolOffset;
prmStr.CyclicPrefixLength = cplen;



dataIdx = coder.nullcopy(zeros(nfft-length(prmStr.NullIndices),1));
        dataIdx(:) = double(setdiff((1:nfft)',prmStr.NullIndices));


% Call internal fcn to compute grid
rxFullGrid = comm.internal.ofdm.demodulate(x,prmStr);


% Output signals
y = rxFullGrid(dataIdx,:,:);

end
% [EOF]

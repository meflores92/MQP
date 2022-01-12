function out = nrRateRecoverPolarOpt(in,K,N)
%nrRateRecoverPolar Polar rate matching recovery
%   OUT = nrRateRecoverPolar(IN,K,N) returns the rate-recovered output,
%   OUT, for an input, IN, of length E. The output, OUT, is of length N and
%   K represents the information block length. Coded-bit interleaving is
%   disabled (iBIL is set to false) for downlink configurations.
%
%   OUT = nrRateRecoverPolar(...,IBIL) allows the enabling of coded-bit
%   interleaving by specifying a boolean scalar (IBIL as true). This
%   setting is used for uplink configurations.
%
%   % Example:
%   % Rate match a polar encoded code block of length 512 to a vector of
%   % length 864 and then recover it.
%
%   N = 2^9;            % Polar encoded block length
%   K = 56;             % Number of information bits
%   E = 864;            % Number of rate matched output bits
%   iBIL = false;       % Deinterleaving of input bits
%
%   in = randi([0 1],N,1);
%   chIn = nrRateMatchPolar(in,K,E,iBIL);
%   out = nrRateRecoverPolar(chIn,K,N,iBIL);
%   isequal(out,in)
%
%   See also nrRateMatchPolar, nrPolarDecode, nrCRCDecode, nrDCIDecode,
%   nrBCHDecode.

%   Copyright 2018 The MathWorks, Inc.

%#codegen

%   Reference:
%   [1] 3GPP TS 38.212, "3rd Generation Partnership Project; Technical
%   Specification Group Radio Access Network; NR; Multiplexing and channel
%   coding (Release 15). Section 5.4.1.
    inE = in;

    % Bit selection, Section 5.4.1.2
    E = length(in);
    if E >= N
        % Just the first set output
        outN = inE(1:N);
    else
        if K/E <= 7/16
            % puncturing (put at the end)
            outN = zeros(N,1,class(in));          % 0s for punctures
            outN(end-E+1:end) = inE;
        else
            % shortening (put at the start)
            outN = 1e20*ones(N,1,class(in));      % use a large value for 0s
            outN(1:E) = inE;
        end
    end

    % Sub-block deinterleaving, Section 5.4.1.1
    out = subBlockDeinterleave(outN);

end


function out = subBlockDeinterleave(in)
% Sub-block deinterleaver
%   OUT = subBlockDeinterleave(IN) returns the sub-block deinterleaved
%   input.
%
%   Reference: TS 38.212, Section 5.4.1.1.

    N = length(in);
    jn = nr5g.internal.polar.subblockInterleaveMap(N);
    out = zeros(N,1,class(in));
    out(jn+1) = in;

end

function t = getT(E)

    % Use quadratic solution with ceil for >= in expression.
    t = ceil((-1+sqrt(1+8*E))/2);

end

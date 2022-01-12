function grid = OFDMDemodulateOpt(fftin,info,nSlot,N,hasSampleRate)
%OFDMDemodulate OFDM demodulation
%
%   Note: This is an internal undocumented function and its API and/or
%   functionality may change in subsequent releases.
%
%   GRID = OFDMDemodulate(WAVEFORM,INFO,NSLOT,N,HASSAMPLERATE,ISCARRIERSYNTAX)
%   performs OFDM demodulation of the time-domain waveform WAVEFORM given
%   OFDM related information INFO and number of OFDM symbols N to be
%   demodulated. NSLOT is used to select the correct cyclic prefix lengths
%   and symbol phases in INFO to use during OFDM demodulation.
%   HASSAMPLERATE (true,false) specifies whether or not a user-specified
%   sample is present.

%   Copyright 2020 The MathWorks, Inc.

%#codegen
    
    % Resampling (if required)
%     r = info.Resampling;
%     if (hasSampleRate || isempty(coder.target))
%         if (any([r.L r.M]~=1))
%             h = designMultirateFIR(r.M,r.L,r.TW,r.AStop);
%             fftin = cast(resample(double(waveform),r.M,r.L,h),class(waveform));
%         else
%             fftin = waveform;
%         end
%     else
%         % Inputs to 'designMultirateFIR' must be compile-time constants for
%         % code generation, but 'r.M', 'r.L' and 'r.TW' cannot be computed
%         % as constants if the function syntax is using nrCarrierConfig.
%         % This 'else' should not be reached when performing code
%         % generation, the function syntax is using nrCarrierConfig and
%         % resampling is required, because an outer function should have
%         % issued an error for this case
%         fftin = waveform;
%     end
    
    % Update cyclic prefix lengths and symbol phases to span all subframes
    % at least partially spanned by the waveform, and to take into
    % consideration the initial slot number
    [cpLengths,symbolPhases] = nr5g.internal.OFDMInfoRelativeNSlot(info,nSlot,N(1));
    
    % OFDM demodulation, TS 38.211 Section 5.3.1
    K = info.NSubcarriers;
    firstSC = (info.Nfft/2) - (K/2) + 1;
    nullIndices = [1:(firstSC-1) (firstSC+K):info.Nfft].';
    symOffset = fix(cpLengths(1:N(1)) * info.CyclicPrefixFraction);
    T = sum(cpLengths(1:N(1)) + info.Nfft);
    grid = ofdmdemodOpt(fftin(1:T,:),info.Nfft,cpLengths(1:N(1)),symOffset,nullIndices);
    
    % Phase decompensation, TS 38.211 Section 5.4
    grid = bsxfun(@times,grid,exp(-1i * symbolPhases(1:N(1))));
    
end

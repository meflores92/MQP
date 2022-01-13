function timingOffset = hTimingOffsetToFrame(BlockPattern, L_max, offset,ssbIdx,rxSampleRate)
    
    % As the symbol lengths are measured in FFT samples, scale the symbol
    % lengths to account for the receiver sample rate. Non-integer delays
    % are approximated at the end of the process.
    scs = 30;
    ofdmInfo = nrOFDMInfo(1,30,'SampleRate',760000000); % smallest FFT size for SCS-SR
    srRatio = rxSampleRate/(scs*1e3*ofdmInfo.Nfft);
    symbolLengths = ofdmInfo.SymbolLengths*srRatio;
    
    % Adjust timing offset to the start of the SS block. This step removes
    % the extra offset introduced in the reference grid during PSS search,
    % which contained the PSS in the second OFDM symbol.
    offset = offset + symbolLengths(1);
    
    % Timing offset is adjusted so that the received grid starts at the
    % frame head i.e. adjust the timing offset for the difference between
    % the first symbol of the strongest SSB, and the start of the frame
    %burstStartSymbols = hSSBurstStartSymbols(BlockPattern,L_max); % Start symbols in SSB numerology
    burstStartSymbols = [ 2     8    16    22    30    36    44    50];
    ssbFirstSym = burstStartSymbols(ssbIdx+1); % 0-based
    
    % Adjust for whole subframes
    symbolsPerSubframe = length(symbolLengths);
    subframeOffset = floor(ssbFirstSym/symbolsPerSubframe);
    samplesPerSubframe = sum(symbolLengths);
    timingOffset = offset - (subframeOffset*samplesPerSubframe);
    
    % Adjust for remaining OFDM symbols and round offset if not integer
    symbolOffset = mod(ssbFirstSym,symbolsPerSubframe);
    symbolOffset = symbolOffset(1);
    timingOffset = round(timingOffset - sum(symbolLengths(1:symbolOffset)));
    
end

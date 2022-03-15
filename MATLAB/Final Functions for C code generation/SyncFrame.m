%This function returns the index of the start of the ss block 
function timingOffset = SyncFrame(rxWaveform)
coder.inline('never');
refGrid = zeros([20*12 2]);
refGrid(nrPSSIndices,2) = nrPSS(0);

% Timing estimation. This is the timing offset to the OFDM symbol of
% the detected SSB
timingOffset = nrTimingEstimate(rxWaveform,20,30,0,refGrid,'SampleRate',46.08e6) - 1644;
end
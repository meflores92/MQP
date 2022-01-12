function timingOffset = SyncFrame()
rx = coder.load('Corrected_Waveform', 'rxWaveformn');
rxWaveform = rx.rxWaveformn;

refGrid = zeros([20*12 2]);
refGrid(nrPSSIndices,2) = nrPSS(0); % Second OFDM symbol for correct CP length

% Timing estimation. This is the timing offset to the OFDM symbol prior to
% the detected SSB due to the content of the reference grid
rxSampleRate = 46.08e6;
samplesPerSymbol = round(rxSampleRate*(0.001/2)/14);
rxWaveform = rxWaveform(1:samplesPerSymbol*4+1);

tic
timingOffset = nrTimingEstimate(rxWaveform,20,30,0,refGrid,'SampleRate',46.08e6) - samplesPerSymbol;
toc
tic
toc
end
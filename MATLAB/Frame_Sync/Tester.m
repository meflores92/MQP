% rx = coder.load('Corrected_Waveform', 'rxWaveformn');
% rxWaveformIn = rx.rxWaveformn;
% rxSampleRate = 46.08e6;
% samplesPerSymbol = round(rxSampleRate*(0.001/2)/14);
% rxWaveform = rxWaveformIn(921600:samplesPerSymbol*4+921601);
% 
% for x = 1:100
% offset = SyncFrame(rxWaveform);
% rxWaveform = rxWaveformIn(921600+offset:samplesPerSymbol*4+921601+offset);
% end

offset = SyncFrame();
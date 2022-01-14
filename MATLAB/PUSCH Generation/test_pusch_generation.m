clear;
clc;
freq = 3.6192e9 + (200*30e3);
% freq = 30e6
% modulation = 1;
% [waveform, sine_wave, time] = pusch_generation(modulation, freq);
% sine_wave = sine_wave.';
%time = time(:);

RadioMasterClockRate = 46.08e6;
RadioDecimationFactor = 1;
CenterFrequency = freq;
Serial = '3164073';
RadioGain = 70;
burst = 5;
% radio = comm.SDRuTransmitter('Platform', 'B210','SerialNum', Serial, 'MasterClockRate', RadioMasterClockRate, 'SamplesPerFrame', 200000,...
%    'CenterFrequency', CenterFrequency, 'Gain',  RadioGain, 'DecimationFactor', 1, 'ClockSource', 'Internal', 'PPSSource',  'Internal', 'OutputDataType', 'double', ...
%    'EnableBurstMode', true, 'NumFramesInBurst', burst);
% 
% txWaveform = radio();

txSampleRate = RadioMasterClockRate;

fPhaseComp = CenterFrequency;

sine_wave = dsp.SineWave (...
    'Frequency',           freq, ...
    'Amplitude',           1,...
    'SampleRate',          txSampleRate, ...
    'SamplesPerFrame',     20e7);
waveform = sine_wave();
tx(waveform);

% figure(1);
% plot(time(1:100, 1), sine_wave(1:100, 1));
figure(2)
spectrogram(waveform, [], [], [], txSampleRate,'yaxis')
%ylim([200 400])
% spectrogram(sine_wave(:,1),ones(nfft,1),0,nfft,'centered',samplerate,'yaxis','MinThreshold',-130);


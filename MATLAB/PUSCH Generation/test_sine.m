clear;
clc;
% freq = 3.6192e9 + (200*30e3);
freq = 20e6;
% modulation = 1;
% [waveform, sine_wave, time] = pusch_generation(modulation, freq);
% sine_wave = sine_wave.';
%time = time(:);

RadioMasterClockRate = 46.08e6;
RadioDecimationFactor = 1;
CenterFrequency = 2e9;
Serial = '3164073';
RadioGain = 70;
burst = 50;
tx = comm.SDRuTransmitter('Platform', 'B210','SerialNum', Serial, 'MasterClockRate', RadioMasterClockRate,...
   'CenterFrequency', CenterFrequency, 'Gain',  RadioGain, 'ClockSource', 'Internal', 'PPSSource',  'Internal', 'InterpolationFactor', 1);

% txWaveform = radio();

txSampleRate = RadioMasterClockRate;

fPhaseComp = CenterFrequency;

sine_wave = dsp.SineWave (...
    'Frequency',           freq, ...
    'Amplitude',           10,...
    'SampleRate',          txSampleRate, ...
    'SamplesPerFrame',     375000);
waveform = sine_wave();

%tx.EnableBurstMode = true;
%tx.NumFramesInBurst = burst;

numFrames = 1000;


fs = RadioMasterClockRate;
sw = dsp.SineWave;
sw.Amplitude = 0.5;
sw.Frequency = 20e6;
sw.ComplexOutput = true;
sw.SampleRate = fs;
sw.SamplesPerFrame = 375000; 
txWaveform = sw();


 for frame = 1:numFrames
     underrun = tx(txWaveform);
 end

release(tx)
% figure(1);
% plot(time(1:100, 1), sine_wave(1:100, 1));
% figure(2)
% spectrogram(waveform, [], [], [], txSampleRate,'yaxis')
%ylim([200 400])
% spectrogram(sine_wave(:,1),ones(nfft,1),0,nfft,'centered',samplerate,'yaxis','MinThreshold',-130);


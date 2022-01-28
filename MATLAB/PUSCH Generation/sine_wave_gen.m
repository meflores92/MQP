function [waveforms] = sine_wave_gen()
%#codegen
clear;
clc;
% This program is used to generate csv file containing different sinusoidal
% waveforms
% Center frequency 3.6192e9
% Fequency of point A is 3.60012e9, pointA = centerfreq - 12*53*30kHz

start_freq = -12*53*30e3; 
stop_freq = 12*53*30e3;
SCS = 30e3; % SCS is 30kHz
freqs = start_freq:SCS:stop_freq; % Creates an array of equally space frequency points according to SCS
SamplesPerFrame = 8e3;
waveforms = zeros(length(freqs), SamplesPerFrame);
RadioMasterClockRate = 46.08e6;
txSampleRate = RadioMasterClockRate;

for i = 1:length(freqs)
    sine_wave = dsp.SineWave (...
        'Frequency',           freqs(i), ...
        'Amplitude',           1,...
        'SampleRate',          txSampleRate, ...
        'SamplesPerFrame',     SamplesPerFrame);
    % post_mod = qammod(sine_wave(), 4);
    waveforms(i, 1:end) = sine_wave()';
    
end 
end
sampling_rate = 46.08e6;
symbols_in_slot = 14;
time_per_slot = 0.5e-3;

time_per_symbol = time_per_slot/symbols_in_slot;
samps_per_symbol = time_per_symbol*sampling_rate;

%Calculate Size of PDCCH Buffer
pdcch_samps = ceil(samps_per_symbol*4);

%Calculate Slot LUT
time = [0.5e-3:0.5e-3:10e-3];
samps = ceil(time*sampling_rate);

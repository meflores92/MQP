function grid = nrOFDMDemodulateOpt(waveform,info,nSlot,N,hasSampleRate)
%nrOFDMDemodulate OFDM demodulation
%   GRID = nrOFDMDemodulate(CARRIER,WAVEFORM) performs OFDM demodulation of
%   the time domain waveform, WAVEFORM, given carrier configuration object
%   CARRIER.
%
%   CARRIER is a carrier configuration object, <a 
%   href="matlab:help('nrCarrierConfig')"
%   >nrCarrierConfig</a>. Only these
%   object properties are relevant for this function:
%
%   SubcarrierSpacing - Subcarrier spacing in kHz (15, 30, 60, 120, 240)
%   CyclicPrefix      - Cyclic prefix ('normal', 'extended')
%   NSizeGrid         - Number of resource blocks in carrier resource grid
%                       (1...275)
%   NSlot             - Slot number
%
%   WAVEFORM is a T-by-R matrix where T is the number of time-domain 
%   samples in the waveform and R is the number of receive antennas.
%
%   GRID is an array of size K-by-L-by-R where K is the number of
%   subcarriers and L is the number of OFDM symbols.
%
%   GRID = nrOFDMDemodulate(WAVEFORM,NRB,SCS,INITIALNSLOT) performs OFDM
%   demodulation of waveform WAVEFORM for NRB resource blocks with 
%   subcarrier spacing SCS and initial slot number INITIALNSLOT.
%
%   NRB is the number of resource blocks (1...275).
%
%   SCS is the subcarrier spacing in kHz (15, 30, 60, 120, 240).
%
%   INITIALNSLOT is the 0-based initial slot number, a non-negative scalar
%   integer. The function selects the appropriate cyclic prefix length for
%   the OFDM demodulation based on the initial slot number modulo the
%   number of slots per subframe.
%
%   GRID = nrOFDMDemodulate(...,NAME,VALUE) specifies additional options as
%   NAME,VALUE pairs to allow control over the OFDM demodulation:
%
%   CyclicPrefix         - Cyclic prefix ('normal' (default), 'extended').
%                          This option is only applicable for function
%                          syntaxes not using nrCarrierConfig
%   Nfft                 - Desired number of FFT points to use in the OFDM
%                          demodulator. If absent or set to [], a default 
%                          value is selected based on other parameters, see
%                          <a href="matlab: doc('nrOFDMDemodulate')"
%                          >nrOFDMDemodulate</a> for details
%   SampleRate           - Sample rate of input waveform. If absent or set
%                          to [], the default value is SampleRate = Nfft *
%                          SCS. If required, the input waveform is
%                          resampled from the specified sample rate to the
%                          sample rate used during OFDM demodulation, Nfft
%                          * SCS
%   CarrierFrequency     - Carrier frequency (in Hz) to calculate the phase
%                          decompensation applied for each OFDM symbol 
%                          (denoted f_0 in TS 38.211 Section 5.4). Default 
%                          is 0
%   CyclicPrefixFraction - Starting position of OFDM symbol demodulation 
%                          (FFT window position) within the cyclic prefix.
%                          Specified as a fraction of the cyclic prefix, in
%                          the range [0,1], with 0 representing the start
%                          of the cyclic prefix and 1 representing the end
%                          of the cyclic prefix. Default is 0.5
%
%   Note that for the numerologies specified in TS 38.211 Section 4.2, 
%   extended cyclic prefix length is only applicable for 60 kHz subcarrier
%   spacing.
%
%   % Example:
%
%   % Configure carrier for 20 MHz bandwidth
%   carrier = nrCarrierConfig;
%   carrier.NSizeGrid = 106;
%
%   % Configure PDSCH and create PDSCH DM-RS symbols and indices
%   pdsch = nrPDSCHConfig;
%   pdsch.NumLayers = 2;
%   sym = nrPDSCHDMRS(carrier,pdsch);
%   ind = nrPDSCHDMRSIndices(carrier,pdsch);
%
%   % Create a carrier resource grid and map PDSCH DM-RS symbols
%   txGrid = nrResourceGrid(carrier,pdsch.NumLayers);
%   txGrid(ind) = sym;
%
%   % Perform OFDM modulation
%   [txWaveform,info] = nrOFDMModulate(carrier,txGrid);
%
%   % Apply a simple 2-by-1 channel
%   H = [0.6; 0.4];
%   rxWaveform = txWaveform * H;
%
%   % Perform OFDM demodulation
%   rxGrid = nrOFDMDemodulate(carrier,rxWaveform);
%
%   See also nrCarrierConfig, nrOFDMInfo, nrOFDMModulate, nrResourceGrid.

%   Copyright 2020 The MathWorks, Inc.

%#codegen

    %narginchk(2,14);
    
    % Validate inputs and get OFDM information
    %[waveform,info,nSlot,N,hasSampleRate] = validateInputs(varargin{:});
    
    % Perform OFDM demodulation
%     sum = 0;
%     for x = 1:100
%     tic
%     grid = nr5g.internal.OFDMDemodulate(waveform,info,nSlot,N,hasSampleRate);
%     a =toc;
%     sum = sum+a;
%     end
%     sum/100
%     
%     sum = 0;
%     for x = 1:100
%     tic
%     grid = OFDMDemodulateOpt(waveform,info,nSlot,N,hasSampleRate);
%     a =toc;
%     sum = sum+a;
%     end
%     sum/100
    grid = OFDMDemodulateOpt(waveform,info,nSlot,N,hasSampleRate);
end

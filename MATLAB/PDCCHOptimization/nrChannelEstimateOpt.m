function [H,nVar,info] = nrChannelEstimateOpt(carrier,rxGrid,cDMRSInd,cDMRS)
%nrChannelEstimate Practical channel estimation
%   [H,NVAR,INFO] = nrChannelEstimate(...) performs channel estimation,
%   returning channel estimate H, noise variance estimate NVAR and
%   information structure INFO. H is a K-by-N-by-R-by-P array where K is
%   the number of subcarriers, N is the number of OFDM symbols, R is the
%   number of receive antennas and P is the number of reference signal
%   ports. NVAR is scalar indicating the measured variance of additive
%   white Gaussian noise on the received reference symbols. INFO is a
%   structure containing the field:
%   AveragingWindow - a 2-element row vector [F T] indicating the number
%                     of adjacent reference symbols in the frequency
%                     direction F and time direction T over which averaging
%                     was performed prior to interpolation
%
%   [H,NVAR,INFO] = nrChannelEstimate(CARRIER,RXGRID,REFIND,REFSYM)
%   performs channel estimation on the received resource grid RXGRID using
%   reference symbols REFSYM whose locations are given by REFIND.
%
%   CARRIER is a carrier configuration object, <a
%   href="matlab:help('nrCarrierConfig')"
%   >nrCarrierConfig</a>. Only this
%   object property is relevant for this function:
%
%   CyclicPrefix      - Cyclic prefix ('normal', 'extended')
%
%   RXGRID is an array of size K-by-L-by-R. K is the number of subcarriers,
%   given by CARRIER.NSizeGrid * 12. L is the number of OFDM symbols in one
%   slot, given by CARRIER.SymbolsPerSlot.
%
%   REFIND and REFSYM are the reference signal indices and symbols,
%   respectively. REFIND is an array of 1-based linear indices addressing a
%   K-by-L-by-P resource array. P is the number of reference signal ports
%   and is inferred from the range of values in REFIND. Only nonzero
%   elements in REFSYM are considered. Any zero-valued elements in REFSYM
%   and their associated indices in REFIND are ignored.
%
%   [H,NVAR,INFO] = nrChannelEstimate(CARRIER,RXGRID,REFGRID) specifies a
%   predefined reference resource grid in REFGRID. REFGRID is an array with
%   nonzero elements representing the reference symbols in their
%   appropriate locations. It is of size K-by-N-by-P, where N is the number
%   of OFDM symbols. REFGRID can span multiple slots. RXGRID must be an
%   array of size K-by-N-by-R.
%
%   [H,NVAR,INFO] = nrChannelEstimate(RXGRID,REFIND,REFSYM,NAME,VALUE,...)
%   and [H,NVAR,INFO] = nrChannelEstimate(RXGRID,REFGRID,NAME,VALUE,...)
%   perform channel estimation as above, but an additional NAME,VALUE pair
%   is used in place of the CARRIER configuration object:
%
%   'CyclicPrefix'      - Cyclic prefix ('normal' (default), 'extended')
%
%   Note that for the numerologies specified in TS 38.211 Section 4.2,
%   extended cyclic prefix length is only applicable for 60 kHz subcarrier
%   spacing. For normal cyclic prefix there are L=14 OFDM symbols in a
%   slot. For extended cyclic prefix, L=12.
%
%   [H,NVAR,INFO] = nrChannelEstimate(...,NAME,VALUE,...) specifies
%   additional options as NAME,VALUE pairs:
%
%   'CDMLengths'      - A 2-element row vector [FD TD] specifying the
%                       length of FD-CDM and TD-CDM despreading to perform.
%                       A value of 1 for an element indicates no CDM and a
%                       value greater than 1 indicates the length of the
%                       CDM. For example, [2 1] indicates FD-CDM2 and no
%                       TD-CDM. The default is [1 1] (no orthogonal
%                       despreading)
%
%   'AveragingWindow' - A 2-element row vector [F T] specifying the number
%                       of adjacent reference symbols in the frequency
%                       domain F and time domain T over which to average
%                       prior to interpolation. F and T must be odd or
%                       zero. If F or T is zero, the averaging value is
%                       determined automatically from the estimated SNR
%                       (calculated using NVAR). The default is [0 0]
%
%   Example:
%   % Create a resource grid containing the PDSCH DM-RS and pass it through
%   % a TDL-C channel. Estimate the channel response and compare it with
%   % the perfect channel estimator.
%
%   carrier = nrCarrierConfig;
%   pdsch = nrPDSCHConfig;
%   dmrsInd = nrPDSCHDMRSIndices(carrier,pdsch);
%   dmrsSym = nrPDSCHDMRS(carrier,pdsch);
%   nTxAnts = 1;
%   txGrid = nrResourceGrid(carrier,nTxAnts);
%   txGrid(dmrsInd) = dmrsSym;
%
%   [txWaveform,ofdmInfo] = nrOFDMModulate(carrier,txGrid);
%
%   channel = nrTDLChannel;
%   channel.NumReceiveAntennas = 1;
%   channel.SampleRate = ofdmInfo.SampleRate;
%   channel.DelayProfile = 'TDL-C';
%   channel.DelaySpread = 100e-9;
%   channel.MaximumDopplerShift = 20;
%   chInfo = info(channel);
%   maxChDelay = ceil(max(chInfo.PathDelays*channel.SampleRate)) + chInfo.ChannelFilterDelay;
%   [rxWaveform,pathGains] = channel([txWaveform; zeros(maxChDelay,nTxAnts)]);
%
%   offset = nrTimingEstimate(carrier,rxWaveform,dmrsInd,dmrsSym);
%   rxWaveform = rxWaveform(1+offset:end,:);
%
%   rxGrid = nrOFDMDemodulate(carrier,rxWaveform);
%
%   [H,nVar,estInfo] = nrChannelEstimate(carrier,rxGrid,dmrsInd,dmrsSym);
%
%   pathFilters = getPathFilters(channel);
%   H_ideal = nrPerfectChannelEstimate(carrier,pathGains,pathFilters,offset);
%
%   figure;
%   subplot(1,2,1);
%   imagesc(abs(H));
%   xlabel('OFDM symbol');
%   ylabel('Subcarrier');
%   title('Practical estimate magnitude');
%   subplot(1,2,2);
%   imagesc(abs(H_ideal));
%   xlabel('OFDM symbol');
%   ylabel('Subcarrier');
%   title('Perfect estimate magnitude');
%
%   See also nrTimingEstimate, nrPerfectChannelEstimate,
%   nrPerfectTimingEstimate, nrCarrierConfig.

%   Copyright 2019-2020 The MathWorks, Inc.

%#codegen

% Determine if syntax with nrCarrierConfig is being used and parse
% relevant inputs

% Validate type of grid and number of dimensions in grid, dimension
% sizes are validated when other relevant parameters are known
% (refInd,refSym versus refGrid syntax, normal versus extended cyclic
% prefix)
K = size(rxGrid,1);
NRB = K / 12;

% Determine whether the refInd,refSym syntax or refGrid syntax is being
% used
opts.CDMLengths= [1,1];
opts.AveragingWindow = [0,0];
% Get OFDM information. Subcarrier spacing (SCS) is hard-wired to 15
% kHz because SCS does not affect OFDM information fields as used in
% the channel estimator:
%  * SampleRate is not used
%  * SymbolsPerSubframe is not used
%  * larger CP lengths every 0.5ms are not used, because only
%    min(CyclicPrefixLengths) is used
%  * length of CyclicPrefixLengths is not used
cp = carrier.CyclicPrefix;

ECP = strcmpi(cp,'extended');
SCS = 15;
ofdmInfo = nr5g.internal.OFDMInfo(NRB,SCS,ECP,struct());
ofdmInfo.CyclicPrefix = cp;

% Get the number of subcarriers K and OFDM symbols L from the OFDM
% information
K = ofdmInfo.NSubcarriers;
L = ofdmInfo.SymbolsPerSlot;

% Validate reference inputs

refInd = cDMRSInd;
refSym = cDMRS;

% Validate reference indices and place in a single column

% Validate reference symbols and place in a single column
refSym = refSym(:);
refInd = double(refInd(:));

% The number of OFDM symbols 'N' in the implied reference grid is
% 'L', the number of OFDM symbols in one slot
N = L;

% Get the number of ports, based on the range of the reference
% symbol indices
P = ceil(max(refInd/(K*L)));
if (~isfinite(P))
    P = 1;
end

% Validate received grid OFDM symbol dimension, it must span one
% slot

% Create reference grid and map reference symbols
refGrid = zeros([K L P],'like',refSym);
refGrid(refInd) = refSym;


% Extract reference indices and symbols from reference grid
refInd = find(refGrid(:)~=0);
refSym = refGrid(refInd);

% Validate reference grid OFDM symbol dimension, it must be equal to
% the number of OFDM symbols in the received grid

% Get channel estimation configuration parameters from options
estConfig.CDMLengths = double(opts.CDMLengths);
estConfig.AveragingWindow = double(opts.AveragingWindow);


% Get optional inputs, inputs whose position depends upon the syntax,
% or variables that depend upon optional inputs
% rxGrid: received grid
% refInd: reference signal indices
% refSym: reference signal symbols
% N: number of OFDM symbols
% P: number of transmit antenna ports
% ofdmInfo: OFDM dimensionality information
% estConfig: channel estimator configuration

% Get channel estimate output dimensions, final channel estimate will
% be of size K-by-N-by-R-by-P
K = ofdmInfo.NSubcarriers;
R = size(rxGrid,3);

% 'eK' is the number of extra subcarriers that will be added to the
% channel estimate grid (half at each side), to mitigate band edge
% effects when reconstructing the channel frequency response (CFR)
% from the denoised channel impulse response (CIR)
eRB = 4;
eK = eRB * 12;

% Create the channel estimate grid, including 'eK' extra subcarriers
siz = [K+eK N R P];
H = complex(zeros(siz,'like',rxGrid));

% Create matrices 'kmin' and 'kmax' which will be used to store the
% minimum and maximum frequency subscripts, for each OFDM symbol and
% transmit port
kmin = zeros([N P]);
kmax = zeros([N P]);

% Create array 'refRBs' which will be used to identify the resource
% blocks which contain reference symbols, for each OFDM symbol and
% transmit port
NRB = K / 12;
refRBs = zeros([NRB+eRB N P]);

% ---------------------------------------------------------------------
% LS estimation, CDM despreading
% ---------------------------------------------------------------------

% Extract CDM despreading parameters
fdCDM = estConfig.CDMLengths(1);
tdCDM = estConfig.CDMLengths(2);

% For each transmit port
for p = 1:P
    
    % Get frequency (subcarrier k) and time (OFDM symbol n) subscripts
    % of reference signal for the current port. 'thisport' is a logical
    % indexing vector for the current port, used to extract the
    % corresponding reference symbols
    [ksubs,nsubs,thisport] = getPortSubscripts(K,N,P,refInd,p);
    refSymThisPort = refSym(thisport);
    
    % For each OFDM symbol
    un = unique(nsubs).';
    for uni = 1:numel(un)
        
        % Get frequency and OFDM symbol subscripts
        n = un(uni);
        k = ksubs(nsubs==n);
        
        % Initialize frequency subscripts 'ke' to be used for
        % interpolation, taking extra subcarriers into account
        ke = k + eK/2;
        
        % Calculate 'kmin' and 'kmax', the minimum and maximum
        % frequency subscripts for this OFDM symbol and port
        rbsubs = (ke - 1) / 12;
        kmin(n,p) = floor(min(rbsubs))*12 - (eK/2) + 1;
        kmax(n,p) = ceil(max(rbsubs) + 1/12)*12 + (eK/2);
        
        % Mark resource blocks (RBs) containing reference signals for
        % this OFDM symbol and port in 'refRBs'
        refRBs(unique(floor(rbsubs)) + 1,n,p) = 1;
        
        % For each receive antenna
        for r = 1:R
            
            % Perform least squares (LS) estimate of channel in the
            % locations of the reference symbols. 'H_LS' is a column
            % vector containing the LS estimates for all subcarriers
            % for the current port, OFDM symbol and receive antenna
            H_LS = rxGrid(k,n,r) ./ refSymThisPort(nsubs==n);
            
            % Perform FD-CDM despreading if required
            if (fdCDM>1)
                % 'm' is zero if the LS estimates divide evenly into
                % FD-CDM groups, otherwise 'm' is the number of LS
                % estimates in the final partial group
                m = mod(numel(H_LS),fdCDM);
                for a = 0:double(m~=0)
                    if (~a)
                        % whole CDM lengths (may be empty)
                        k_LS = 1:(numel(H_LS)-m);
                        nkCDM = fdCDM;
                    else
                        % part CDM length (may be empty)
                        k_LS = numel(H_LS) + (-m+1:0);
                        nkCDM = m;
                    end
                    % Extract the LS estimates and reshape so that each
                    % column contains an FD-CDM group
                    x = reshape(H_LS(k_LS),nkCDM,[]);
                    % Average across 1st dimension (i.e. across the
                    % FD-CDM group) and repeat the averaged value
                    x = repmat(mean(x,1),[nkCDM 1]);
                    % Reshape back into a single column
                    H_LS(k_LS) = reshape(x,[],1);
                end
            end
            
            % Assign the estimates into the appropriate region of
            % the overall channel estimate array
            H(ke,n,r,p) = H_LS;
            
        end
        
    end
    
    % Perform TD-CDM despreading if required
    if (tdCDM>1)
        % 'm' is zero if the estimates divide evenly into TD-CDM
        % groups, otherwise 'm' is the number of estimates in the final
        % partial group
        m = mod(numel(un),tdCDM);
        for a = 0:double(m~=0)
            if (~a)
                % whole CDM lengths (may be empty)
                unCDM = un(1:end-m);
                nlCDM = tdCDM;
            else
                % part CDM length (may be empty)
                unCDM = un(end-m+1:end);
                nlCDM = m;
            end
            for r = 1:R
                % Extract the estimates and reshape so that each row
                % and plane contains a TD-CDM group
                x = reshape(H(:,unCDM,r,p),K+eK,nlCDM,[]);
                % 'mx' is the mean of 'x' across the 2nd dimension
                % (i.e. across the TD-CDM group), excluding zeros
                mx = sum(x,2) ./ max(sum(x~=0,2),1);
                % Repeat the averaged value and reshape back into a
                % matrix
                x = repmat(mx,[1 nlCDM]);
                H(:,unCDM,r,p) = reshape(x,[],numel(unCDM));
            end
        end
    end
    
end

% ---------------------------------------------------------------------
% Interpolation in frequency direction, CIR denoising
% ---------------------------------------------------------------------

% Calculate minimum cyclic prefix length in terms of a DFT of size K+eK
cp = floor(min(ofdmInfo.CyclicPrefixLengths) / ofdmInfo.Nfft * (K+eK));

% Create time-domain windowing function for CIR denoising
w = raised_cosine_window(cp*2,cp);
w = [w; zeros([K+eK-length(w) 1])];
w = circshift(w,-cp-floor(cp/2));

% Create empty matrices 'Hn' and 'subn' which will be used to store
% channel estimates and subscripts used for noise estimation
Hn = zeros([0 1],'like',H);
subn = zeros([0 4]);

% For each transmit port
for p = 1:P
    
    % Get frequency (subcarrier k) and time (OFDM symbol n) subscripts
    % of the reference signal for the current port
    [ksubs,nsubs] = getPortSubscripts(K,N,P,refInd,p);
    
    % Create interpolators ('vpisLow' and 'vpisHigh') that will be used
    % to create virtual pilots (VPs) for the low and high edge of the
    % grid for this port, and the corresponding sets of unique minima
    % ('ukLow') and maxima ('ukHigh') frequency subscripts (one value
    % for each interpolator). The interpolators use reference symbols
    % from any OFDM symbol across the whole reference resource grid,
    % and whose frequency subscripts are near the minimum or maximum
    % frequency subscript for that interpolator
    [vpisLow,ukLow,ndsLow] = ...
        createVPInterpolators(kmin(:,p),K,eK,ksubs,nsubs,[-12 eK/2+12]);
    [vpisHigh,ukHigh,ndsHigh] = ...
        createVPInterpolators(kmax(:,p),K,eK,ksubs,nsubs,[-eK/2-12 12]);
    
    % For each OFDM symbol
    un = unique(nsubs).';
    for uni = 1:numel(un)
        
        % Get frequency subscripts for the current OFDM symbol
        n = un(uni);
        k = ksubs(nsubs==n);
        
        % Prepare subscripts 'ln' and 'pn' used for noise estimation
        nK = numel(k);
        ln = repmat(n,[nK 1]);
        pn = repmat(p,[nK 1]);
        
        % Initialize frequency subscripts 'ke' to be used for
        % interpolation, taking extra subcarriers into account. Also
        % store these as 'ke0' which is used to reset these subscripts
        % for each receive antenna
        ke = k + eK/2;
        ke0 = ke;
        
        % Calculate range of frequency subscripts for this OFDM symbol
        % and port
        krange = (kmin(n,p):kmax(n,p)).';
        
        % Select VP interpolators that correspond to the current OFDM
        % symbol
        vpiIdxLow = find(ukLow==kmin(n,p));
        vpiLow = vpisLow{vpiIdxLow(1)};
        ndLow = ndsLow(vpiIdxLow(1));
        vpiIdxHigh = find(ukHigh==kmax(n,p));
        vpiHigh = vpisHigh{vpiIdxHigh(1)};
        ndHigh = ndsHigh(vpiIdxHigh(1));
        
        % Extract channel estimate regions that will be used to provide
        % values for the VP interpolation for each receive antenna
        vpIndicesLow = calculateVPIndices(ndLow,vpiLow,siz,n,p);
        HvpLow = H(vpIndicesLow);
        vpIndicesHigh = calculateVPIndices(ndHigh,vpiHigh,siz,n,p);
        HvpHigh = H(vpIndicesHigh);
        
        % For each receive antenna
        for r = 1:R
            
            % Initialize frequency subscripts to be used for
            % interpolation
            ke = ke0;
            
            % Get LS estimates (already CDM despread if required).
            % 'H_LS' is a column vector containing the LS estimates for
            % all subcarriers for the current port, OFDM symbol and
            % receive antenna
            H_LS = H(ke,n,r,p);
            
            % Record LS estimates and their subscripts, for use in
            % noise estimation
            rn = repmat(r,[nK 1]);
            Hn = [Hn; H_LS]; %#ok<AGROW>
            subn = [subn; [k ln rn pn]]; %#ok<AGROW>
            
            % If FD-CDM despreading is configured, replace groups of
            % LS estimates with a single estimate in the position of
            % the average frequency index
            if (fdCDM>1)
                H_LS = H_LS(1:fdCDM:end);
                nkCDM = min(fdCDM,numel(ke));
                m = mod(numel(ke),nkCDM);
                km = ke(end-m+1:end);
                ke = reshape(ke(1:end-m),nkCDM,[]);
                ke = [mean(ke,1).'; repmat(mean(km),double(m~=0),1)];
            end
            
            % Create virtual pilots at the lower edge: every 6
            % subcarriers between the minimum frequency subscript
            % 'kmin' and the lowest reference signal subcarrier
            kvpLow = (kmin(n,p):6:(ke(1)-1)).';
            vpLow = createVPs(ndLow,vpiLow,kvpLow,n,HvpLow(:,r));
            
            % Create virtual pilots at the upper edge: every 6
            % subcarriers between the maximum frequency subscript
            % 'kmax' and the highest reference signal subcarrier
            kvpHigh = flipud((kmax(n,p):-6:(ke(end)+1)).');
            vpHigh = createVPs(ndHigh,vpiHigh,kvpHigh,n,HvpHigh(:,r));
            
            % Perform interpolation in the frequency direction to give
            % a channel estimate for all relevant frequency subscripts,
            % and assign the estimate into the appropriate region of
            % the overall channel estimate array
            kevp = [kvpLow; ke; kvpHigh];
            Hvp = [vpLow; H_LS; vpHigh];
            Hk = interpolate(1,kevp,n,Hvp,krange,n,'spline',0);
            H(krange,n,r,p) = Hk(:,1);
            
            % Perform an IDFT of the channel estimate to give the CIR
            h = ifft(H(:,n,r,p));
            
            % Apply time domain windowing function to denoise CIR
            h = h .* w;
            
            % Perform a DFT of the CIR to give the denoised CFR
            H(:,n,r,p) = fft(h);
            
        end
        
    end
    
end

% Remove extra subcarriers from the channel estimate that were added
% during interpolation, remove the corresponding resource blocks from
% 'RBs' and adjust 'kmax' appropriately ('kmin' and 'kmax' are used
% subsequently to determine indices for interpolation in the time
% direction)
H = H(eK/2 + (1:K),:,:,:);
refRBs = refRBs(eRB/2 + (1:NRB),:,:);
kmax = kmax - eK;

% ---------------------------------------------------------------------
% Noise estimation, averaging parameter selection
% ---------------------------------------------------------------------

% Perform noise estimation, by measuring the variance between the
% original LS estimates 'Hn' and the same locations in the denoised
% estimate 'Hdn'. The scaling factor 's' is empirically determined.
% Note that the accuracy of the noise estimation will be influenced by
% the number of LS estimates available
if (numel(Hn)>R)
    indn = sub2ind(size(H),subn(:,1),subn(:,2),subn(:,3),subn(:,4));
    Hdn = H(indn);
    subnR1 = subn(subn(:,3)==1,[1 2 4]); % 1st receive antenna
    s = determineNoiseScaling(ofdmInfo,subnR1,fdCDM,tdCDM);
    nVar = var(Hn - Hdn) * s;
    % Determine how the channel CSI will affect the receiver SNR
    % (assuming MMSE equalization)
    c = determineCSIScaling(refRBs,H,nVar);
else
    nVar = zeros(1,1,class(H));
    c = ones(1,1,class(H));
end

% If either averaging parameter is not set, automatically choose a
% value based on the SNR (calculated from the noise variance and the
% effect of CSI)
estConfig = setAveragingParameters(estConfig,nVar*c);

% Return averaging parameters in info output structure
info.AveragingWindow = estConfig.AveragingWindow;

% ---------------------------------------------------------------------
% Averaging and interpolation in frequency direction
% ---------------------------------------------------------------------

% Extract frequency averaging parameter
freqAveraging = estConfig.AveragingWindow(1);

% For each transmit port
for p = 1:P
    
    % Get time (OFDM symbol n) subscripts of reference signal for the
    % current port
    [~,nsubs] = getPortSubscripts(K,N,P,refInd,p);
    
    if (freqAveraging~=1)
        
        % For each OFDM symbol
        un = unique(nsubs).';
        for uni = 1:numel(un)
            
            % Get current OFDM symbol number 'n'
            n = un(uni);
            
            % For each receive antenna
            for r = 1:R
                
                % Get frequency subscripts 'k' for the current OFDM
                % symbol (2 points per RB)
                krange = (kmin(n,p):kmax(n,p)).';
                k = krange(3:6:end);
                
                % Extract channel estimate for those subscripts. 'Hlrp'
                % is a column vector containing the denoised estimates
                % for 2 subcarriers per resource block for the current
                % port, OFDM symbol and receive antenna
                Hlrp = H(k,n,r,p);
                
                % Perform averaging in the frequency direction
                Hlrp = averaging(Hlrp,freqAveraging);
                
                % Perform interpolation of estimates in the frequency
                % direction to give a channel estimate for all
                % subcarriers, and assign the estimate into the
                % appropriate region of the overall channel estimate
                % array
                H(krange,n,r,p) = interpolate(1, ...
                    k,n,Hlrp,krange,n,'spline',1);
                
            end
            
        end
        
    end
    
    % For each OFDM symbol for this port, blank any subcarriers
    % outside of the RBs that originally contained reference symbols
    un = find((kmin(:,p).')~=0);
    for uni = 1:numel(un)
        
        n = un(uni);
        rbsubs = find(refRBs(:,n,p)==0) - 1;
        if (~isempty(rbsubs))
            ksubs = bsxfun(@plus,rbsubs*12,(1:12)).';
            ksubs = ksubs(:);
            H(ksubs,n,:,p) = 0;
        end
        
    end
    
end

% ---------------------------------------------------------------------
% Averaging and interpolation in time direction
% ---------------------------------------------------------------------

% Extract time averaging parameter
timeAveraging = estConfig.AveragingWindow(2);

% For each transmit port
for p = 1:P
    
    % Establish labels 'z' for the OFDM symbols containing reference
    % symbols in each RB for this port. The labels are integers which
    % identify different OFDM symbol sets. 'z' is a column vector
    % containing one label for each RB
    [~,~,iz] = unique([zeros([1 N]); refRBs(:,:,p)],'rows','stable');
    z = iz(2:end) - 1;
    
    % Form unique set of labels 'uz', corresponding to unique OFDM
    % symbol sets, excluding the case of the empty set of OFDM symbols
    % (having label 0)
    uz = 1:max(z);
    
    % For each unique OFDM symbol set
    for zi = 1:numel(uz)
        
        % Get RBs having this OFDM symbol set
        RBs = find(z==uz(zi)) - 1;
        
        % Calculate OFDM symbol subscripts 'n' for this OFDM symbol
        % set. Also store these as 'n0' which is used to reset these
        % subscripts for each RB block / receive antenna
        n = find(refRBs(RBs(1) + 1,:,p)~=0);
        n0 = n;
        
        % Split into blocks of contiguous RBs. Each column of 'rbidx'
        % will have two rows, giving the index of the first and last RB
        % of the block within 'RBs'
        d = [-2; RBs; NRB+1];
        d = find(diff(d)~=1);
        rbidx = [d(1:end-1) d(2:end)-1].';
        
        % For each contiguous block of RBs
        for b = 1:size(rbidx,2)
            
            % Calculate frequency subscripts 'k' for the block
            k = ((RBs(rbidx(1,b))*12 + 1):(RBs(rbidx(2,b))*12 + 12)).';
            
            % For each receive antenna
            for r = 1:R
                
                % Initialize OFDM symbol subscripts 'n' to be used for
                % interpolation
                n = n0;
                
                % Extract channel estimate. 'Hrp' is a matrix of
                % denoised and frequency averaged / interpolated
                % estimates for the current RB block, port and receive
                % antenna. The rows of 'Hrp' correspond to the
                % subcarriers within the current RB block, the columns
                % to OFDM symbols containing reference symbols
                Hrp = H(k,n,r,p);
                
                % If TD-CDM despreading is configured, replace groups
                % of estimates with a single estimate in the position
                % of the average OFDM symbol index
                if (tdCDM>1)
                    Hrp = Hrp(:,1:tdCDM:end);
                    nlCDM = min(tdCDM,numel(n));
                    m = mod(numel(n),nlCDM);
                    nm = n(end-m+1:end);
                    n = reshape(n(1:end-m),nlCDM,[]);
                    n = [mean(n,1) repmat(mean(nm),1,double(m~=0))];
                end
                
                % Perform averaging in the time direction
                if (timeAveraging~=1)
                    Hrp = averaging(Hrp.',timeAveraging).';
                end
                
                % Obtain a channel estimate for all OFDM symbols. After
                % this step, the columns of 'Hrp' correspond to all
                % OFDM symbols in the slot
                if (numel(n)>1)
                    % For multiple reference OFDM symbols, perform 2-D
                    % interpolation of estimates
                    Hrp = interpolate(2,k,n,Hrp,k,1:N,'linear',0);
                else
                    % For a single reference OFDM symbol, repeat the
                    % single estimate
                    Hrp = repmat(Hrp,[1 N]);
                end
                
                % Assign the estimate into the appropriate region of
                % the overall channel estimate array
                H(k,:,r,p) = Hrp;
                
            end
            
        end
        
    end
    
end

end

% -------------------------------------------------------------------------
% local functions
% -------------------------------------------------------------------------

% Create interpolators used to create virtual pilots. 'vpis' is the set of
% interpolators with each element corresponding to the unique frequency
% subscript in the corresponding element of 'uk'. 'nd' is the number of
% dimensions over which each interpolator operates (0, 1 or 2)
function [vpis,uk,nd] = createVPInterpolators(kedge,K,eK,ksubs,lsubs,range)

% Determine the set of unique frequency subscripts
uk = unique(kedge(kedge~=0)).';

% Take extra subcarriers into account by adjusting reference symbol
% frequency subscripts
ksubs = ksubs + eK/2;

% For each unique frequency subscript
vpis = coder.nullcopy(cell([1 numel(uk)]));
nd = zeros([1 numel(uk)]);
for i = 1:numel(uk)
    
    % Determine the start and end frequency subscripts of reference
    % signals which will contribute to the interpolation performed by
    % this interpolator
    kstart = max(uk(i) + range(1),1);
    kend = min(uk(i) + range(2),K+eK);
    
    % Determine the actual reference signal subcarrier and OFDM symbol
    % subscripts ('kvp' and 'lvp') within those limits
    vpidx = ((ksubs >= kstart) & (ksubs <= kend));
    kvp = ksubs(vpidx);
    lvp = lsubs(vpidx);
    
    % If there are multiple subcarriers and/or multiple OFDM symbols,
    % construct the interpolator. Values are set to zero and will be
    % provided per receive antenna when performing interpolation. For
    % cases with a single subcarrier and a single OFDM symbol,
    % interpolation is not performed
    multiSubcarrier = (numel(unique(kvp)) > 1);
    multiSymbol = (numel(unique(lvp)) > 1);
    if (multiSubcarrier && multiSymbol)
        nd(i) = 2;
        % 2-D interpolation, record subcarrier and OFDM symbol
        % subscripts
        vpis{i} = [kvp lvp];
    elseif (multiSubcarrier) % && ~multiSymbol
        nd(i) = 1;
        % 1-D interpolation
        if (isempty(coder.target))
            % Use gridded interpolant
            vpis{i} = griddedInterpolant(kvp, ...
                zeros(size(kvp)),'linear');
        else
            % Record subcarrier subscripts
            vpis{i} = kvp;
        end
    else % ~multiSubcarrier && ~multiSymbol
        nd(i) = 0;
        % "0-D interpolation", just record the subscript of the single
        % value from which the VPs will be copied (this subscript will
        % not actually be used)
        vpis{i} = unique(kvp);
    end
    
end

end

% Calculate indices for VPs in the channel estimate across all receive
% antennas, given number of dimensions 'nd', VP interpolator 'vpi', channel
% estimate size vector 'siz' and port 'p'. For nd=0 or 1, 'n' additionally
% specifies the OFDM symbol index
function ind = calculateVPIndices(nd,vpi,siz,n,p)

K = siz(1);
N = siz(2);
R = siz(3);
if (nd==2)
    points = vpi;
elseif (nd==1)
    if (isempty(coder.target))
        points = vpi.GridVectors{1};
    else
        points = vpi;
    end
    points = [points repmat(n,size(points))];
else % nd==0
    points = [vpi n];
end
ind = points(:,1) + (points(:,2)-1)*K;
ind = bsxfun(@plus,ind,(0:R-1)*K*N);
ind = ind + (p-1)*K*N*R;

end

% Raised cosine window creation; creates a window function of length n+w
% with raised cosine transitions on the first and last 'w' samples.
function p = raised_cosine_window(n,w)

p = 0.5*(1-sin(pi*(w+1-2*(1:w).')/(2*w)));
p = [p; ones([n-w 1]); flipud(p)];

end

% Gets k,n subscripts for K-by-N-by-P grid given indices 'ind' and port
% 'port'. 'thisport' is a logical indexing vector for the port 'port', used
% to extract the corresponding reference symbols
function [ksubs,nsubs,thisport] = getPortSubscripts(K,N,P,ind,port)

[ksubs,nsubs,psubs] = ind2sub([K N P],ind(:));

thisport = (psubs==port);

ksubs = ksubs(thisport);
nsubs = nsubs(thisport);

end

% Perform averaging of columns of 'in' over 'N' samples. 'N' is expected
% to be odd
function out = averaging(in,N)

% Convolve each channel of 'in' with an N-by-1 vector of ones
h = ones([N 1]);
m = convmtx(h,size(in,1));
out = m * in;

% Retain central rows of output to make the number of rows the same as
% the input, removing other rows
M = (N - 1) / 2;
out = out(M+1:end-M,:);

% Create and apply scaling matrix 'nv' which normalizes by the number
% of samples averaged in each element of 'out'
nv = sum(m,2);
nv = nv(M+1:end-M,:);
out = bsxfun(@rdivide,out,nv);

end

% Interpolate channel estimate 'H0' defined for subcarrier subscripts 'k0'
% and OFDM symbol subscripts 'l0', using 'method' as the interpolation
% method. 'nd' is the number of dimensions over which interpolation is
% performed. 'polar' determines whether interpolation is performed using
% polar (true) or Cartesian (false) co-ordinates. 'H' contains the
% interpolated channel estimate for subcarrier subscripts 'k' and OFDM
% symbol subscripts 'n'
function H = interpolate(nd,k0,l0,H0,k,n,method,polar)

if (isempty(coder.target))
    if (nd==1)
        gi = griddedInterpolant(k0,zeros(size(H0)),method);
    else % nd==2
        gi = griddedInterpolant({k0 l0},zeros(size(H0)),method);
    end
else
    if (nd==1)
        gi = k0;
    else % nd==2
        % For code generation, interp2 is used for 2-D interpolation.
        % Unlike griddedInterpolant, interp2 does not perform
        % extrapolation. Therefore, explicit extrapolation is performed
        % here for the first and last OFDM symbol, unless they already
        % appear in 'l0'
        if (~any(n(1)==l0))
            H0 = [extrapolate(l0,H0,n(1),method) H0];
            l0 = [n(1) l0];
        end
        if (~any(n(end)==l0))
            H0 = [H0 extrapolate(l0,H0,n(end),method)];
            l0 = [l0 n(end)];
        end
        [lg,kg] = meshgrid(l0,k0);
        % For code generation, perform 2-D Cartesian interpolation.
        % This relies on the (accurate) assumption that interpolate()
        % is only called with polar=false for nd==2. The interp2 call
        % requires to be here rather than inside cartInterpolate in
        % order that coder does not attempt to generate code for
        % interp2 inside a call to createVPs, where input 'n' is scalar
        % and consequently fails size checks performed by interp2
        [yq,xq] = meshgrid(n,k);
        H = interp2(lg,kg,H0,yq,xq,method);
        return;
    end
end

if (polar)
    H = polarInterpolate(nd,gi,k,n,H0,method);
else
    H = cartInterpolate(nd,gi,k,n,H0,method);
end

end

% Extrapolates channel estimate 'H0' in Cartesian co-ordinates. Each row of
% 'H' contains the extrapolated channel estimate for OFDM subscript 'l0',
% with the corresponding row of 'H0' containing estimates for OFDM symbol
% subscripts 'l0'. 'method' is the extrapolation method
function H = extrapolate(l0,H0,l,method)

nk = size(H0,1);
H = zeros([nk 1],'like',H0);
for i = 1:nk
    H(i) = interp1(l0,H0(i,:),l,method,'extrap');
end

end

% Interpolates channel estimate 'H0' in polar co-ordinates, using
% interpolant 'ipt'. 'H' contains the interpolated channel estimate for
% subcarrier subscripts 'k' and OFDM symbol subscripts 'n'. 'nd' is the
% number of dimensions over which the interpolation is performed and
% 'method' is the interpolation method
function H = polarInterpolate(nd,ipt,k,n,H0,method)

[theta_in,r_in] = cart2pol(real(H0),imag(H0));
theta_in = unwrap(theta_in);
theta_out = cartInterpolate(nd,ipt,k,n,theta_in,method);
r_out = cartInterpolate(nd,ipt,k,n,r_in,method);
[x,y] = pol2cart(theta_out,r_out);
H = complex(x,y);

end

% Interpolates channel estimate 'H0' in Cartesian co-ordinates (or real
% values), using interpolant 'ipt'. 'H' contains the interpolated channel
% estimate for subcarrier subscripts 'k' and OFDM symbol subscripts 'n'.
% 'nd' is the number of dimensions over which the interpolation is
% performed and 'method' is the interpolation method
function H = cartInterpolate(nd,ipt,k,n,H0,method)

% If the specified method is 'VP', this indicates that the
% interpolation is being performed for the virtual pilots. In this
% case, the channel estimate 'H0' is pre-processed to give a best-fit
% linear interpolation (to remove noise) prior to linearly
% extrapolating to calculate the VPs
isVP = strcmp(method,'VP');

% Perform interpolation
if (nd==1)
    if (isempty(coder.target))
        % Pre-process VPs if applicable
        if (isVP)
            H0 = preprocessVPs(ipt.GridVectors{1},H0);
        end
        % Perform 1-D interpolation using griddedInterpolant
        ipt.Values = H0;
        H = ipt(k);
    else
        % Pre-process VPs if applicable
        if (isVP)
            H0 = preprocessVPs(ipt(:,1),H0);
            method1 = 'linear';
        else
            method1 = method;
        end
        % Perform 1-D interpolation using interp1
        H = interp1(ipt,H0,k,method1,'extrap');
    end
else % nd==2
    if (isempty(coder.target) && ~isnumeric(ipt))
        % Perform 2-D gridded interpolation using griddedInterpolant
        ipt.Values = H0;
        H = ipt({k n});
    else
        % Perform 2-D scattered interpolation
        % 'ipt' contains a sample points array for scattered
        % interpolation
        x = ipt(:,1);
        y = ipt(:,2);
        % Find the locations 'yn' of estimates for the current OFDM
        % symbol 'n'
        yn = find(y==n);
        % Pre-process VPs if applicable
        if (isVP)
            H0(yn) = preprocessVPs(x(yn),H0(yn));
        end
        % If there are multiple estimates for the current OFDM
        % symbol
        if (~isscalar(yn))
            % Interpolate the estimates for each subscript in 'k'
            H = interp1(x(yn),H0(yn),k,'linear','extrap');
        else
            % Repeat the single estimate for all subscripts 'k'
            H = repmat(H0(yn),size(k));
        end
    end
end

end

% Perform a linear fit of the channel estimate 'H' whose subcarrier
% subscripts are 'k'
function H = preprocessVPs(k,H)

if (numel(k)>1)
    p = polyfit(k,H,1);
    H = polyval(p,k);
end

end

% Create virtual pilots 'vps' using interpolant 'vpi'. 'H0' contains the
% values from which the virtual pilots are created. Virtual pilots are
% created for subcarrier subscripts 'k' and OFDM symbol subscript 'n'.
% 'nd' is the number of dimensions over which the interpolation is
% performed
function vps = createVPs(nd,vpi,k,n,H0)

if (nd==0)
    % Repeat the single estimate 'H0' for all subscripts 'k'
    vps = repmat(H0,size(k));
else % nd==1 or nd==2
    % Perform interpolation
    vps = polarInterpolate(nd,vpi,k,n,H0,'VP');
end

end

% Determine noise scaling based on CP length, reference symbol layout, and
% CDM parameters
function s = determineNoiseScaling(ofdmInfo,subn,fdCDM,tdCDM)

% Determine if extended cyclic prefix length applies
ECP = strcmpi(ofdmInfo.CyclicPrefix,'extended');

% Determine reference symbol frequency spacings 'deltas' and their
% average 'dk'. Each column of 'idx' will have two rows, giving the
% index of the first and last element of the subscripts for each OFDM
% symbol and antenna plane in 'subn'. Then for each OFDM symbol and
% antenna plane the reference symbol frequency spacing is calculated,
% and the average is taken. Note that any spacings greater than 12 are
% limited to a value of 12 to avoid skewing the average (and such large
% spacings are outside the range where the spacing influences the noise
% scaling)
idx = [[0 0]; subn(:,2:3); [0 0]];
idx = find(sum(diff(idx)~=0,2));
idx = [idx(1:end-1) idx(2:end)-1].';
deltas = [];
for i = 1:size(idx,2)
    nk = (idx(2,i)-idx(1,i)+1);
    delta = diff(subn(idx(1,i):idx(2,i),1));
    delta(delta>12) = 12;
    if (nk>1)
        deltas = [deltas; delta]; %#ok<AGROW>
    end
end
dk = mean(deltas);

% The effect of FD-CDM despreading, interpolation and CIR denoising on
% the noise variance is influenced by the FD-CDM despreading length,
% the reference symbol frequency spacing, and the cyclic prefix length.
% For sufficiently large 'dk' and/or 'fdCDM', the frequency span of the
% averaging exceeds the bandwidth of the interpolation and CIR
% denoising, and the scaling factor is a constant multiple of 'fdCDM'.
% 'log10s' contains log10 of the noise scaling factors for smaller
% values of 'dk' and 'fdCDM'. 'p1' and 'p2' are first order polynomials
% for creating the scaling factors for larger values of 'dk' with
% fdCDM=1 and fdCDM=2 respectively
if (ECP)
    %  dk:         1      2      3      4
    log10s = [0.3599 1.3548 2.9660 4.1610;  % fdCDM=1
        1.0524 1.2727      0      0]; % fdCDM=2
    p1 = [0.3157 2.9911]; % fdCDM=1
else
    %  dk:         1      2      3      4      5      6
    log10s = [0.0713 0.1654 0.2845 0.4164 0.6409 0.9259;  % fdCDM=1
        0.4633 0.6965 0.9379 1.1196 1.2059      0;  % fdCDM=2
        0.7403 1.0876 1.2757      0      0      0;  % fdCDM=3
        0.9720 1.3307 1.4028      0      0      0;  % fdCDM=4
        1.2079 1.4707 1.4990      0      0      0;  % fdCDM=5
        1.3616 1.5517 1.5961      0      0      0;  % fdCDM=6
        1.5208 1.6357 1.6470      0      0      0]; % fdCDM=7
    p1 = [0.4342 -1.7355]; % fdCDM=1
end
p2 = [0.0000 1.2727]; % fdCDM=2

% Establish if reference symbols are arranged in groups of adjacent
% resource elements. This arrangement influences the noise variance for
% fdCDM=1 and fdCDM=2, so specific scaling factors are used
if (any(fdCDM==[1 2]))
    % Get unique frequency spacings
    udk = unique(deltas);
    % If there are two unique spacings and the minimum spacing is 1
    % (adjacent resource elements)
    if (numel(udk)==2 && min(udk)==1)
        % Update 'dk' to be the other spacing and update the scaling
        % factors. Note that the first column of 'log10s' corresponds
        % to dk=1 which is not used as 'dk' is now greater than 1
        dk = max(deltas);
        if (ECP)
            log10s(1,:) = [NaN 0.6880 1.2332 1.6372];
            log10s(2,:) = [NaN 1.6140 1.9154 0.0000];
            p1 = [0.1140 1.3631];
            p2 = [0.1031 1.8078];
        else
            log10s(1,:) = [NaN 0.1087 0.1203 0.1070 0.0477 0.0090];
            log10s(2,:) = [NaN 0.0000 0.0000 0.0000 0.0000 0.0000];
            p1 = [0.1343 -0.9540];
            p2 = [0.2745 -0.0861];
        end
    end
end

% General case, constant multiple of fdCDM
s = fdCDM * 6.41;

% For tabulated fdCDM values
if (fdCDM<=size(log10s,1))
    
    % Select the tabulated entries for the configured fdCDM value
    log10s = log10s(fdCDM,:);
    log10s(log10s==0) = [];
    ndk = length(log10s);
    
    if (dk>=1 && dk<=ndk)
        % For 'dk' in the tabulated range, interpolate between the
        % tabulated entries in 'log10s'
        y = interp1(1:ndk,log10s,dk);
        s = 10^y;
    elseif (dk>ndk && any(fdCDM==[1 2]))
        % For larger 'dk' with fdCDM=1 or fdCDM=2, interpolate using
        % first order polynomial 'p'
        if (fdCDM==1)
            p = p1;
        else % fdCDM==2
            p = p2;
        end
        y = (p(1)*dk + p(2));
        s = 10^y;
    end
    
end

% The effect of TD-CDM despreading on the noise variance is simply a
% processing gain factor given by the CDM despreading length 'tdCDM'
if (tdCDM>1)
    s = s * tdCDM;
end

end

% Determine how the channel CSI will affect the receiver SNR
% (assuming MMSE equalization)
function c = determineCSIScaling(refRBs,H,nVar)

% Create a vector used to store CSI values
csi = [];

% Get number of ports 'P' in the channel estimate
P = size(H,4);

% Get the RBs (0-based) with reference symbols in any OFDM symbol (2nd
% dimension of 'refRBs') and any port (3rd dimension of 'refRBs')
RBs = find(any(refRBs,[2 3])) - 1;

% For each RB
for i = 1:numel(RBs)
    
    % Prepare logical indices 'x' giving the OFDM symbols for which the
    % current RB has reference symbols on all ports
    rb = RBs(i);
    x = all(refRBs(rb + 1,:,:),3);
    
    % Extract the channel matrix 'Hrb' from one RE of the current RB,
    % across the OFDM symbols 'x' and all receive antennas and ports
    Hrb = H((rb*12) + 6,x,:,:);
    
    % Permute 'Hrb' to give an R-by-P-by-N array where R is the number
    % of receive antennas, P is the number of ports, and N is the
    % number of active OFDM symbols
    Hrb = permute(Hrb,[3 4 2 1]);
    
    % Compute the CSI for each R-by-P channel matrix 'Hn' in 'Hrb'
    for n = 1:size(Hrb,3)
        
        Hn = Hrb(:,:,n);
        csi = [csi; svd(Hn'*Hn + nVar*eye(P)/sqrt(P))]; %#ok<AGROW>
        
    end
    
end

% Compute factor 'c' used to scale the noise estimate 'nVar', such that
% 1/(nVar*c) gives an estimate of the average receiver SNR assuming
% MMSE equalization
if (~isempty(csi) && ~any(csi==0))
    c = mean(1 ./ csi);
else
    c = ones(1,1,class(H));
end

end

% If either averaging parameter is not set, automatically choose a value
% based on the SNR (calculated from the noise variance)
function cec = setAveragingParameters(cec,nVar)

if (any(cec.AveragingWindow==0))
    
    SNR = -10 * log10(nVar);
    
    % Frequency averaging
    if (cec.AveragingWindow(1)==0)
        if (SNR>20)
            cec.AveragingWindow(1) = 1;
        elseif (SNR>7)
            cec.AveragingWindow(1) = 3;
        elseif (SNR>5)
            cec.AveragingWindow(1) = 5;
        else % SNR<=5
            cec.AveragingWindow(1) = 7;
        end
    end
    
    % Time averaging
    if (cec.AveragingWindow(2)==0)
        if (SNR>8)
            cec.AveragingWindow(2) = 1;
        else % SNR<=8
            cec.AveragingWindow(2) = 3;
        end
    end
    
end

end

% Parse optional inputs

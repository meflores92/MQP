function dciCW = nrPDCCHDecode(sym,nID,nRNTI,varargin)
%nrPDCCHDecode Physical downlink control channel decoding
%   DCICW = nrPDCCHDecode(SYM,NID,NRNTI) returns a vector of soft bits
%   DCICW resulting from the inverse of physical downlink control channel
%   processing as defined in TS 38.211 Section 7.3.2.
%   SYM is the received PDCCH symbols (complex column vector).
%   NID is the pdcch-DMRS-ScramblingID (0...65535), if configured, for a
%   UE-specific search space, else, it is the physical layer cell identity,
%   NCellID (0...1007).
%   NRNTI is the C-RNTI (0...65535) for a PDCCH in a UE specific search
%   space or 0 otherwise.
%   The two supported sets for {NID,NRNTI} are {NCellID,0} or
%   {pdcch-DMRS-ScramblingID,C-RNTI}.
%
%   DCICW = nrPDCCHDecode(...,NVAR) allows specification of the noise
%   variance estimate NVAR employed for PDCCH demodulation. When not
%   specified, it defaults to 1e-10.
%
%   % Example:
%   % Generate PDCCH symbols configured with pdcch-DMRS-ScramblingID and
%   % then recover DCI codeword using PDCCH decoder.
%
%   nID = 2^11;                 % pdcch-DMRS-ScramblingID, (0...65535)
%   nRNTI = 123;                % C-RNTI
%   nVar = 0;                   % Noise variance estimate
%   dciCW = randi([0 1],560,1); % DCI codeword
%   sym = nrPDCCH(dciCW,nID,nRNTI);
% 
%   rxDCICW = nrPDCCHDecode(sym,nID,nRNTI,nVar);
%
%   isequal(dciCW, rxDCICW<0)
%
%   See also nrPDCCH, nrPDCCHPRBS.

%   Copyright 2018 The MathWorks, Inc.

%   Reference:
%   [1] 3GPP TS 38.211, "3rd Generation Partnership Project; Technical 
%   Specification Group Radio Access Network; NR; Physical Channel and 
%   Modulation (Release 15). Section 7.3.2.

%#codegen

    narginchk(3,4);
    if nargin>3
        nVar = varargin{1};
    else
        nVar = 1e-10;
    end

    % Validate inputs
    validateInputs(sym,nID,nRNTI,nVar);

    % Section 7.3.2.4 Demodulation
    recDCICW = nrSymbolDemodulate(sym,'QPSK',nVar);
    % Section 7.3.2.3 Descrambling
    cSeq = nrPDCCHPRBS(nID,nRNTI,length(recDCICW),'MappingType','signed', ...
        'OutputDataType',class(sym));
    
    dciCW = cSeq.*recDCICW;

end


function validateInputs(sym,nID,nRNTI,nVar)
% Check inputs and parse options

    fcnName = 'nrPDCCHDecode';

    % Validate PDCCH modulation symbols
    validateattributes(sym,{'single','double'},{'column'},fcnName,'SYM');

    % Validate scrambling identity (0...65535 or 0...1007)
    validateattributes(nID, {'numeric'}, ...
        {'scalar','nonnegative','integer','<=',65535},fcnName,'NID');

    % Validate radio network temporary identifier (0...65535)
    validateattributes(nRNTI, {'numeric'}, ...
        {'scalar','nonnegative','integer','<=',65535},fcnName,'NRNTI');

    % Validate noise variance
    validateattributes(nVar, {'double','single'}, ...
        {'real','scalar','nonnegative','finite','nonnan'}, ...
        fcnName,'NVAR');

end

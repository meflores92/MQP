function [seq,cinit] = nrPDCCHPRBSOpt(cinit,n,varargin)
%nrPDCCHPRBS PDCCH scrambling sequence
%   [SEQ,CINIT] = nrPDCCHPRBS(NID,NRNTI,N) returns the N-element
%   pseudo-random binary sequence (PRBS) output SEQ from a generator that
%   is initialized by the 31-bit integer, CINIT, deduced from specified NID
%   and NRNTI, as per TS 38.211 Section 7.3.2.3.
%
%   NID is the pdcch-DMRS-ScramblingID (0...65535), if configured, for a
%   UE-specific search space, else, it is the physical layer cell identity,
%   NCellID (0...1007).
%   NRNTI is the C-RNTI (1...65519) for a PDCCH in a UE specific search
%   space or 0 otherwise.
%   The two supported sets for {NID,NRNTI} are {NCellID,0} or
%   {pdcch-DMRS-ScramblingID,C-RNTI}.
%
%   [SEQ,CINIT] = nrPDCCHPRBS(...,NAME,VALUE,...) specifies additional
%   options as NAME,VALUE pairs to allow control over the format of the
%   sequence:
%
%   'MappingType' - 'binary' to map true to 1, false to 0 (default)
%                   'signed' to map true to -1, false to 1
%
%   For 'binary' mapping, the output data type is logical. For 'signed',
%   the output data type is controlled by the additional pair:
%
%   'OutputDataType' - 'double' for double precision (default)
%                      'single' for single precision
%
%   % Example 1:
%   % Generate a 100-element sequence initialized by
%   % pdcch-DMRS-ScramblingID as 10, and C-RNTI as 20.
%
%   out1 = nrPDCCHPRBS(10,20,100);
%
%   % Example 2:
%   % Generate a 120-element sequence with NCellID as 100, and nRNTI as 0.
%
%   out2 = nrPDCCHPRBS(100,0,120);
%
%   See also nrPDCCH, nrPDCCHDecode, nrPRBS.

%   Copyright 2018-2020 The MathWorks, Inc.

%#codegen

%   Reference:
%   [1] 3GPP TS 38.211, "3rd Generation Partnership Project; Technical
%   Specification Group Radio Access Network; NR; Physical Channel and
%   Modulation (Release 15). Section 7.3.2.3.


    %cinit = mod(double(nRNTI)*2^16 + double(nID),2^31);

    % Get scrambling sequence
    seq = nrPRBS(cinit,n,varargin{:});

end
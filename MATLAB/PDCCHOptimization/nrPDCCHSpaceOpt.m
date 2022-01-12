function [allInd,allDMRS,allDMRSInd] = nrPDCCHSpaceOpt(carrier,pdcch)
%nrPDCCHSpace PDCCH resources for all candidates at all aggregation levels
%   ALLINDICES = nrPDCCHSpace(CARRIER,PDCCH) returns the physical downlink
%   control channel (PDCCH) resource element indices, ALLINDICES, as per TS
%   38.211 Section 7.3.2, for all candidates at all aggregation levels for
%   the specified carrier-specific configuration object, CARRIER, and
%   PDCCH-specific configuration object, PDCCH.
%   ALLINDICES is a five-element cell array, where each element holds the
%   PDCCH resource element indices for all the candidates at an aggregation
%   level {1,2,4,8,16}. By default, the function uses 1-based, linear
%   indexing with carrier grid as a reference. Alternative indexing formats
%   can also be used.
%
%   CARRIER is a carrier-specific configuration object, as described in
%   <a href="matlab:help('nrCarrierConfig')"
%   >nrCarrierConfig</a> with these applicable properties:
%
%   <a href="matlab:help('nrCarrierConfig/SubcarrierSpacing')"
%   >SubcarrierSpacing</a>     - Subcarrier spacing in kHz
%   <a href="matlab:help('nrCarrierConfig/CyclicPrefix')"
%   >CyclicPrefix</a>          - Cyclic prefix type
%   <a href="matlab:help('nrCarrierConfig/NSlot')"
%   >NSlot</a>                 - Slot number
%   <a href="matlab:help('nrCarrierConfig/NFrame')"
%   >NFrame</a>                - System frame number
%   <a href="matlab:help('nrCarrierConfig/NSizeGrid')"
%   >NSizeGrid</a>             - Size of the carrier resource grid in terms of
%                           number of resource blocks (RBs)
%   <a href="matlab:help('nrCarrierConfig/NStartGrid')"
%   >NStartGrid</a>            - Starting RB index of the carrier resource grid
%                           relative to common resource block 0 (CRB 0)
%
%   PDCCH is a PDCCH-specific configuration object, as described in
%   <a href="matlab:help('nrPDCCHConfig')"
%   >nrPDCCHConfig</a> with these applicable properties:
%
%   <a href="matlab:help('nrPDCCHConfig/NStartBWP')"
%   >NStartBWP</a>             - Starting RB index of the bandwidth part (BWP) 
%                           resource grid relative to CRB 0
%   <a href="matlab:help('nrPDCCHConfig/NSizeBWP')"
%   >NSizeBWP</a>              - Number of resource blocks in BWP
%   <a href="matlab:help('nrPDCCHConfig/CORESET')"
%   >CORESET</a>               - <a href="matlab:help('nrCORESETConfig')"
%   >Control resource set</a> configuration object
%   <a href="matlab:help('nrPDCCHConfig/SearchSpace')"
%   >SearchSpace</a>           - <a href="matlab:help('nrSearchSpaceConfig')"
%   >Search space set</a> configuration object
%   <a href="matlab:help('nrPDCCHConfig/RNTI')"
%   >RNTI</a>                  - Radio network temporary identifier
%   <a href="matlab:help('nrPDCCHConfig/DMRSScramblingID')"
%   >DMRSScramblingID</a>      - PDCCH DM-RS scrambling identity
%
%   [ALLINDICES,ALLDMRS,ALLDMRSINDICES] = nrPDCCHSpace(CARRIER,PDCCH) also
%   returns PDCCH demodulation reference signal (DM-RS) symbols, ALLDMRS,
%   and PDCCH DM-RS resource element indices, ALLDMRSINDICES, as per TS
%   38.211 Section 7.4.1.3, for the specified configuration objects. The
%   outputs ALLDMRS and ALLDMRSINDICES are similar five-element cells with
%   each cell representing all candidates for the specific aggregation
%   level.
%
%   [___] = nrPDCCHSpace(CARRIER,PDCCH,NAME,VALUE,...) specifies
%   additional options as NAME,VALUE pairs to allow control over the format
%   of the indices and DM-RS symbol output datatype:
%
%   'IndexStyle'       - 'index' for linear indices (default)
%                        'subscript' for [subcarrier, symbol, antenna]
%                        subscript row form
%
%   'IndexBase'        - '1based' for 1-based indices (default)
%                        '0based' for 0-based indices
%
%   'IndexOrientation' - 'carrier' for indices referenced to carrier
%                        grid (default)
%                        'bwp' for indices referenced to BWP grid
%
%   'OutputDataType'   - 'double' for double precision (default)
%                        'single' for single precision
%
%   The outputs ALLINDICES and ALLDMRSINDICES have the same indexing format
%   as specified by the index-formatting Name,Value pairs.
%   OutputDataType only applies to the DM-RS symbol output.
%
%   % Example 1:
%   % Generate the PDCCH resource element indices for all candidates at all
%   % aggregation levels for the default configurations of nrCarrierConfig
%   % and nrPDCCHConfig.
%
%   carrier = nrCarrierConfig;
%   pdcch = nrPDCCHConfig;
%   allInd = nrPDCCHSpace(carrier,pdcch);
%   size(allInd{3})         % 4 candidates at aggregation level 4
%
%   % Example 2:
%   % Generate PDCCH DM-RS symbols for all candidates at all aggregation
%   % levels for an interleaved CORESET with a duration of 3 symbols.
%
%   carrier = nrCarrierConfig;
%   pdcch = nrPDCCHConfig;
%   pdcch.NStartBWP = 6;
%   pdcch.NSizeBWP = 36;
%   pdcch.CORESET.FrequencyResources = ones(1,6);
%   pdcch.CORESET.Duration = 3;
%   pdcch.CORESET.REGBundleSize = 3;
%   pdcch.SearchSpace.NumCandidates = [5 5 3 2 1];
%
%   [~,allDMRS] = nrPDCCHSpace(carrier,pdcch);
%   size(allDMRS{5})        % 1 candidate at aggregation level 16
%
%   % Example 3:
%   % Generate PDCCH DM-RS resource element indices for all candidates at
%   % all aggregation levels using 1-based, subscript indexing form
%   % referenced to the BWP grid.
%
%   carrier = nrCarrierConfig;
%   carrier.NStartGrid = 3;
%   carrier.NSizeGrid = 60;
%   pdcch = nrPDCCHConfig;
%   pdcch.NStartBWP = 5;
%   pdcch.NSizeBWP = 48;
%   pdcch.CORESET.FrequencyResources = ones(1,6);
%   pdcch.CORESET.Duration = 3;
%   pdcch.CORESET.CCEREGMapping = 'noninterleaved';
%   pdcch.SearchSpace.NumCandidates = [5 5 3 2 1];
%
%   [~,~,allDMRSInd] = nrPDCCHSpace(carrier,pdcch,'IndexOrientation', ...
%       'bwp','IndexStyle','subscript');
%   size(allDMRSInd{2})     % 5 candidates at aggregation level 2
%
%   See also nrPDCCHConfig, nrSearchSpaceConfig, nrCORESETConfig,
%   nrPDCCHDecode, nrPDCCH, nrCarrierConfig.

%   Copyright 2019-2020 The MathWorks, Inc.

%   References:
%   [1] 3GPP TS 38.211, "3rd Generation Partnership Project; Technical
%   Specification Group Radio Access Network; NR; Physical channels and
%   modulation. Sections 7.3.2, 7.4.1.3.
%   [2] 3GPP TS 38.213, "3rd Generation Partnership Project; Technical
%   Specification Group Radio Access Network; NR; Physical layer
%   procedures for control. Sections 10, 13.
%   [3] 3GPP TS 38.331, "3rd Generation Partnership Project; Technical
%   Specification Group Radio Access Network; NR; Radio Resource Control
%   (RRC) protocol specification. Section 6.3.2, SearchSpace IE.

%#codegen

    coder.extrinsic('nr5g.internal.parseOptions');

    % Validate and parse inputs
    fcnName = 'nrPDCCHSpace';
    narginchk(2,10);
    opts = coder.const(nr5g.internal.parseOptions(fcnName, ...
        {'IndexBase','IndexStyle','OutputDataType', ...
        'IndexOrientation'}));
    dType = opts.OutputDataType;
    
    % Check for occasion and get CCEIndices for all candidates at all
    % levels for the SS, if so.
    [isOccasion,candidates] = getCandidates(carrier,pdcch);

    if isOccasion
        % Extract parameter objects
        crst = pdcch.CORESET;
        ss   = pdcch.SearchSpace;
        
        % Get REG bundles and interleaving pattern
        [regB,f] = nr5g.internal.pdcch.getREGBundles(carrier,pdcch,opts);

        aggLvls = [1 2 4 8 16];

        % Get RB indices for all candidates
        pdcchRBIdx = cell(5,1);
        % Loop over all aggregation levels
        for alIdx = 1:5
            alCand = candidates{alIdx};

            % Loop over all candidates for a aggregation level
            alpdcchRBIdx = zeros(aggLvls(alIdx)*6/crst.Duration, ...
                                 crst.Duration,ss.NumCandidates(alIdx),'uint32');
            for cIdx = 1:size(alCand,2)
                cceIndices = alCand(:,cIdx);

                if strcmp(crst.CCEREGMapping,'interleaved')
                    regBInt = regB(:,f+1);            % Interleave them
                    cce = reshape(regBInt, 6, []);    % form CCEs
                    rbCCEIdx = cce(:,cceIndices+1);   % Select CCEs
                else
                    % nonInterleaved: regB == CCE
                    cce = reshape(regB,6,[]);         % form CCEs
                    rbCCEIdx = cce(:,cceIndices+1);   % Select CCEs
                end

                % Repool the RBs, per symbol in slot, for frequency first
                % filling
                alpdcchRBIdx(:,:,cIdx) = ...
                    sort(reshape(rbCCEIdx,crst.Duration,[])'); %#ok
            end

            pdcchRBIdx{alIdx} = alpdcchRBIdx;
        end

        % Get all candidates resource element (RE) level indices for PDCCH
        % and PDCCH DM-RS
        symIdxPerRB = uint32([0 2:4 6:8 10:11])+1; % Exclude DM-RS, 1-based
        dmrsIdxPerRB = uint32([1 5 9])+1;          % DM-RS, 1-based
        tmpAllInd = cell(5,1);
        tmpAllDMRSInd = cell(5,1);
        % Loop over aggregation levels
        for alIdx = 1:length(tmpAllInd)
            rbChIdxSym = pdcchRBIdx{alIdx};

            % Loop over candidates
            alpdcchInd = zeros(aggLvls(alIdx)*6*9, ...
                               ss.NumCandidates(alIdx),'uint32');
            alpdcchDMRSInd = zeros(aggLvls(alIdx)*6*3, ...
                               ss.NumCandidates(alIdx),'uint32');
            for cIdx = 1:ss.NumCandidates(alIdx)
                %             [aggLevel*6/crst.Duration crst.Duration]
                rbChIdxSymC = rbChIdxSym(:,:,cIdx);
                rbPDCCHIdx = reshape(rbChIdxSymC,[],1);

                % Expand to RE level from RB indexes (frequency-first
                % within an RB)
                for idx = 1:length(rbPDCCHIdx)
                    % PDCCH RE indices
                    alpdcchInd((idx-1)*9+(1:9),cIdx) = ...
                        ((rbPDCCHIdx(idx)-1)*12+symIdxPerRB).';

                    % PDCCH DM-RS RE indices
                    alpdcchDMRSInd((idx-1)*3+(1:3),cIdx) = ...
                        ((rbPDCCHIdx(idx)-1)*12+dmrsIdxPerRB).';
                end
            end
            tmpAllInd{alIdx} = alpdcchInd;
            tmpAllDMRSInd{alIdx} = alpdcchDMRSInd;
        end

        % Compute DMRS symbols for all candidates
        allDMRS = cell(5,1);
        % Loop over aggregation levels
        for alIdx = 1:length(allDMRS)
            rbChIdxSym = pdcchRBIdx{alIdx};

            % Loop over candidates
            alpdcchDMRS = complex(zeros(aggLvls(alIdx)*6*3, ...
                                    ss.NumCandidates(alIdx),dType));
            for cIdx = 1:ss.NumCandidates(alIdx)
                %             [aggLevel*6/crst.Duration crst.Duration]
                rbChIdxSymC = rbChIdxSym(:,:,cIdx);

                alpdcchDMRS(:,cIdx) = nr5g.internal.pdcch.dmrsSymbols( ...
                    carrier,pdcch,rbChIdxSymC,opts);
            end
            allDMRS{alIdx} = alpdcchDMRS;
        end

        % Both indices are in 'index', '1based' format as yet
        if strcmpi(opts.IndexStyle,'subscript') || ...
                strcmpi(opts.IndexBase,'0based')
            % Handle other indexing options: 0based, subscript
            % Do this via applyIndicesOptions which takes 0-based inputs
            allInd = cell(5,1);
            allDMRSInd = cell(5,1);
            if (strcmpi(opts.IndexStyle,'subscript')) % 3D output per candidate
                if strcmpi(opts.IndexOrientation,'carrier')
                    % single antenna only
                    reGridSize = [12*uint32(carrier.NSizeGrid) carrier.SymbolsPerSlot];
                else
                    reGridSize = [12*uint32(pdcch.NSizeBWP) carrier.SymbolsPerSlot];
                end

                for alIdx = 1:5
                    alNumCand = ss.NumCandidates(alIdx);
                    alpdcchInd = zeros(aggLvls(alIdx)*6*9,3,alNumCand,'uint32');
                    alpdcchDMRSInd = zeros(aggLvls(alIdx)*6*3,3,alNumCand,'uint32');
                    for cIdx = 1:alNumCand
                        pdcchIndices = tmpAllInd{alIdx}(:,cIdx);
                        alpdcchInd(:,:,cIdx) = ...
                            nr5g.internal.applyIndicesOptions(reGridSize, ...
                                opts,pdcchIndices-1);

                        pdcchDMRSIndices = tmpAllDMRSInd{alIdx}(:,cIdx);
                        alpdcchDMRSInd(:,:,cIdx) = ...
                            nr5g.internal.applyIndicesOptions(reGridSize, ...
                                opts,pdcchDMRSIndices-1);
                    end
                    allInd{alIdx} = alpdcchInd;
                    allDMRSInd{alIdx} = alpdcchDMRSInd;
                end
            else % 0Based, 1D output per candidate
                for alIdx = 1:5
                    alNumCand = ss.NumCandidates(alIdx);
                    alpdcchInd = zeros(aggLvls(alIdx)*6*9,alNumCand,'uint32');
                    alpdcchDMRSInd = zeros(aggLvls(alIdx)*6*3,alNumCand,'uint32');
                    for cIdx = 1:alNumCand
                        pdcchIndices = tmpAllInd{alIdx}(:,cIdx);
                        alpdcchInd(:,cIdx) = pdcchIndices-1; % 0-based

                        pdcchDMRSIndices = tmpAllDMRSInd{alIdx}(:,cIdx);
                        alpdcchDMRSInd(:,cIdx) = pdcchDMRSIndices-1; % 0-based
                    end
                    allInd{alIdx} = alpdcchInd;
                    allDMRSInd{alIdx} = alpdcchDMRSInd;
                end
            end
        else % index, 1Based
            allInd = tmpAllInd;
            allDMRSInd = tmpAllDMRSInd;
        end

    else % No occasion

        % Return empties
        allDMRS = {zeros(0,1,dType); zeros(0,1,dType); zeros(0,1,dType); ...
                   zeros(0,1,dType); zeros(0,1,dType)};
        if strcmpi(opts.IndexStyle,'subscript')
            allInd = {zeros(0,3,'uint32'); zeros(0,3,'uint32'); ...
                      zeros(0,3,'uint32'); zeros(0,3,'uint32'); ...
                      zeros(0,3,'uint32')};
            allDMRSInd = allInd;
        else
            allInd = {zeros(0,1,'uint32'); zeros(0,1,'uint32'); ...
                      zeros(0,1,'uint32'); zeros(0,1,'uint32'); ...
                      zeros(0,1,'uint32')};
            allDMRSInd = allInd;
        end
    end

end

function [isOccasion,candidates] = getCandidates(carrier,pdcch)
% Off Section 10, [2].
%
% For input carrier, pdcch configs, returns the CCE indices for all
% candidates (one set of CCE indexes per slot, does not cover multiple
% symbolStarts)
%   0-based frame and slot numbers - for the current NSlot only
%   For monitoring, start from offset for duration slots
%   Assumes: nCI = 0 (carrier indicator field)

    [isOccasion,slotNum] = nr5g.internal.pdcch.isOccasion(carrier,pdcch);

    if isOccasion
        % Compute candidates for all aggregation levels
        candidates = getAllCCEIndexes(pdcch.SearchSpace,pdcch.CORESET, ...
                                   double(pdcch.RNTI),slotNum);
    else
        % not monitored, bail out
        candidates = {zeros(0,1,'uint32')};
    end
end

function candidates = getAllCCEIndexes(ssCfg,crstCfg,rnti,slotNum)
% For all candidates, return the CCE indexes (L-sized).
% Slot based, not per monitored occasion in a slot (assume only one per SS)

    nCI = 0;   % Assumes nCI = 0 (carrier indicator field)
    numCCEs = double(crstCfg.Duration)*sum(crstCfg.FrequencyResources);

    Yp = nr5g.internal.pdcch.getYp(ssCfg.SearchSpaceType,ssCfg.CORESETID,...
            rnti,slotNum);

    % Determine candidates for each aggregation level
    aggLvls = [1 2 4 8 16];
    candidates = cell(5,1);
    for i = 1:5 % for AL {1,2,4,8,16}
        MsAL = ssCfg.NumCandidates(i);
        L = aggLvls(i);

        % Store column-wise CCEIndices for each candidate
        cceIdx = zeros(L,MsAL,'uint32');
        for ms = 0:MsAL-1
            for idx = 0:L-1
                cceIdx(idx+1,ms+1) = L*( mod(Yp + ...
                    floor(double(ms*numCCEs)/double(L*MsAL)) + nCI, ...
                    floor(numCCEs/L)) ) + idx;
            end
        end
        candidates{i} = cceIdx;
    end
end

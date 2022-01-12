function [rxSym,pdcchHest] = nrExtractResourcesOpt(ind,rxCarrGrid)
%nrExtractResources Extract resource elements from a resource array
%   nrExtractResources extracts resource elements from one or more resource
%   arrays given a set of resource element indices. Resource elements can
%   be extracted from a resource array with a different dimensionality than
%   the resource array addressed by the given indices.
%
%   In 5G Toolbox(TM), indices are generated for mapping sequences of
%   physical channel and signal symbols to a resource grid. These indices
%   are generated using channel or signal specific functions and address
%   resource elements in an array sized M-by-N-by-P. M is the number of
%   subcarriers, N is the number of OFDM symbols and P is the number of
%   planes. See <a href="matlab:doc('nrExtractResources')"
%   >nrExtractResources</a> for a diagram of this concept.
%
%   Typically the resource array to extract from is one of the following:
%     - A 3D received grid, sized M-by-N-by-R. R is the number of receive
%       antennas. This is typically created after OFDM demodulation.
%     - A 4D channel estimation grid, sized M-by-N-by-R-by-P. This is
%       typically created by channel estimation functions.
%   Note that the size of a 3D received grid can be described as a 4D grid
%   with P = 1.
%
%   RE = nrExtractResources(IND,GRID) extracts resource elements RE from
%   a resource array GRID using indices IND. Resource elements are
%   extracted from each M-by-N plane within GRID using indices addressing
%   unique subcarrier and symbol locations over all planes of the indexed
%   resource array. The extracted resource elements array, RE, is sized
%   NREs-by-R-by-P. NRE is the number of resource elements extracted
%   for each M-by-N plane of GRID.
%   
%   [RE,REIND] = nrExtractResources(IND,GRID) also returns the indices of
%   extracted resource elements, REIND, within GRID. REIND is the same size
%   as the extracted resource elements array, RE.
%
%   [RE1,RE2,...,REIND1,REIND2,...] = nrExtractResources(IND,GRID1,GRID2,...)
%   extracts resource elements from multiple resource arrays using indices
%   IND.
%
%   [RE,...] = nrExtractResources(IND,GRID,...,NAME,VALUE,...) specifies
%   additional options as NAME,VALUE pairs to allow control over the format
%   of the input indices to be specified:
%
%   'IndexStyle'       - 'index' for linear indices (default)
%                        'subscript' for [subcarrier, symbol, antenna] 
%                         subscript row form
%
%   'IndexBase'        - '1based' for 1-based indices (default) 
%                        '0based' for 0-based indices
%
%   The extracted resource element indices REIND will be returned using the
%   same index style and base as input indices IND. The resource extraction
%   method can also be specified:
%
%   'ExtractionMethod' - 'allplanes' (default) or 'direct'
%
%   The 'allplanes' method (default) extracts resource elements from each
%   M-by-N plane within GRID using indices addressing unique subcarrier and
%   symbol locations over all planes of the indexed resource array. The
%   'direct' method extracts elements from each M-by-N plane (for a 3D
%   GRID) or M-by-N-by-R array (for a 4D GRID) using indices
%   addressing the corresponding plane of the indexed resource array
%   directly. See <a href="matlab:doc('nrExtractResources')"
%   >nrExtractResources</a> for diagrams showing the differences 
%   between the extraction methods.
%
%   When using the 'direct' method, the size of the extracted resource
%   element array, RE, depends on the number of indices addressing each
%   plane of the indexed resource grid:
%      - The same number address each plane, then RE is sized NRE-by-R-by-P.
%        NRE is the number of resource elements extracted from each M-by-N
%        plane of GRID.
%      - A different number address each plane, then RE is a column vector
%        containing all extracted resource elements.
%   
%   Example:
%   % Use nrExtractResources for PBCH beamforming, and to extract PBCH
%   % symbols from a received grid and associated channel estimate in
%   % preparation for decoding.
%
%   % Create symbols and indices for a PBCH transmission
%   ncellid = 17;
%   v = 0;
%   E = 864;
%   cw = randi([0 1],E,1);
%   
%   pbchTxSym = nrPBCH(cw,ncellid,v);
%   pbchInd = nrPBCHIndices(ncellid);
%
%   % Use nrExtractResources to create indices for the PBCH when beamformed
%   % onto 2 transmit antennas, and use these indices to map the beamformed
%   % PBCH into the transmitter resource array
%
%   carrier = nrCarrierConfig;
%   carrier.NSizeGrid = 20;
%   P = 2;
%   txGrid = nrResourceGrid(carrier,P);
%   F = [1 1i];
%   [~,bfInd] = nrExtractResources(pbchInd,txGrid);
%   txGrid(bfInd) = pbchTxSym * F;
%
%   % Perform OFDM modulation
%   txWaveform = nrOFDMModulate(carrier,txGrid);
%
%   % Create channel matrix and apply channel to transmitted waveform
%   R = 3;
%   H = fft(eye(max([P R])));
%   H = H(1:P,1:R);
%   H = H / norm(H);
%   rxWaveform = txWaveform * H;
%
%   % Create channel estimate including beamforming
%   hEstGrid = repmat(permute(H.'*F.',[3 4 1 2]),[240 4]);
%   nEst = 0;
%
%   % Perform OFDM demodulation
%   rxGrid = nrOFDMDemodulate(carrier,rxWaveform);
%
%   % Use nrExtractResources to extract symbols from received and channel
%   % estimate grids in preparation for PBCH decoding
%
%   [pbchRxSym,pbchHestSym] = nrExtractResources(pbchInd,rxGrid,hEstGrid);
%   figure;
%   plot(pbchRxSym,'o:');
%   title('Received PBCH constellation');
%
%   % Decode PBCH with extracted resource elements
%   pbchEqSym = nrEqualizeMMSE(pbchRxSym,pbchHestSym,nEst);
%   pbchBits = nrPBCHDecode(pbchEqSym,ncellid,v);
%   figure;
%   plot(pbchEqSym,'o:');
%   title('Equalized PBCH constellation');
%
%   See also nrEqualizeMMSE.

%   Copyright 2018-2020 The MathWorks, Inc.

%#codegen

    coder.extrinsic('nr5g.internal.parseOptions');

    % Parse options and gather grid inputs
    fcnName = 'nrExtractResources';
    opts = coder.const(nr5g.internal.parseOptions(fcnName, ...
        {'IndexStyle','IndexBase','ExtractionMethod'}));
   

    % Ensure indices are 1-based for processing
    if (strcmpi(opts.IndexBase,'0based'))
        ind = ind + 1;
    end
    
    % Validate that indices are positive and finite
%     validateattributes(ind,{'numeric'}, ...
%         {'positive','finite'},fcnName,'IND');
%     if (strcmpi(opts.IndexStyle,'subscript'))
%         % Validate that subscript form has 3 columns
%         validateattributes(ind,{'numeric'},{'ncols',3},fcnName,'IND');
%     end

    % Extract REs from a grid at a time by transforming indices as
    % appropriate
    ngrids = 1;
    out = cell(1,2*ngrids); % Preallocate output
    for i = 1:ngrids
        grid = rxCarrGrid;
        gSize = size(grid);
        M = gSize(1); % Number of subcarriers
        N = gSize(2); % Number of OFDM symbols
        if strcmpi(opts.ExtractionMethod,'allplanes')
            % If allplanes method is used, project over the P and R
            % dimensions
            projDims = [size(grid,4) size(grid,3)];
        else
            % Otherwise project only over the R dimension
            projDims = size(grid,3);
        end

        % Validate subcarrier and OFDM symbol dimensions if indices are
        % not empty
%         coder.internal.errorIf(~isempty(ind) && (M*N)==0, ...
%             'nr5g:nrExtractResources:indexExceedsDimsLinear',M*N,i);
        
        % Convert between indexing types (subscript and linear) and
        % calculate number of indexed planes, P

        if strcmpi(opts.IndexStyle,'subscript')
            P = max(ind(:,3));
            subIndTmp = double(ind); % [M,N,P] indices
            linInd = sub2ind([M,N,P],ind(:,1),ind(:,2),ind(:,3));
        else
            P = max(ceil(double(ind(:))/(M*N)));
            linInda = double(ind);
            [subM,subN,subP] = ind2sub([M,N,P],linInda(:));
            subIndTmp = [subM,subN,subP]; % [M,N,P] indices
        end
        P(isempty(P)) = 1; % Force P to be 1 if no planes detected

        % If a 3D grid and direct extraction method, use unaltered indices
        if (ndims(grid) < 4 && strcmpi(opts.ExtractionMethod,'direct'))
            indOut = ind;

        % Otherwise project the indices using subscripts
        else
            % If allplanes, project only unique time-frequency indices
            if strcmpi(opts.ExtractionMethod,'allplanes')
                subIndIn = unique(subIndTmp(:,1:2),'rows','stable');
            else
                subIndIn = subIndTmp;
            end
            indPerR = size(subIndIn,1); % Number of indices per rx antenna

            % Index of first dimension projected
            projStartIdx = size(subIndIn,2)+1;

            % Allocate output subscript indices 
            subIndOut = [subIndIn zeros(indPerR,numel(projDims))];

            % Determine shape and size of output
            if strcmpi(opts.ExtractionMethod,'direct') && (ndims(grid)>3)
            % Direct mode, determine output shape given number of indices
            % per plane. If they differ return a column vector.
                indPerP = sum(repmat(1:P,indPerR,1) == repmat(subIndOut(:,3),1,P));
                if (max(indPerP)-min(indPerP)) == 0
                    % NRE-by-R-by-P
                    sizeOut = [indPerR/P,P,projDims];
                    sizeOut([2 3]) = sizeOut([3 2]);
                else
                    % NNRETotal-by-1-by-1
                    sizeOut = [indPerR*prod(projDims),1,1];
                end
            else
            % Allplanes mode always have same number of elements on each plane
                sizeOut = [indPerR,projDims,1];
                sizeOut([2 3]) = sizeOut([3 2]);
            end

            % Project indices over receive antennas and higher dimensions
            subIndOut = repmat(subIndOut,prod(projDims),1);

            % Add dimensions projecting over
            addedInd = repmat(1:prod(projDims),indPerR,1);
            if strcmpi(opts.ExtractionMethod,'allplanes')
                [subP,subR] = ind2sub(projDims,addedInd(:));
                subIndOut(:,projStartIdx:end) = [subP subR];
            else
                subR = ind2sub(projDims,addedInd(:));
                subIndOut(:,projStartIdx) = subR;
            end

            % Reorder for M-by-N-by-R-by-P
            subIndOut(:,[3 4]) = subIndOut(:,[4 3]);

            % Order indices to ascend by dimension to allow reshaping but
            % maintain M-N order (ignore subscript indices 1 and 2)
            [~,sortIdx] = sortrows(subIndOut(:,end:-1:3));
            subIndOut = subIndOut(sortIdx,:);

            % Convert subscript to linear indices and reshape
            % (M-by-N-by-R)
            linInd = reshape(sub2ind(size(grid),subIndOut(:,1),subIndOut(:,2),subIndOut(:,3),subIndOut(:,4)),sizeOut(1:3));

            % Output linear or subscript indices
            if strcmpi(opts.IndexStyle,'subscript')
                indOut = subIndOut(:,1:ndims(grid));
            else
                indOut = linInd;
            end
        end

        % Validate linear indices
%         nElements = numel(grid);
%         coder.internal.errorIf(any(linInd(:) > nElements), ...
%             'nr5g:nrExtractResources:indexExceedsDimsLinear',nElements,i);
        
        % Extract REs with linear indices
        out{i} = grid(linInd);

        % Convert indices to 0-based from 1-based used for processing
        if strcmpi(opts.IndexBase,'0based')
            indOut = indOut - 1;
        end

        % Return indices of same class as input indices
        out{i+ngrids} = cast(indOut,'like',ind);

    end
    [rxSym,pdcchHest] = out{1:max(nargout,1)};

end

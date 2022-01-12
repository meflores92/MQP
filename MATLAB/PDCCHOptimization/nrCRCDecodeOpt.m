function [blk,err] = nrCRCDecodeOpt(blkcrc,poly,mask)
%nrCRCDecode Cyclic redundancy check decoding and removal
%   [BLK,ERR] = nrCRCDecode(...) checks the input data block for a CRC
%   error assuming the data block comprises a block of data with the
%   associated CRC bits attached. The data part of the input is returned in
%   BLK. The logical difference (xor) between the attached CRC and the CRC
%   recalculated across the data part of the input is returned in uint32
%   scalar ERR. If ERR is not equal to 0, either an error has occurred or
%   the input CRC has been masked. A logical mask can also be applied
%   directly to ERR.
%
%   [BLK,ERR] = nrCRCDecode(BLKCRC,POLY) returns BLK, the data only part of
%   the combined data and CRC input BLKCRC, and uint32 ERR, the logical
%   (xor) CRC difference. BLKCRC is a matrix (double, int8 or logical)
%   where each column is treated as a separate data block and processed
%   independently. The CRC polynomial is defined by a value from the set
%   ('6','11','16','24A','24B','24C'). See TS 38.212 Section 5.1 for the
%   associated polynomials.
%
%   [BLK,ERR] = nrCRCDecode(BLKCRC,POLY,MASK) behaves as above except the
%   CRC difference is also XORed with the scalar nonnegative integer MASK
%   parameter before it is returned in ERR.
%
%   Example:
%   % This example shows the effect of CRC decoding a block of
%   % data with and without a mask. When decoding without a mask, ERR is
%   % equal to the RNTI because the CRC had been masked during coding.
%   % Hence, the logical difference between the original CRC and the
%   % recalculated CRC is the CRC mask. CRC decoding using the RNTI as a
%   % mask results in zero ERR.
%
%   % CRC encode an all-ones vector and mask with RNTI
%   RNTI = 12;
%   blkCrc = nrCRCEncode(ones(100,1),'24C',RNTI);
%
%   % CRC decode the result without using a mask, ERR should be the RNTI
%   [blk,err1] = nrCRCDecode(blkCrc,'24C');
%   err1
%
%   % CRC decoding using the RNTI as a mask, ERR should be zero
%   [blk,err2] = nrCRCDecode(blkCrc,'24C',RNTI);
%   err2
%
%   See also nrCRCEncode, nrCodeBlockDesegmentLDPC, nrPolarDecode,
%   nrLDPCDecode, nrRateRecoverPolar, nrRateRecoverLDPC, nrBCHDecode,
%   nrDCIDecode.

%   Copyright 2018 The MathWorks, Inc.

%#codegen


    % Validate inputs and get properties
    polyIndex = nr5g.internal.validateCRCinputs(blkcrc,poly,mask,'nrCRCDecode');
    polyLengths = [6 11 16 24 24 24];
    gLen = 0; % Initialize for codegen
    gLen(:) = polyLengths(polyIndex);

    % Perform cyclic redundancy check for data only part of blkcrc
    reEncodedBlk = nrCRCEncode(blkcrc(1:end-gLen,:),poly,mask);
    [codeLen,numCodeBlocks] = size(blkcrc);
    if isempty(blkcrc)
        blk = zeros(0,numCodeBlocks,class(blkcrc));
        err = zeros(0,numCodeBlocks,'uint32');
    else
        blk = reEncodedBlk(1:end-gLen,:);
        if codeLen <= gLen
            % For input length less than parity bit length
            blkcrcL = [false(gLen-codeLen,numCodeBlocks); blkcrc>0];
            if mask
                maskBits = comm.internal.utilities.de2biBase2LeftMSB( ...
                    double(mask),gLen)';
                errBits = xor(blkcrcL,repmat(maskBits>0,[1 numCodeBlocks]));
            else
                errBits = blkcrcL;
            end
        else
            errBits = xor(reEncodedBlk(end-gLen+1:end,:)>0, ...
                blkcrc(end-gLen+1:end,:));
        end
        err = uint32(sum(double(errBits).*repmat((2.^(gLen-1:-1:0)'), ...
            [1 numCodeBlocks])));
    end

end

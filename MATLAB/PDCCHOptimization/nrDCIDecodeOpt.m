function [dciBits,mask] = nrDCIDecodeOpt(dciCW,Kout,L,rnti)
%nrDCIDecode Downlink control information decoding
%   DCIBITS = nrDCIDecode(DCICW,K,L) decodes the input soft bits, DCICW, as
%   per TS 38.212 Sections 7.3.4, 7.3.3 and 7.3.2 to output the decoded DCI
%   bits, DCIBITS of length K. The processing includes rate recovery, polar
%   decoding and CRC decoding.
%   L is the specified list length used for polar decoding.
%   The input DCICW must be a column vector of soft bits (LLRs) and the
%   output DCIBITS is the output DCI message of length K.
%
%   [DCIBITS,MASK] = nrDCIDecode(...) also outputs the masked value. MASK
%   equals the RNTI value when there is no CRC error for the decoded block.
%
%   [DCIBITS,MASK] = nrDCIDecode(...,RNTI) also accepts an RNTI value that
%   may have been used at the transmit end for masking. When an RNTI is
%   specified, MASK equals 0 when there is no CRC error for the decoded
%   block.
%
%   % Example:
%   % Decode an encoded DCI block and check the recovered information
%   % elements.
%
%   K = 32;
%   RNTI = 100;
%   E = 240;
%   L = 8;
%   dciBits = randi([0 1],K,1);
%   dciCW   = nrDCIEncode(dciBits,RNTI,E);
%   [recBits,mask] = nrDCIDecode(1-2*dciCW,K,L,RNTI);
%   isequal(recBits,dciBits)
%   mask
%
%   See also nrDCIEncode, nrPDCCHDecode, nrPDCCH.

%   Copyright 2018-2019 The MathWorks, Inc.


%   Reference:
%   [1] 3GPP TS 38.212, "3rd Generation Partnership Project; Technical
%   Specification Group Radio Access Network; NR; Multiplexing and channel
%   coding (Release 15). Section 7.3.

% 
%     hard = [];
%     for x = 1:length(dciCW)
%         if dciCW(x) >= 0
%             hard(x) = 1;
%         else
%             hard(x) = -1;
%         end
%     end

    E = length(dciCW);
    K = Kout+24;            % K includes CRC bits
    nMax = 9;               % for downlink

    N = nr5g.internal.polar.getN(K,E,nMax);
    % Rate recovery, Section 7.3.4, [1]
    recBlk = nrRateRecoverPolarOpt(dciCW,K,N);
    % Polar decoding, Section 7.3.3, [1]
    %padCRC = true;              % signifies input prepadding with ones
    

    decBlk = nrPolarDecodeOpt(recBlk,K,E);

    
    %decBlk = nrPolarDecode(recBlk,K,E,L,true,rnti);

    % CRC decoding, Section 7.3.2, [1]
    [padDCIBits,mask] = nrCRCDecode([ones(24,1);decBlk],'24C',rnti);
    dciBits = cast(padDCIBits(25:end,1),'int8'); % remove the prepadding
end



%     [F,qPC] = nr5g.internal.polar.construct(K,E,9);
%     q_info_list = find(F == 0)-1;
%     
%     [dciBits, mask] = uci_decode(hard, K, N, E, 0, 1, q_info_list, [], 24, Kout);





% 
%     [~, ~, g] = get_crc_objective(24);
%     
%     [G_crc, H_crc] = crc_generator_matrix(g, Kout);
%     psi_vec = get_psi_for_advanced_sc_decoder(node_type_matrix);
%     llr = recBlk; 
%     L = 8; %Guess
%     info_bits = find(F == 0); %Might be Kout
%     lambda_offset = 2.^(0 : log2(N));
%     llr_layer_vec = get_llr_layer(N);
%     bit_layer_vec = get_bit_layer(N);
%     
%     
%     tic
%     polar_info_esti = FastSCL_decoder(llr, L, info_bits, lambda_offset, llr_layer_vec, bit_layer_vec, psi_vec, node_type_matrix, H_crc);
%     toc
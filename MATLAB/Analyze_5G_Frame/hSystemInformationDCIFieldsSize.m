%hSystemInformationDCIFieldsSize size in bits of DCI Format 1_0 fields

%   Copyright 2020 The MathWorks, Inc.

function dcispec1_0 = hSystemInformationDCIFieldsSize(NSizeBWP)

    dcispec1_0 = struct();
    dcispec1_0.RIV = ceil(log2(NSizeBWP*(NSizeBWP+1)/2));
    dcispec1_0.TDDIndex = 4;
    dcispec1_0.VRBToPRBInterleaving = 1;
    dcispec1_0.ModCoding = 5;
    dcispec1_0.RV = 2;
    dcispec1_0.SIIndicator = 1;
    dcispec1_0.Reserved = 15;
    
end
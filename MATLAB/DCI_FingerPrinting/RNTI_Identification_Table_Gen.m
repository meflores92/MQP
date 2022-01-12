


function RNTI_ID_MATRIX = RNTI_Identification_Table_Gen(num, nID)

K = 39;             % Number of DCI message bits
dciBits = randi([0 1],K,1,'int8');
RNTI_ID_MATRIX = zeros((num+1), length(nID));
dciCW = nrDCIEncode(dciBits,0,432);

jn = nr5g.internal.polar.subblockInterleaveMap(512);
cw =0;
idx = zeros(1, 32);
idx1 = zeros(1, 32);
idx2 = zeros(1, 32);
idx3 = zeros(1, 32);
for x = 80:111
idx(x-79) = find(jn == x) - 80;
idx1(x-79) = find(jn == x+256) - 80;
idx2(x-79) = find(jn == x+128) - 80;
idx3(x-79) = find(jn == x+384)- 80;
end
for j = 1: length(nID)
    for x = 0:num
        RNTI_ID_MATRIX(x+1, j) = decodeAccuracy(x, nID(j), dciCW, idx, idx1, idx2, idx3, cw);
    end
end
end

function CodeWord = decodeAccuracy(rnti, j, dciCW, idx, idx1, idx2, idx3, CodeWord)



% Get scrambling sequence

%cSeq = nrPDCCHPRBS(0,rnti,length(dciCW));

data = xor(dciCW,nrPRBS(mod(double(rnti)*2^16 + double(j),2^31),432));

%data(end-431:end) = xor(dciCW,nrPRBS(mod(double(rnti)*2^16 + double(0),2^31),432));
%out(jn+1) = data;

for x = 1:32
    if ~(xor(xor(data(idx(x)), data(idx1(x))), xor(data(idx2(x)), data(idx3(x)))))
        CodeWord = CodeWord + 2^(x-1);
    end 
end

%CodeWord= bi2de((xor(xor(data(idx(1:32)), data(idx1(1:32))), xor(data(idx2(1:32)), data(idx3(1:32)))) == 0).');
%CodeWord=comm.internal.utilities.bi2de((cast((xor(xor(data(idx(1:32)), data(idx1(1:32))), xor(data(idx2(1:32)), data(idx3(1:32)))) == 0), 'double').'), 2, 'right-msb', 'logical');

end
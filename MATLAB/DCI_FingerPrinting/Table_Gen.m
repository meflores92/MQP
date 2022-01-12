% total = 0;
% for x = 1:30
% tic
% RNTI_ID_MATRIX = RNTI_Identification_Table_Gen_mex(65519,0);
% a = toc;
% total = total +a;
% end
% total/30


% RNTI_ID_MATRIX = RNTI_Identification_Table_Gen_mex(65519,0);
% save('RNTI_TABLE', 'RNTI_ID_MATRIX');

% tic
% nID = 0:16380;
% RNTI_ID_MATRIX = RNTI_Identification_Table_Gen_mex(65519, nID);
% save('RNTI_ID_MATRIX.mat', 'RNTI_ID_MATRIX', '-v7.3');
% toc
% clear


% tic
% nID = 16381:32760;
% RNTI_ID_MATRIX = RNTI_Identification_Table_Gen_mex(65519, nID);
% save('RNTI_ID_MATRIX1.mat', 'RNTI_ID_MATRIX', '-v7.3');
% toc
% clear

% 
% tic
% nID = 32761:49140;
% RNTI_ID_MATRIX = RNTI_Identification_Table_Gen_mex(65519, nID);
% save('RNTI_ID_MATRIX2.mat', 'RNTI_ID_MATRIX', '-v7.3');
% toc
% clear
% 
% 
tic
nID = 49141:65519;
RNTI_ID_MATRIX = RNTI_Identification_Table_Gen_mex(65519, nID);
save('RNTI_ID_MATRIX3.mat', 'RNTI_ID_MATRIX', '-v7.3');
toc
clear


% max = 0;
% for x = 1:65519
%     value = RNTI_ID_MATRIX(x);
%     count = 0;
%     for i = 1:(65519)
%         if RNTI_ID_MATRIX(i) == value
%             count = count +1;
%         end
%     end
%     if count > max
%         max = count;
%     end
% end
% max

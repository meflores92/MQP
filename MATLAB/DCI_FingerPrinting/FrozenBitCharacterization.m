%This program confimrs the PolarPairsMatrix algorithm

rng(211);           % Set RNG state for repeatability

nID = 23;           % pdcch-DMRS-ScramblingID
rnti = 100;         % C-RNTI for PDCCH in a UE-specific search space
K = 64;             % Number of DCI message bits
E = 288;            % Number of bits for PDCCH resources
nMax = 9;

dciBits = randi([0 1],K,1,'int8');
        
out = nrPolarEncode(dciBits,E);

pi = nr5g.internal.polar.interleaveMap(K);
dciBits = dciBits(pi+1);

[F,qPC] = nr5g.internal.polar.construct(K,E,nMax);

u = zeros(512,1);
u(F==0) = dciBits;

idx = find(F);
F(F==0) = dciBits;
for x = 1:length(idx)
    F(idx(x)) = 0;
end


%u = F;
newF = zeros(length(F),1);
interval = 2;
step = 1;
ID = -2;
while (length(F)-interval) >= 0
    for x = 0:interval:(length(F)-interval)
        for i = 1:step
            newF(i+x) = xor(F(i+x), F(i+x+(interval/2)));
            newF(i+x+(interval/2)) = F(i+x+(interval/2));
%             if(F(i+x) == 0 && F(i+x+(interval/2)) == 0)
%                newF(i+x) = xor(F(i+x), F(i+x+(interval/2)));
%                newF(i+x+(interval/2)) = F(i+x+(interval/2));
%             elseif(F(i+x) == 0)
%                 newF(i+x) = F(i+x+(interval/2));
%                 newF(i+x+(interval/2)) = F(i+x+(interval/2));
%             elseif(F(i+x+(interval/2)) == 0)
%                 newF(i+x) = F(i+x);
%                 newF(i+x+(interval/2)) = F(i+x+(interval/2));
%             elseif(F(i+x) == F(i+x+(interval/2)) && F(i+x) ~= -1)
%                 newF(i+x) = 0;
%                 newF(i+x+(interval/2)) = F(i+x+(interval/2));
%             else
%                 newF(i+x) = ID;
%                 ID = ID -1;
%                 newF(i+x+(interval/2)) = F(i+x+(interval/2));
%             end
        end
    end
    F = newF;
    step = step*2;
    interval = interval*2;
%     256 - length(find(F))
%     pairs = [];
%     for x = ID:-1
%         count = 0;
%         for i = 1:length(F)
%             if F(i) == x
%                 count = count + 1;
%             end
%         end
%         if(count > 1)
%             pairs = [pairs, count];
%         end
%     end
%     length(pairs)
end







% Get G, nth Kronecker power of kernel
N = length(F);
n = log2(N);
ak0 = [1 0; 1 1];   % Arikan's kernel
allG = cell(n,1);   % Initialize cells
for i = 1:n
    allG{i} = zeros(2^i,2^i);
end
allG{1} = ak0;      % Assign cells
for i = 1:n-1
    allG{i+1} = kron(allG{i},ak0);
end
G = allG{n};

% Encode using matrix multiplication
%out = mod(u'*G,2)';

error = 0;

for x = 1:length(out)
    if out(x) ~= F(x)
        error = error+1;
    end
end
error
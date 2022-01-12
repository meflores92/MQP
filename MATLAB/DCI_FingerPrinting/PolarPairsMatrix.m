% This program generates a matrix showing the intermediate steps between 
% Each XOR operation in the polar encoding matrix

rng(211);           % Set RNG state for repeatability

nID = 23;           % pdcch-DMRS-ScramblingID
rnti = 100;         % C-RNTI for PDCCH in a UE-specific search space
K = 140;             % Number of DCI message bits
E = 432;            % Number of bits for PDCCH resources
nMax = 9;

dciBits = randi([0 1],K,1,'int8');

[F,qPC] = nr5g.internal.polar.construct(K,E,nMax);

idx = find(F);
ID = -1;
for x = 1:length(F)
    if F(x) == 0
        F(x) = ID;
        ID = ID-1;
    end
end
for x = 1:length(idx)
    F(idx(x)) = 0;
end
ID = ID-1;
newF = zeros(length(F),1);
interval = 2;
step = 1;
Matrix = [];
Matrix(1:length(F), 1) = F;
PairsMatrix = [];
iteration = 2;
while (length(F)-interval) >= 0
    for x = 0:interval:(length(F)-interval)
        for i = 1:step
            if(F(i+x) == 0 && F(i+x+(interval/2)) == 0)
               newF(i+x) = xor(F(i+x), F(i+x+(interval/2)));
               newF(i+x+(interval/2)) = F(i+x+(interval/2));
            elseif(F(i+x) == 0)
                newF(i+x) = F(i+x+(interval/2));
                newF(i+x+(interval/2)) = F(i+x+(interval/2));
            elseif(F(i+x+(interval/2)) == 0)
                newF(i+x) = F(i+x);
                newF(i+x+(interval/2)) = F(i+x+(interval/2));
            elseif(F(i+x) == F(i+x+(interval/2)) && F(i+x) ~= -1)
                newF(i+x) = 0;
                newF(i+x+(interval/2)) = F(i+x+(interval/2));
            else
                newF(i+x) = ID;
                ID = ID -1;
                newF(i+x+(interval/2)) = F(i+x+(interval/2));
            end
        end
    end
    F = newF;
    Matrix(1:length(F), iteration) = F;
    step = step*2;
    interval = interval*2;
    iteration = iteration+1;
    pairs = [];
    for x = ID:-1
        count = 0;
        for i = 1:length(F)
            if F(i) == x
                count = count + 1;
            end
        end
        if(count > 1)
            pairs = [pairs, count];
        end
    end
    length(pairs);
end
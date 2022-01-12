

K = 62;  % Info bits
E = 200; % Output length
L = 256;

encOut = [ones(L/2, 1); zeros(L/2,1)];

dciCW = nrRateMatchPolar(encOut,K,E);

out = nrRateRecoverPolar(dciCW, K, L);
% hSSBurstSubcarrierSpacing subcarrier spacing of SS block pattern

%   Copyright 2020 The MathWorks, Inc.

function scs = hSSBurstSubcarrierSpacing(blockPattern)
%# codgen
    if (strcmpi(blockPattern,"Case A"))
        scs = 15;
    elseif (strcmpi(blockPattern,"Case B"))
        scs = 30;
    elseif (strcmpi(blockPattern,"Case C"))
        scs = 30;
    elseif (strcmpi(blockPattern,"Case D"))
        scs = 120;
    elseif (strcmpi(blockPattern,"Case E"))
        scs = 240;
    else
        scs = 30;
    end

end
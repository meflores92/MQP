global maxListDepth;
maxListDepth = 8;
global accumMetric;
global decisionStep;
function [s, list] = recurseNodeNatural8(llr, f)
% llr = input LLRs (8-by-CODEWORD_SIZE. If the input LLRs are (1-by-CODEWORD_SIZE) then use repmat(llrs, 8, 1)
% f = array indicating frozen bits. f(n)==1 to indicate that bit#n is frozen.
% s = output msg
% l = output LLRs.
%Stop recursions at node#0 (length-1)
N = size(llr, 2);
if (N==1)
    if (f == 0)
        %-ve LLR -> 1, +ve LLR -> 0
        %Decisions with no metric growth
        st = (llr <= 0);
        %metric for correct decision
        mt = zeros(size(llr));
        %Decisions with some metric growth
        sf = (llr > 0);
        %metric for forced error
        mf = abs(llr);
        %These if-elses need to be extended for list 16, 32, etc.
        %This example is for list 8.
        %This builds up all possible decisions in the first 3 unfrozen
        %branches, to fill up the list depth of 8.
        if (decisionStep == 0)
            s = [st(1:4); sf(5:8);];
            accumMetric = accumMetric + [mt(1:4); mf(5:8);];
            decisionStep = 1;
            list = 1:maxListDepth;
        elseif (decisionStep == 1)
            s = [st(1:2); sf(3:4); st(5:6); sf(7:8);];
            accumMetric = accumMetric + [mt(1:2); mf(3:4); mt(5:6); mf(7:8);];
            decisionStep = 2;
            list = 1:maxListDepth;
        elseif (decisionStep == 2)
            s = [st(1); sf(2); st(3); sf(4); st(5); sf(6); st(7); sf(8);];
            accumMetric = accumMetric + [mt(1); mf(2); mt(3); mf(4); mt(5); mf(6); mt(7); mf(8);];
            decisionStep = 3;
            list = 1:maxListDepth;
        else
            tmp = [accumMetric+mt; accumMetric+mf;];
            %Call the pruning function
            idx = medianMetricPrune(tmp);
            %assemble the possible symbols
            tmps = [st; sf;];
            s = tmps(idx);
            %Accumulate the metric
            accumMetric = tmp(idx);
            %Maintain a metric of zero at the top of the list
            accumMetric = accumMetric - min(accumMetric);
            %Update the list of which paths were followed.
            list = idx;
            %From the structure of tmp, entry (Z+maxListDepth) is path#Z,
            %etc.
            list(list > maxListDepth) = list(list > maxListDepth) - maxListDepth;
        end
    else
        %Frozen bit
        s = zeros(size(llr));
        %Update the metrics. The frozen bit is known to be 0 (-ve LLR)
        %so increase the metric where the LLR is +ve
        accumMetric = accumMetric + abs(llr) .* (llrs <= 0);
        %No list pruning necessary
        list = 1:maxListDepth;
    end
else
    %Compute prior-LLRs backwards (no knowledge of bits)
    idx1 = 1:N/2;
    idx2 = idx1 + N/2;
    llr1 = llr_prior(llr(:, idx1), llr(:, idx2));
    %Upper (left) branch
    [s1, list1] = recurseNodeNatural8(llr1, f(1:(N/2)), metricPruneHandle, metricHandle);
    %Got the decisions, so update LLRs based on these
    llr2 = llr_posterior(s1, llr(list1, idx1), llr(list1, idx2) );
    %lower (right) branch
    [s2, list2] = recurseNodeNatural8(llr2, f((N/2+1):end), metricPruneHandle, metricHandle);
    %These decisions go back up one node.
    list = list1(list2);
    %Modulo-2 (EXOR) addition
    s = [mod(s1(list2,:) + s2, 2) s2];
end
end

function z = llr_prior(w1,w2)
global maxListDepth;
for n=1:maxListDepth
    z(n,:) = sign(w1(n,:).*w2(n,:)) .* min(abs([w1(n,:); w2(n,:);]), [], 1);
end
end

function z = llr_posterior(s, w1, w2)
global maxListDepth;
for n=1:maxListDepth
    z(n,:) = (1-2*s(n,:)).*w1(n,:) + w2(n,:);
end
end

%Metric prune function: median threshold is equivalent to "best-N"
function idx = medianMetricPrune(metricsIn)
global maxListDepth;
%Sort for best metrics
[~, idx] = sort(metricsIn, 'ascend');
medianThresh = 0.5*(metricsIn(idx(maxListDepth)) + metricsIn(idx(maxListDepth+1)));
idx = find(metricsIn <= medianThresh, maxListDepth, 'first');
end
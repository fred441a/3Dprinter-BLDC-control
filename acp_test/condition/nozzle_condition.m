nBad = readtable('con_badNozzle.csv','PreserveVariableNames',true);
volnBad = nBad.voltage;
timenBad = (nBad.absTime)/1000000;
tsnBad = timenBad - timenBad(1);
dBad = [volnBad, tsnBad];

nWorst = readtable('con_WorstNozzle.csv','PreserveVariableNames',true);
volnWorst = nWorst.voltage;
timenWorst = (nWorst.absTime)/1000000;
tsnWorst = timenWorst - timenWorst(1);
dWorst = [volnWorst, tsnWorst];

datanGood = readtable('con_goodNozzle.csv','PreserveVariableNames',true);
volnGood = datanGood.voltage;
timenGood = (datanGood.absTime)/1000000;
tsnGood = timenGood - timenGood(1);
dGood = [volnGood, tsnGood];

u_nBad = mean(volnBad);
sig_nBad = std(volnBad);

u_nWorst = mean(volnWorst)
sig_nWorst = std(volnWorst)

u_nGood = mean(volnGood)
sig_nGood = std(volnGood)

hist2(dGood, dWorst, 0.01, 1.4)

%print(dWorst)
function print(sys)
    plot(sys(:,2), sys(:,1))
    xlim([sys(1,2) sys(end,2)])
    xlabel('Time [s]');
    ylabel('Voltage Value');
    title('Voltage value over time for Good nozzle');
grid on
end


function hist2(data1, data2, binWidth, overflowEdge)
data1 = data1(:,1);
    data2 = data2(:,1);

    % Remove NaNs / Infs
    data1 = data1(isfinite(data1));
    data2 = data2(isfinite(data2));

    figure;

    % Bin edges with overflow bin
    minEdge = min([data1; data2]);
    edges = minEdge:binWidth:overflowEdge;
    edges(end+1) = Inf;

    % Plot normalized histograms
    histogram(data1, edges, ...
        'Normalization','probability', ...
        'FaceColor','b', 'FaceAlpha',0.7, ...
        'DisplayName','Good nozzle');
    hold on;

    histogram(data2, edges, ...
        'Normalization','probability', ...
        'FaceColor','r', 'FaceAlpha',0.7, ...
        'DisplayName','Discarded Nozzle');
    hold off;

    xlabel('Voltage value');
    ylabel('Normalised density');
    title('Normalized Histogram for a Good and a Discarded nozzle');
    legend('show');
    grid on;
end


function showHis(data)
binWidth = 0.005;
overflow = 1.4;
edges = [min(data(:)) : binWidth : overflow, inf];
histogram(data(:), edges, 'Normalization', 'probability')
    xlabel('Devincies');
    ylabel('Density');
    title('Normalized Histogram');
end

function printMuch(u1,s1, u2,s2)

% Example means and standard deviations
u_1 = u1;  s_1 =s1;
u_2 = u2;  s_2 = s2;

% Create a common x-axis range that covers all curves
x = linspace(min([u_1-3*s_1, u_2-3*s_2]), max([u_1+3*s_1, u_2+3*s_2]), 500);

% Compute the probability density functions
y1 = (1/(s_1*sqrt(2*pi))) * exp(-0.5*((x-u_1)/s_1).^2);
y2 = (1/(s_2*sqrt(2*pi))) * exp(-0.5*((x-u_2)/s_2).^2);


% Plot all three curves
plot(x, y1, 'r', 'LineWidth', 2); hold on;
plot(x, y2, 'g', 'LineWidth', 2);


% Add labels and legend
xlabel('Voltage Value');
ylabel('Probability Density');
title('Normal Distributions for a Good and Discarded nozzle');
legend('\mu_Discareded, \sigma_Discareded', '\mu_Good, \sigma_Good');
grid on;
hold off;

end
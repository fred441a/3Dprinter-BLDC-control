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

showHis(volnGood)

function print(sys)
    plot(sys(:,2), sys(:,1))
    xlim([sys(1,2) sys(end,2)])
    xlabel('Time [s]');
    ylabel('Voltage Value');
    title('Voltage value over time for Good nozzle');
grid on
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
data = readtable('acp_sse_data.csv','PreserveVariableNames',true);
data.Properties.VariableNames
time = data.time_s;
ws = data.measWs_radS;
dev = data.deviancy;
vol = data.voltage;

%showHisDev(dev)
sys = vol;

sigma = std(sys)
mu = mean(sys)



%%
function showHisDev(data)
    histogram(data, 'BinWidth', 0.03571, 'Normalization', 'probability');
    xlabel('Devincies');
    ylabel('Density');
    title('Normalized Histogram');
end

function showHisWs(data)
    histogram(data, 'BinWidth', 0.1501, 'Normalization', 'probability');
    xlabel('Measured \omega [rad/s]');
    ylabel('Density');
    title('Normalized Histogram');
end
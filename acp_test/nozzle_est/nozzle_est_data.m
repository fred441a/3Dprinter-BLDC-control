data255 = readtable('nozzle_255.csv','PreserveVariableNames',true);
vol255 = data255.voltage;
time255 = (data255.absTime)/1000000;
ts255 = time255 - time255(1);
d55 = [vol255, ts255];

data275 = readtable('nozzle_275.csv','PreserveVariableNames',true);
vol275 = data275.voltage;
time275 = (data275.absTime)/1000000;
ts275 = time275 - time275(1);
d75 = [vol275, ts275];

data265 = readtable('nozzle_265.csv','PreserveVariableNames',true);
vol265 = data265.voltage;
time265 = (data265.absTime)/1000000;
ts265 = time265 - time265(1);
d65 = [vol265, ts265];

u_255 = mean(vol255);
sig_255 = std(vol255);

u_275 = mean(vol275);
sig_275 = std(vol275);

u_265 = mean(vol265);
sig_265 = std(vol265);

print(d65)

function print(sys)
    plot(sys(:,2), sys(:,1))
    xlim([sys(1,2) sys(end,2)])
    xlabel('Time [s]');
    ylabel('Voltage Value');
    title('Voltage value over time for 265deg ');
grid on
end
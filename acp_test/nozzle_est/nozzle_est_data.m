data255 = readtable('nozzle_255.csv','PreserveVariableNames',true);
vol255 = data255.voltage;
time255 = (data255.absTime)/1000000;
ts255 = time255 - time255(1);
ws255 = data255.ws_meas;
d55 = [vol255, ts255,ws255];

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

printHist(d55,d65,d75, 0.007 ,1.5)


function printHist(data1, data2, data3 ,binWidth, overflowEdge)
    % data1, data2 are matrices: [value, time]
    % Histogram only column 1, normalized to probability

    % Extract signal only
    data1 = data1(:,1);
    data2 = data2(:,1);
    data3 = data3(:,1);

    % Remove NaNs / Infs
    data1 = data1(isfinite(data1));
    data2 = data2(isfinite(data2));
    data3 = data3(isfinite(data3));


    figure;

    % Bin edges with overflow bin
    minEdge = min([data1; data2;data3]);
    edges = minEdge:binWidth:overflowEdge;
    edges(end+1) = Inf;

    % Plot normalized histograms
    histogram(data1, edges, ...
        'Normalization','probability', ...
        'FaceColor','r', 'FaceAlpha',0.7, ...
        'DisplayName','Data at 255 deg');
    hold on;

    histogram(data2, edges, ...
        'Normalization','probability', ...
        'FaceColor','b', 'FaceAlpha',0.7, ...
        'DisplayName','Data at 265deg');
    hold on;

    histogram(data3, edges, ...
        'Normalization','probability', ...
        'FaceColor','g', 'FaceAlpha',0.7, ...
        'DisplayName','Data at 275 deg');
    hold off;

    xlabel('Value');
    ylabel('Probability');
    title('Normalized Histogram for each of the temperatures');
    legend('show');
    grid on;
end



function print(sys)
    plot(sys(:,2), sys(:,1))
    xlim([sys(1,2) sys(end,2)])
    xlabel('Time [s]');
    ylabel('Voltage Value');
    title('Voltage value over time for 255deg ');
grid on
end

function printAdv(sys)
    % sys(:,1) = vol255
    % sys(:,2) = ts255
    % sys(:,3) = ws255

    figure;

    % Left y-axis: WS vs time
    yyaxis left
    plot(sys(:,2), sys(:,3), 'r', 'LineWidth', 1.5); % red line
    ylabel('WS (units)'); % adjust units

    % Right y-axis: Voltage vs time
    yyaxis right
    plot(sys(:,2), sys(:,1), 'b', 'LineWidth', 1.5); % blue line
    ylabel('Voltage (V)'); % adjust units

    % Common x-axis
    xlabel('Time [s]');
    xlim([sys(1,2) sys(end,2)]);
    
    title('Voltage and WS over Time for 255deg');
    legend('WS', 'Voltage');
    grid on;
end




function printMuch()

% Example means and standard deviations
u_1 = u_255;  s_1 = sig_255;
u_2 = u_275;  s_2 = sig_275;
u_3 = u_265;  s_3 = sig_265;

% Create a common x-axis range that covers all curves
x = linspace(min([u_1-3*s_1, u_2-3*s_2, u_3-3*s_3]), max([u_1+3*s_1, u_2+3*s_2, u_3+3*s_3]), 500);

% Compute the probability density functions
y1 = (1/(s_1*sqrt(2*pi))) * exp(-0.5*((x-u_1)/s_1).^2);
y2 = (1/(s_2*sqrt(2*pi))) * exp(-0.5*((x-u_2)/s_2).^2);
y3 = (1/(s_3*sqrt(2*pi))) * exp(-0.5*((x-u_3)/s_3).^2);

% Plot all three curves
plot(x, y1, 'r', 'LineWidth', 2); hold on;
plot(x, y2, 'g', 'LineWidth', 2);
plot(x, y3, 'b', 'LineWidth', 2);

% Add labels and legend
xlabel('Voltage Value');
ylabel('Probability Density');
title('Normal Distributions for different extrution temperatures');
legend('\mu_255, \sigma_255', '\mu_275, \sigma_2_75', '\mu_265, \sigma_265');
grid on;
hold off;

end
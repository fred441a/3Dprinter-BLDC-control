%% Interactive step response comparison: final value 1 vs 0.95

clc; clear; close all;

% Original transfer function numerator and denominator
num_orig = 5 * [1 2];
den = [1 2 5];
G_temp = tf(num_orig, den);

% --------------------- System 1: Settles at 1 ---------------------
desired_ss1 = 1;
dc_gain = dcgain(G_temp);
num1 = num_orig * (desired_ss1 / dc_gain);
G1 = tf(num1, den);

% --------------------- System 2: Settles at 0.95 ---------------------
desired_ss2 = 0.95;
num2 = num_orig * (desired_ss2 / dc_gain);
G2 = tf(num2, den);

% --------------------- Interactive step response ---------------------
figure('Name','Interactive Step Response Comparison','NumberTitle','off');
hold on;
grid on;

% Plot system 1
step(G1);     % MATLAB interactive step response viewer
% Plot system 2 on the same figure
step(G2);

title('Interactive Step Response: Settling at 1 vs 0.95');
xlabel('Time (s)');
ylabel('Output');
legend('Settling at 1','Settling at 0.95','Location','southeast');

% Enable data cursor to click and inspect overshoot, rise time, steady-state, etc.
datacursormode on;

% Print step info in command window
info1 = stepinfo(G1);
info2 = stepinfo(G2);
disp('System 1 (final value = 1) Step Info:'); disp(info1);
disp('System 2 (final value = 0.95) Step Info:'); disp(info2);

% Print final values and steady-state errors
final1 = dcgain(G1);
final2 = dcgain(G2);
fprintf('System 1 final value: %.3f, Steady-state error: %.3f\n', final1, 1-final1);
fprintf('System 2 final value: %.3f, Steady-state error: %.3f\n', final2, 1-final2);

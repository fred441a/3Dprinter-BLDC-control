%% Very slow, non-oscillatory system
clc; clear; close all;

% ---------------- Parameters ----------------
K = 1;       % system gain (final value)
zeta = 1.5;  % overdamped (no oscillation)
wn = 0.2;    % natural frequency (rad/s) → slow response
% --------------------------------------------

% Second-order system: G(s) = K * wn^2 / (s^2 + 2*zeta*wn*s + wn^2)
num = K * wn^2;
den = [1 2*zeta*wn wn^2];
G = tf(num, den);

% Display transfer function
disp('Very slow, non-oscillatory system:');
G

% Step response
t_sim = 0:0.1:80;  % simulate long time to see settling
figure('Name','Slow Settling Step Response','NumberTitle','off');
step(G, t_sim);
grid on;
title('Step Response of Very Slow System (No Oscillation)');
xlabel('Time (s)');
ylabel('Output');

% Enable interactive data cursor
datacursormode on;

% Step response info
info = stepinfo(G);
disp('Step Response Characteristics:');
disp(info);

% Final value
final_value = dcgain(G);
fprintf('Final value (steady-state): %.3f\n', final_value);

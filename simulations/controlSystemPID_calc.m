% Motor constants
L   = 1.12E-6;
R   = 2.8;
K_t = 0.22;
K_e = 2.25;
B   = 0.123;
J   = 0.000498;

s = tf('s');

% motor transfere function H(s)
H = K_t / ( (K_t*K_e) + (s*L + R) * (J*s + B) );
H_zpk = zpk(H);
H_poles = pole(H);
H_pole1 = -H_poles(1);
H_pole2 = -H_poles(2);
H_gain = get(H_zpk, 'K');

H_kp = 1/(H_pole2/H_gain)

K_p = H_kp %196.4633;
%T_i = 1;
T_d = 1/H_pole1 %1/H_pole2 = 0.0013

%C = K_p ;
%C = K_p * (1 + (1/(T_i*s)));
%C = K_p * (1 + (1/(T_i*s)) + (T_d*s));
C = K_p * (1 + (T_d*s));

% Closed-loop systems
T  = feedback(C*H, 1);

% Openloop system
T1 = C*H;

%step(T1)
%margin(T1)
%grid on

%[pidC, info] = pidtune(H, 'PID');

displayResult(H)
displayResult(T)

step(T)
grid on

%% ==============================================================
% === All helper functions go below ============================

function displayResult(system)
    sysName = inputname(1);

    % Convert to zpk
    SystemZpk  = zpk(system);

    printFactorized(sysName,  SystemZpk);
   
end

function printFactorized(name, sys)
    [z, p, k] = zpkdata(sys, 'v');
    zStrs = arrayfun(@fmtFactor, z, 'UniformOutput', false);
    pStrs = arrayfun(@fmtFactor, p, 'UniformOutput', false);

    fprintf('\n%s(s):\n', name);
    if isempty(zStrs)
        fprintf('Numerator:   %.6g\n', k);
    else
        fprintf('Numerator:   %.6g * %s\n', k, strjoin(zStrs, ' * '));
    end
    fprintf('Denominator: %s\n', strjoin(pStrs, ' * '));
end

function str = fmtFactor(root)
    a = real(root);
    b = imag(root);
    if abs(b) < 1e-8
        if a < 0
            str = sprintf('(s + %.6g)', abs(a));
        else
            str = sprintf('(s - %.6g)', a);
        end
    else
        if a < 0, signA = '+'; else, signA = '-'; end
        if b < 0
            str = sprintf('(s %s %.6g - %.6gj)', signA, abs(a), abs(b));
        else
            str = sprintf('(s %s %.6g + %.6gj)', signA, abs(a), abs(b));
        end
    end
end

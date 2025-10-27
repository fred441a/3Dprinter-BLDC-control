% Motor constants
L   = 1.12E-1;
R   = 2.8;
K_t = 0.22;
K_e = 2.25;
B   = 0.123;
J   = 0.001;

s = tf('s');

% motor transfere function H(s)
H = K_t / ( (K_t*K_e) + (s*L + R) * (J*s + B) );

% Controller parameters
K_p = 20;
T_i = 1;
T_d = 0.02;


%C = K_p ;
%C = K_p * (1 + (1/(T_i*s)));
C = K_p * (1 + (1/(T_i*s)) + (T_d*s));
%C = K_p * (1 + (T_d*s));

% Closed-loop systems
T  = feedback(C*H, 1);

% Openloop system
T1 = C*H;

%step(T1)
%margin(T1)
%grid on
pole(T);
zero(T);

% Enable or disable all printed output

displayResults(T);
%displayResults(H);

%% ==============================================================
% === All helper functions go below ============================

function displayResults(system)
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

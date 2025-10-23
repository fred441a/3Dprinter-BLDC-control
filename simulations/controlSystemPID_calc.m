% Motor constants
L   = 1.12E-6;
R   = 2.8;
K_t = 0.22;
K_e = 2.25;
B   = 0.123;
J   = 0.1;

s = tf('s');

% motor transfere function H(s)
H = K_t / ( (K_t*K_e) + (s*L + R) * (J*s + B) );

% Controller parameters
K_p = 200;
T_i = 0.071;
T_d = 0.02;

C = K_p * (1 + (1/(T_i*s)) + (T_d*s));

% Closed-loop systems
T  = feedback(C*H, 1);

% Openloop system
T1 = C*H;

% Enable or disable all printed output
showDisplay = true;   % Set to false to suppress all printouts

displayResults(H, T1, T, showDisplay);



%% ==============================================================
% === All helper functions go below ============================

function displayResults(H, T1, T, showDisplay)
    % Only print if the flag is true
    if ~showDisplay
        return;
    end

    % Convert to zpk
    Hzpk  = zpk(H);
    T1zpk = zpk(T1);
    Tzpk  = zpk(T);

    disp(' ')
    disp('======================================')
    disp('Zero–Pole–Gain (ZPK) forms:')
    disp('======================================')
    Hzpk
    T1zpk
    Tzpk

    fprintf('\n======================================\n');
    fprintf(' FACTORIZED FORMS (s + a)(s + b)...\n');
    fprintf('======================================\n');

    printFactorized('H',  Hzpk);
    printFactorized('T1', T1zpk);
    printFactorized('T',  Tzpk);
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

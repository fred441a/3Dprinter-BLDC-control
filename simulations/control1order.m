% Motor constants
R   = 2.8;
K_t = 0.22;
K_e = 0.36;
B   = 0.0197;
J   = 3.74E-3;
s = tf('s');

% Transfer function H1(s)
Hp = 2.5 / (s*0.08 + 1);

% Motor closed-loop transfer function H_c(s)
Hd = (K_t / ( (K_t*K_e) + R * (J*s + B) ));

% Plot both on the same figure
figure;
step(Hp, Hd)
legend show
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

s = tf('s');

%time konstant of motor
tau = 0.09;
%riste time of motor
tr = 0.1758;

K = 14/2.75;

%system sample time
Ts = 0.01

%bandwidth of system (in this case the natural freq of the motor wn)
wb = 1/(10*Ts)
% dampening faktor (calculated with chatgpt) 5%
zigma = 0.7 ;


Kp = (2*zigma*wb*tau-1)/K
Ki = (tau*wb^2)/K
Ti = 1/Kp*tau;

PI = Kp+Ki/s;

%H_c = K_t / ( (K_t*K_e) + (R) * (J*s + B) );

H_c = K/(s*tau+1)
system_o = H_c*PI;
margin(system_o)
system = (PI*H_c)/(1+H_c*PI);
%step(system)
grid on

system = (PI*H_c)/(1+H_c*PI);
sys = feedback(PI*H_c,1);
%step(system)
%grid on
%bode(H_c*PI);
Control_s = stepinfo(system);
Motor_s = stepinfo(H_c);

Control_s.RiseTime;
Motor_s.RiseTime;

%sysD = c2d(system, 0.012, 'zoh');
%step(sysD);

%hold on
%bode(system);
%bode(H_c)
%step(system);
%step(H_c);
%hold off
%grid on

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

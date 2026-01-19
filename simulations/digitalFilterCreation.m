
% =========================================
% GIVEN SPECIFICATIONS
% =========================================
Ap = 3;
Ar = 20;

wp = 11.5;
wr = 12.5;
w0 = sqrt((wp*wr));

eps = sqrt(10^(0.1*Ap) - 1);
k   = wp / wr;

% =========================================
% STEP 1–9 (Nome, order, etc.)
% =========================================
q0 = 0.5 * (1 - (1 - k^2)^(1/4)) / (1 + (1 - k^2)^(1/4));
q  = 150*q0^13 + 15*q0^9 + 2*q0^5 + q0;

d  = sqrt((10^(0.1*Ap) - 1) / (10^(0.1*Ar) - 1));
n  = ceil(log(16/d^2) / log(1/q))

beta = (1/(2*n)) * log((sqrt(1 + eps^2) + 1) / (sqrt(1 + eps^2) - 1) );

% Parameter "a"
M = ceil(10 / sqrt(1-q));
num = 0; den = 1;
for m = 0:M
    num = num + (-1)^m * q^(m*(m+1)) * sinh((2*m+1)*beta);
end

for m = 1:M
    den = den + 2*(-1)^m * q^(m^2) * cosh(2*m*beta);
end
a = 2*q^(1/2)*(num /(1+ den));

U = sqrt((1+k*a^2)*(1+(a^2/k)));

% =========================================
% STEP 10–14: omega_i, V_i, a_i, b_i, c_i
% =========================================
if mod(n,2) == 0
    Ni = n/2;
else
    Ni = (n-1)/2;
end

omega = zeros(1,Ni);
V = zeros(1,Ni);
ai = zeros(1,Ni);
bi = zeros(1,Ni);
ci = zeros(1,Ni);

for i = 1:Ni

    if mod(n,2) == 0
        l = i - 0.5;
    else
        l = i;
    end

    % omega_i
    num = 0; den = 1;
    for m = 0:M
        num = num + (-1)^m * q^(m*(m+1)) * ...
            sin((2*m+1)*pi*l/n);
    end
    for m = 1:M
        den = den + 2*(-1)^m * q^(m^2) * ...
            cos(2*pi*m*l/n);
    end
    omega(i) = 2*q^(1/4)*num / den;

    % Remaining parameters
    V(i)  = sqrt((1 - k*omega(i)^2)*(1 - omega(i)^2/k));
    ai(i) = 1 / (omega(i)^2);
    bi(i) = (2*a*V(i)) / (1 + a^2*omega(i)^2);
    ci(i) = (a*V(i)^2 + (omega(i)*U)^2) / ((1 + a^2*omega(i)^2)^2);

end

% =========================================
% STEP 15: H0
% =========================================
if mod(n,2) == 1
    H0 = a;
else
    H0 = 1/sqrt(1 + eps^2);
end

for i = 1:Ni
    H0 = H0 * (ci(i)/ai(i));
end

% =========================================
% FINAL STEP: Build H_a(s)
% =========================================
s = tf('s');
s = (s/w0);

Ha = 1;
for i = 1:Ni
    Ha = Ha * (s^2 + ai(i)) / (s^2 + bi(i)*s + ci(i));
end

if mod(n,2) == 1
    Ha = H0 * Ha / (s + a);   % n odd
else
    Ha = H0 * Ha;            % n even
end

% =========================================
% RESULT
% =========================================
disp('Analog Elliptic Transfer Function H_a(s):');
bode(Ha)


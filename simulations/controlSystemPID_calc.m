% Here are my constants
L = 1.12e-6;
R = 2.8;
K_t = 0.22;
K_e = 2.25;
B = 0.123;
K_b = 0.0;
J = 1E-4;
tau_L = 0;

s = tf('s');

H = K_t/((K_t*K_e) + (s*L + R) * (J*s + B));
H

OH = 1/((s*L + R)*(s*J + B));
%OH

%step(H)
%bode(H)
%pzmap(H);
grid on

poles = pole(H);
poles;

%margin(H);

%controller
K_p = 2E3;
T_i = 0.00005;
T_d = 0.0010;

%C = K_p;
%C = K_p * (1 + (1/(T_i *s )) + (T_d *s));
C = K_p * (1 + (T_d * s ));
%C = K_p * (1 + (1/(T_i *s )) );
%C;

T = (C*H)/(C * H + 1);
step(T)
grid on
%bode(T)
%margin(T)

%pole(T)

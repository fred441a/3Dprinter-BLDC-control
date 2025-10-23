% Here are my constants
L = 34E-6;
R = 1.38;
K_t = 0.23;
K_e = 0.0325;
B = 0.014;
K_b = 0.0;
J = 0.1;
tau_L = 0;

s = tf('s');

H = K_t/((K_t*K_e) + (s*L + R) * (J*s + B));
H

step(H)
%bode(H)
%pzmap(H);
%margin(H)
grid on
pole(H)
zero(H)


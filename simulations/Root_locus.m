tau = 0.08
K = 2.5
% 5% overshoot
zigma = 0.7

Kp = (2*zigma*wb*tau-1)/K
Ki = (tau*wb^2)/K

s = tf("s");
PI = (Kp+Ki/s);
system = K/(tau*s+1);

rlocus(PI*system);
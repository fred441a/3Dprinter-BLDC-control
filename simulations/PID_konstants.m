s = tf('s');

%time konstant of motor
tau = 0.08
%riste time of motor. May i suggest a smaller one? evt 0.05?
tr = 0.07
%bandwidth of system (in this case the natural freq of the motor wn)
wb = 1.8/tr
% dampening faktor (calculated with chatgpt) 5%. May i suggest a higher one. evt 0.9?
PO  = 0.01;
zigma = - log(PO) / sqrt(pi^2 + log(PO)^2)
%zigma = 0.7

Kp = 2*zigma*wb*tau-1;
Ki = tau*wb^2;
Ti = Kp/Ki;

PI = Kp+Ki/s;

%H_c = K_t / ( (K_t*K_e) + (R) * (J*s + B) );

H_c = 2.5/(s*0.08+1);

system_o = H_c*PI;

system = (PI*H_c)/(1+H_c*PI);

%bode(H_c*PI);
Control_s = stepinfo(system);
Motor_s = stepinfo(H_c);

Control_s.RiseTime;
Motor_s.RiseTime;

hold on
%bode(system);
%bode(H_c)
step(system);
step(H_c);
hold off
grid on
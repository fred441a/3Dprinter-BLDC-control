s = tf('s');

%time konstant of motor
tau = 0.08
%riste time of motor
tr = 0.1758
%bandwidth of system (in this case the natural freq of the motor wn)
wb = 1.8/tr
% dampening faktor (calculated with chatgpt) 5%
zigma = 0.7

Kp = 2*zigma*wb*tau-1;
Ki = tau*wb^2
Ti = Kp/Ki

PI = Kp+Ki/s

%H_c = K_t / ( (K_t*K_e) + (R) * (J*s + B) );

H_c = 2.5/(s*0.08+1)

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
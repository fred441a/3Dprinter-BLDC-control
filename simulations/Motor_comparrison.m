s = tf("s")
Measured = 2.5/(0.08*s+1);
Motor_step_j = 1.68/(0.078*s+1);
Kt = 0.22
Ke = 0.359
Kb = 0.0313
R = 2.8
B = 0.00000112
J = 0.000498

Motor_density_j = ((Kt)/(R*B+Kt*Ke))/(((R*J)/(R*B+Ke*Kt))*s+1)

hold on
step(Measured)
step(motor_step_j)
step(motor_density_j)
grid on
hold off
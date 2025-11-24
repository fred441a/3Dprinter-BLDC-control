Kp = 0.05
Ki = 1.7
Ts = 0.01
z = tf("z",Ts);

PI = Kp+Ki*((Ts*z)/(z-1));
PI.Numerator
PI.Denominator
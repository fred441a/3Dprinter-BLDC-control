Kp = 0.11
Ki = 1.4286
Ts = 0.01
z = tf("z",Ts);

PI = Kp+Ki*((Ts*z)/(z-1));
PI.Numerator
PI.Denominator

%syms Kp Ki Ts z
%PI = Kp+Ki*((Ts*z)/(z-1));
%PI_time = iztrans(PI);
%isp(PI_time);
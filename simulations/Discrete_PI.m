%s = tf("s");
syms Kp Ki Ts z
%PI = Kp+KI/s;

PI_discrete = Kp+Ki/((1-1/z)*Ts);

PI_time_discrete = iztrans(PI_discrete);


displayFormula(PI_time_discrete)
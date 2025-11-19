

class PID
{
private:
  float Kp, Ki, Kd;
  float limit = 6;

public:
  PID(float Kp, float Ki, float Kd) : Kp(Kp), Ki(Ki), Kd(Kd) {}
  float voltage(float ws_measure, float ws_wanted)
  {
    static float integral = 0;
    static float last_e = 0;
    static float pid = 0;
    static float Kaw = ((Ki / Kp));
    float e = ws_wanted - ws_measure;
    float derivative = (Kd * (last_e - e));
    derivative = 0;

    float pidPure = (Kp * e) + integral + derivative;
    printf("ws_wanted:%f , ws_measured:%f, errorTerm:%f \n", ws_wanted, ws_measure, e);
    printf("pidPure:%f\n", pidPure);

    // anti Windup scheme. If above, clamp, below set 0.:
    float pidSat = pidPure;
    if (pidSat > limit)
      pidSat = limit;
    if (pidSat < -limit)
      pidSat = 0;

    printf("pidSat:%f \n", pidSat);

    // update terms for next time
    float anti_windup = (Kaw * (pidSat - pidPure));
    integral += (Ki * e + anti_windup);

    last_e = e;
    if (pidSat < 0)
    {
      pidSat = 0;
    }
    return pidSat;
  };

  float voltageDis(float ws_measure, float ws_wanted, float T)
  {
    static float last_e = 0;
    static float e_last = 0;
    static float v_last = 0;

    float e = ws_wanted - ws_measure;

    float v = Kp*e - Kp*e_last + e_last*Ki*T + v_last;
    v_last = v;
    e_last = e;

    return v;
  };
};

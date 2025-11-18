

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
    static float Kaw = ((Ki/Kp));
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
    integral += (Ki * e + anti_windup)*0.012;

    last_e = e;
    return pidSat;
  };
};

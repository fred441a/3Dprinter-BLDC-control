

class PID
{
private:
  float Kp, Ki, Kd, Kaw;
  float limit = 16;

public:
  PID(float Kp, float Ki, float Kd, float Kaw) : Kp(Kp), Ki(Ki), Kd(Kd), Kaw(Kaw) {}
  double voltage(double ws_measure, double ws_wanted)
  {
    static double integral = 0;
    static double last_e = 0;
    static float last_pid = 0;
    static float last_pidPure = 0;
    static float anti_windup = 0;
    static float pid = 0;
    double e = ws_wanted - ws_measure;

    anti_windup = (last_pid-last_pidPure) * Kaw;
    integral += (Ki+anti_windup) * e;
    pid = (Kp * e) + integral + (Kd * (last_e - e));
    if (pid > limit)
    {
      last_pidPure = pid;
      pid = limit;
    }
    last_e = e;
    last_pid = pid;
    return pid;
  };
};



class PID {
private:
  float Kp, Ki, Kd;

public:
  PID(float Kp, float Ki, float Kd) : Kp(Kp), Ki(Ki), Kd(Kd) {}
  double voltage(double ws_measure, double ws_wanted) {
    static double integral = 0;
    static double last_e = 0;
    double e = ws_wanted - ws_measure;
    integral += Ki * e;
    return (Kp * e) + integral + (Kd * (last_e - e));
    last_e = e;
  };
};



class PID {
private:
  float Kp, Ti, Td;

public:
  PID(float Kp, float Ti, float Td) : Kp(Kp), Ti(Ti), Td(Td) {}
  double voltage(double ws_measure, double ws_wanted) {
    static double integral = 0;
    static double last_e = 0;
    double e = ws_wanted - ws_measure;
    return (Kp * e) + (Ti * integral) + (Td * (last_e - e));
    last_e = e;
    integral += e;
  };
};

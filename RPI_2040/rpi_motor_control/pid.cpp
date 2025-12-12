

#include <mutex>
class PID {
private:
  float Kp, Ki, Kd;
  float limit = 6;

public:
  PID(float Kp, float Ki, float Kd) : Kp(Kp), Ki(Ki), Kd(Kd) {}

  float voltageDis(float ws_measure, float ws_wanted, std::mutex ws_lock,
                   float Ts) {
    static float last_e = 0;
    static float e_last = 0;
    static float u_last = 0;
	ws_lock.lock();

    float e = ws_wanted - ws_measure;

    float u = Kp * e - Kp * e_last + Ki * e_last * Ts +
              u_last; // Kp*e - Kp*e_last + e*Ki*T + v_last;
    u_last = u;
    e_last = e;

    return u;
  };
};

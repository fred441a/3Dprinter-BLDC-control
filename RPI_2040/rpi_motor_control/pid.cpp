#include <cstdint>
#include <queue>
class PID {
private:
  float Kp, Ki, Kd;
  float limit = 6;
  std::queue<bool> *alarm_queue;
  uint8_t alarm_counter;

  void Alarm_check(float u) {
    if (u > 4.5) {
      alarm_counter++;
    } else {
      alarm_counter = 0;
    }
    if (alarm_counter > 50) {
      alarm_queue->push(true);
    }
  }

public:
  PID(float Kp, float Ki, float Kd, std::queue<bool> *alarm_queue)
      : Kp(Kp), Ki(Ki), Kd(Kd), alarm_queue(alarm_queue) {}

  float voltageDis(float ws_measure, float ws_wanted, float Ts) {
    static float last_e = 0;
    static float e_last = 0;
    static float u_last = 0;

    float e = ws_wanted - ws_measure;

    float u = Kp * e - Kp * e_last + Ki * e_last * Ts +
              u_last; // Kp*e - Kp*e_last + e*Ki*T + v_last;
    u_last = u;
    e_last = e;
    Alarm_check(u);

    return u;
  };
};

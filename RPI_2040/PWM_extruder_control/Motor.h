#pragma once
class Motor {
public:
  Motor(float Kt, float Ke, float B);
  float Kt;
  float Ke;
  float B;
};

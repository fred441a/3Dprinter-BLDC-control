#include <string>
#pragma once
/**
 * Abstract class that exposes a print function for debugging
 */
class DebugPrinter {
private:
public:
  virtual void print(std::string str) = 0;
};

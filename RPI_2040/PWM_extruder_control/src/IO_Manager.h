#define __FREERTOS 1
#include <FreeRTOS.h>
#include <semphr.h>
#include <unordered_map>

/**
 * IOManager handles mutex lock for the IO pins
 */
class IOManager {
private:
  // Hashmap of pins and corrosponding mutex
  std::unordered_map<int, SemaphoreHandle_t> pinLock;

  bool takePin();
  void releasePin();

public:
  IOManager();
  bool ReadDigitalPin();
  void WriteDigitalPin(bool output);
};

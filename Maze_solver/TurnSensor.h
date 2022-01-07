#pragma once
#include <Zumo32U4.h>

extern Zumo32U4ButtonA buttonA;
extern Zumo32U4LCD lcd;
extern Zumo32U4IMU imu;

extern uint32_t turnAngle;
extern int16_t turnRate;

#define turnAngle45 0x20000000
#define turnAngle90 turnAngle45 * 2
#define turnAngle1 (turnAngle45 + 22) / 45

class TrunSensor {

  
  
  public:
  uint32_t turnAngle = 0;
  int16_t turnRate = 0;
  int16_t gyroOffset = 0;
  uint16_t gyroLastUpdate = 0;
  // ZASADA RAII Resource acquisition is initialization
  void turnSensorSetup();
  void turnSensorReset();
  void turnSensorUpdate();
  
};

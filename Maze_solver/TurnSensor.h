#pragma once
#include <Zumo32U4.h>

#define LOOP_END 1
#define ONE_SECOND 1000

extern Zumo32U4ButtonA buttonA;
extern Zumo32U4LCD lcd;
extern Zumo32U4IMU imu;

#define TURN_ANGLE_45 0x20000000

class TurnSensor {
  // ZMIENNE PRYWATNE 
  // uzywane wewnatrz klasy nie sa potrzebne na zewnatrz
  // gwarantujem,y ze te zminne nie zostana zmodyfikowane poza klasa 
  private:
  // ZASADA RAII Resource acquisition is initialization
  uint32_t turnAngle = 0;
  int16_t turnRate = 0;
  int16_t gyroOffset = 0;
  uint16_t gyroLastUpdate = 0;

  // uzywamy na zawnatrz klasy 
  public:
  void turnSensorSetup();
  void turnSensorReset();
  void turnSensorUpdate();
  // potrzbujemy odczytac zminna turnAngle ale nie chemy pozwolic by inne obiekty mogly ja modyfikowac 
  // potrzebujemy Interfacu
  uint32_t getTurnAngle();
  
};

#pragma once
#include <Zumo32U4.h>


// hmmm to sa jakis interfacy ale nie wiem jak do nich podejsc w kwesti designu 
// problem jest taki ze on tu uzywa sie eksternow czyli to jest jakas bliloteka dynamiczna za tym skoro tak to zapina 
// tego za bardzo nie chce narazie ruszac
extern Zumo32U4ButtonA buttonA;
extern Zumo32U4LCD lcd;
extern Zumo32U4IMU imu;

// Uzywam define choc mozna wrocic do const a juz najlepiej by bylo do constexpr
#define turnAngle45 0x20000000
#define turnAngle90 turnAngle45 * 2
#define turnAngle1 (turnAngle45 + 22) / 45

class TrunSensor {
  // ZMIENNE PRYWATNE 
  // uzywane wewnatrz klasy nie sa potrzebne na zewnatrz
  private:
  int16_t turnRate = 0;
  int16_t gyroOffset = 0;
  uint16_t gyroLastUpdate = 0;
 
  public:
  uint32_t turnAngle = 0;
  // ZASADA RAII Resource acquisition is initialization
  void turnSensorSetup();
  void turnSensorReset();
  void turnSensorUpdate();
  
};

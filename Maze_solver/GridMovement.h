#pragma once

#include <Wire.h>
#include <Zumo32U4.h>
#include "TurnSensor.h"

#define NUM_OF_SENSORS 5
#define SENSOR_THRESHOLD 200
#define SENSOR_THRESHOLD_DARK 600
#define CALIBRATION_SPEED 150

extern uint16_t lineSensorValues[NUM_OF_SENSORS];

extern Zumo32U4IMU imu;
extern Zumo32U4ButtonA buttonA;
extern Zumo32U4ButtonB buttonB;
extern Zumo32U4Motors motors;
extern Zumo32U4LCD lcd;
extern Zumo32U4LineSensors lineSensors;

struct settings{
  uint16_t straightSpeed = 200;
  uint16_t intersectionDelay = 50;
  uint16_t turnSpeed = 200;
};

class GridMovment {
  private :
  TurnSensor Sensor;
  settings stats;
  void printBar(uint8_t barHeight);
  uint16_t readSensors();
  void lineSensorSetup();
  bool aboveLine(uint8_t sensorIndex);
  bool aboveLineDark(uint8_t sensorIndex);
   
  public :
  bool aboveDarkSpot();
  void turn(char dir);
  void followSegment(bool Kierunek = true);
  void driveToIntersectionCenter(bool Kierunek = true);
  void driveToIntersectionCenter(bool * foundLeft, bool * foundStraight, bool * foundRight);
  void gridMovementSetup();
  void updateSettings();
  void printSettings();
  


};

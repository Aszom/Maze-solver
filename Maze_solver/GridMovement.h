#pragma once

#include <Wire.h>
#include <Zumo32U4.h>

  
const uint16_t straightSpeed2 = 200;
const uint16_t straightSpeed1 = 400;
const uint16_t intersectionDelay2 =80;
const uint16_t intersectionDelay1 =50;
const uint16_t turnSpeed = 200;
const uint16_t calibrationSpeed = 150;
const uint16_t sensorThreshold = 200;
const uint16_t sensorThresholdDark = 600;
const uint8_t numSensors = 5;

const int32_t gyroAngle45 = 0x20000000;

void printBar(uint8_t barHeight);
uint16_t readSensors();
bool aboveLine(uint8_t sensorIndex);
bool aboveLineDark(uint8_t sensorIndex);
bool aboveDarkSpot();
extern uint16_t lineSensorValues[numSensors];
void turn(char dir);
void followSegment(bool Kierunek = true);
void driveToIntersectionCenter(bool Kierunek = true);
void driveToIntersectionCenter(bool * foundLeft, bool * foundStraight, bool * foundRight);
void gridMovementSetup();


extern Zumo32U4IMU imu;
extern Zumo32U4ButtonA buttonA;
extern Zumo32U4Motors motors;
extern Zumo32U4LCD lcd;
extern Zumo32U4LineSensors lineSensors;

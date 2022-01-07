#include <Wire.h>
#include "TurnSensor.h"


void TrunSensor::turnSensorSetup()
{
  Wire.begin();
  imu.init();
  imu.enableDefault();
  imu.configureForTurnSensing();

  lcd.clear();
  lcd.print(F("Gyro cal"));

  ledYellow(1);

  delay(1000);

  int32_t total = 0;
  for (uint16_t i = 0; i < 1024; i++)
  {
    while(!imu.gyroDataReady()) {}
    imu.readGyro();
    total += imu.g.z;
  }
  ledYellow(0);
  gyroOffset = total / 1024;

  lcd.clear();
  turnSensorReset();
  while (!buttonA.getSingleDebouncedRelease())
  {
    turnSensorUpdate();
    lcd.gotoXY(0, 0);
    lcd.print((((int32_t)turnAngle >> 16) * 360) >> 16);
    lcd.print(F("   "));
  }
  lcd.clear();
  
}

void TrunSensor::turnSensorReset()
{
  gyroLastUpdate = micros();
  turnAngle = 0;
}

void TrunSensor::turnSensorUpdate()
{
  imu.readGyro();
  turnRate = imu.g.z - gyroOffset;
  uint16_t m = micros();
  uint16_t dt = m - gyroLastUpdate;
  gyroLastUpdate = m;
  int32_t d = (int32_t)turnRate * dt;
  
  // (0.07 dps/digit) * (1/1000000 s/us) * (2^29/45 unit/degree)
  // = 14680064/17578125 unit/(digit*us)
  turnAngle += (int64_t)d * 14680064 / 17578125;
}

#include "GridMovement.h"

uint16_t lineSensorValues[NUM_OF_SENSORS];

static void loadCustomCharacters()
{
  static const char levels[] PROGMEM = {
    0, 0, 0, 0, 0, 0, 0, 63, 63, 63, 63, 63, 63, 63
  };
  lcd.loadCustomCharacter(levels + 0, 0);  // 1 bar
  lcd.loadCustomCharacter(levels + 1, 1);  // 2 bars
  lcd.loadCustomCharacter(levels + 2, 2);  // 3 bars
  lcd.loadCustomCharacter(levels + 3, 3);  // 4 bars
  lcd.loadCustomCharacter(levels + 4, 4);  // 5 bars
  lcd.loadCustomCharacter(levels + 5, 5);  // 6 bars
  lcd.loadCustomCharacter(levels + 6, 6);  // 7 bars
}

void GridMovment::lineSensorSetup()
{
  lcd.clear();
  lcd.print(F("Line call"));
  delay(1000);
  
  Sensor.turnSensorReset();

  motors.setSpeeds(-CALIBRATION_SPEED, CALIBRATION_SPEED);
  while((int32_t)Sensor.getTurnAngle() < TURN_ANGLE_45 * 2)
  {
    lineSensors.calibrate();
    Sensor.turnSensorUpdate();
  }

  motors.setSpeeds(CALIBRATION_SPEED, -CALIBRATION_SPEED);
  while((int32_t)Sensor.getTurnAngle() > -TURN_ANGLE_45 * 2)
  {
    lineSensors.calibrate();
    Sensor.turnSensorUpdate();
  }

  motors.setSpeeds(-CALIBRATION_SPEED, CALIBRATION_SPEED);
  while((int32_t)Sensor.getTurnAngle() < 0)
  {
    lineSensors.calibrate();
    Sensor.turnSensorUpdate();
  }

  motors.setSpeeds(0, 0);

  lcd.clear();
  while(!buttonA.getSingleDebouncedPress())
  {
    readSensors();

    lcd.gotoXY(0, 0);
    for (uint8_t i = 0; i < NUM_OF_SENSORS; i++)
    {
      uint8_t barHeight = map(lineSensorValues[i], 0, 1000, 0, 8);
      printBar(barHeight);
    }
  }

  lcd.clear();
  
}


void GridMovment::printBar(uint8_t height)
{
  if (height > 8) { height = 8; }
  const char barChars[] = {' ', 0, 1, 2, 3, 4, 5, 6, 255};
  lcd.print(barChars[height]);
}

uint16_t GridMovment::readSensors()
{
  return lineSensors.readLine(lineSensorValues);
}

bool GridMovment::aboveLine(uint8_t sensorIndex)
{
  return lineSensorValues[sensorIndex] > SENSOR_THRESHOLD;
}

bool GridMovment::aboveLineDark(uint8_t sensorIndex)
{
  return lineSensorValues[sensorIndex] > SENSOR_THRESHOLD_DARK;
}

bool GridMovment::aboveDarkSpot()
{
  return aboveLineDark(1) && aboveLineDark(2) && aboveLineDark(3);
}


void GridMovment::turn(char dir)
{
  if (dir == 'S')
  {
    return;
  }

  Sensor.turnSensorReset();

  uint8_t sensorIndex;

  switch(dir)
  {
  case 'B':
    motors.setSpeeds(-stats.turnSpeed, stats.turnSpeed);
    while((int32_t)Sensor.getTurnAngle() < TURN_ANGLE_45 * 3)
    {
      Sensor.turnSensorUpdate();
    }
    sensorIndex = 1;
    break;

  case 'L':
    motors.setSpeeds(-stats.turnSpeed, stats.turnSpeed);
    while((int32_t)Sensor.getTurnAngle() < TURN_ANGLE_45)
    {
      Sensor.turnSensorUpdate();
    }
    sensorIndex = 1;
    break;

  case 'R':
    motors.setSpeeds(stats.turnSpeed, -stats.turnSpeed);
    while((int32_t)Sensor.getTurnAngle() > -TURN_ANGLE_45)
    {
      Sensor.turnSensorUpdate();
    }
    sensorIndex = 3;
    break;

  default:
    return;
  }

  while(1)
  {
    readSensors();
    if (aboveLine(sensorIndex))
    {
      break;
    }
  }
}

void GridMovment::followSegment(bool Kierunek = true)
{
  while(1)
  {
    uint16_t position = readSensors();
    int16_t error = (int16_t)position - 2000;
    int16_t speedDifference = error / 4;
    int16_t leftSpeed = 0;
    int16_t rightSpeed = 0;

    leftSpeed = (int16_t)stats.straightSpeed + speedDifference;
    rightSpeed = (int16_t)stats.straightSpeed - speedDifference;
    leftSpeed = constrain(leftSpeed, 0, (int16_t)stats.straightSpeed);
    rightSpeed = constrain(rightSpeed, 0, (int16_t)stats.straightSpeed);
    
    motors.setSpeeds(leftSpeed, rightSpeed);

    if(!aboveLine(0) && !aboveLine(1) && !aboveLine(2) && !aboveLine(3) && !aboveLine(4))
    {
      break;
    }

    if(aboveLine(0) || aboveLine(4))
    {
      break;
    }
  }
}

void GridMovment::driveToIntersectionCenter(bool Kierunek = true)
{
  motors.setSpeeds(stats.straightSpeed, stats.straightSpeed);
  if(Kierunek != false)
  { 
    delay(stats.intersectionDelay);
  }
  else if(Kierunek == false)
  {
    delay(stats.intersectionDelay);
  }
  
}

void GridMovment::driveToIntersectionCenter(bool * foundLeft, bool * foundStraight, bool * foundRight)
{
  *foundLeft = 0;
  *foundStraight = 0;
  *foundRight = 0;

  motors.setSpeeds(stats.straightSpeed, stats.straightSpeed);
  for(uint16_t i = 0; i < stats.intersectionDelay / 2; i++)
  {
    readSensors();
    if(aboveLine(0))
    {
      *foundLeft = 1;
    }
    if(aboveLine(4))
    {
      *foundRight = 1;
    }
  }

  readSensors();

  if(aboveLine(1) || aboveLine(2) || aboveLine(3))
  {
    *foundStraight = 1;
  }
  
}

void GridMovment::gridMovementSetup()
{
  lineSensors.initFiveSensors();
  loadCustomCharacters();
  Sensor.turnSensorSetup();
  lineSensorSetup();
}

void GridMovment::updateSettings()
{
  if(stats.straightSpeed >= 400)
  {
    stats.straightSpeed = 200;
    stats.intersectionDelay = 50;
  }
  else
  {
    stats.straightSpeed += 100;
    stats.intersectionDelay += 15;
  }
  
}
void GridMovment::printSettings()
{
  lcd.clear();
  lcd.print("Speed :");
  lcd.gotoXY(0, 1);
  lcd.print(stats.straightSpeed);
  lcd.print("     ");
}

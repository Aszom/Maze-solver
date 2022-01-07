#include <Wire.h>
#include <Zumo32U4.h>
#include "GridMovement.h"
#define LOOP_END 1

Zumo32U4LCD lcd;
Zumo32U4Buzzer buzzer;
Zumo32U4ButtonA buttonA;
Zumo32U4Motors motors;
Zumo32U4LineSensors lineSensors;
Zumo32U4IMU imu;

char path[100];
uint8_t pathLength = 0;

static GridMovment robotMovment;

void displayPath()
{
  path[pathLength] = 0;
  lcd.clear();
  if(pathLength > 16)
  {
    lcd.print("PATH +16");
    return;
  }
  
  lcd.print(path);
  if(pathLength > 8)
  {
    lcd.gotoXY(0, 1);
    lcd.print(path + 8);
  }
  
}

void setup()
{
  robotMovment.printSettings();
  buzzer.playFromProgramSpace(PSTR("!>g32>>c32"));
  robotMovment.gridMovementSetup();
  mazeSolve();
}

void loop()
{
  while(LOOP_END)
  {
    if( buttonA.getSingleDebouncedPress())
    {
      robotMovment.updateSettings();
      robotMovment.printSettings();
    }
    if( buttonB.isPressed())
    {
      break;
    } 
  }
  
  mazeFollowPath(false);
  
  buttonA.waitForButton();
  mazeFollowPath(true);
  
}

char selectTurn(bool foundLeft, bool foundStraight, bool foundRight)
{
  if(foundRight) { return 'R'; }
  else if(foundStraight) { return 'S'; }
  else if(foundLeft) { return 'L'; }
  else { return 'B'; }
}

void mazeSolve()
{
  
  pathLength = 0;
  buzzer.playFromProgramSpace(PSTR("!L16 cdegreg4"));
  delay(1000);

  while(LOOP_END)
  {
    robotMovment.followSegment();
    bool foundLeft, foundStraight, foundRight;
    robotMovment.driveToIntersectionCenter(&foundLeft, &foundStraight, &foundRight);

    if(robotMovment.aboveDarkSpot())
    {
      break;
    }

    char dir = selectTurn(foundLeft, foundStraight, foundRight);
    if (pathLength >= sizeof(path))
    {
      lcd.clear();
      lcd.print(F("pathfull"));
      while(1)
      {
        ledRed(1);
      }
    }

    path[pathLength] = dir;
    pathLength++;
    simplifyPath(); //upraszczanie
    displayPath(); //wyswitlanie
    if (pathLength == 2 && path[0] == 'B' && path[1] == 'B')
    {
      buzzer.playFromProgramSpace(PSTR("!<b4"));
    }
    robotMovment.turn(dir);

  }

  motors.setSpeeds(0, 0);
  buzzer.playFromProgramSpace(PSTR("!>>a32"));
  lcd.clear();
}

char revertPath(char turnDecision)
{
  switch(turnDecision)
  {
  case 'L':
    return 'R';
  case 'R':
    return 'L';
  default:
    return 'S';
  }
}

// KIERUNEK == TRUE Z STARTU NA METE
// KIERUNEK == FALSE Z METY NA START
void mazeFollowPath(bool Kierunek)
{
  lcd.clear();
  char revertedPath [100] = "";
  for(uint16_t i = 0; i < pathLength; i++)
  {
    revertedPath[pathLength-i-1] = revertPath(path[i]);
  }
  if(Kierunek){
    lcd.print(path);
  }
  else{
    lcd.print(revertedPath);
  }
  buzzer.playFromProgramSpace(PSTR("!>c32"));
  delay(1000);
  
  robotMovment.turn('B');
  robotMovment.turn('S');
 
  for(uint16_t i = 0; i < pathLength; i++)
  {
    robotMovment.followSegment(false);
    robotMovment.driveToIntersectionCenter();
    if(Kierunek){
      robotMovment.turn(path[i]);
    }
    else{
      robotMovment.turn(revertedPath[i]);
    }
  }

  robotMovment.followSegment();
  motors.setSpeeds(0, 0);
  buzzer.playFromProgramSpace(PSTR("!>>a32"));
}

void simplifyPath()
{
  if(pathLength < 3 || path[pathLength - 2] != 'B')
  {
    return;
  }

  int16_t totalAngle = 0;

  for(uint8_t i = 1; i <= 3; i++)
  {
    switch(path[pathLength - i])
    {
    case 'L':
      totalAngle += 90;
      break;
    case 'R':
      totalAngle -= 90;
      break;
    case 'B':
      totalAngle += 180;
      break;
    }
  }

  totalAngle = totalAngle % 360;
  switch(totalAngle)
  {
  case 0:
    path[pathLength - 3] = 'S';
    break;
  case 90:
    path[pathLength - 3] = 'L';
    break;
  case 180:
    path[pathLength - 3] = 'B';
    break;
  case 270:
    path[pathLength - 3] = 'R';
    break;
  }

  pathLength -= 2;
}

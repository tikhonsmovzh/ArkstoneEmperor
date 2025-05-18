#include <Arduino.h>
#include "Devices.h"
#include "Configs.h"
#include "DriveTrain/DriveTrain.h"
// #include "DriveTrain/DriveTrain.h"
// #include "Intake.h"

void setup() {
  Serial.begin(115200);

  devicesBegin();

  while (!startButton.readState());
  while (startButton.readState());
  
  // intakeBegin();
  // detectFloor();

  driveTrainBegin();
  
  while (!startButton.readState());
  while (startButton.readState());

  // intakeStart();
  driveTrainStart();

  brushServoLeft.write(0);
  brushServoRight.write(180);
}

void loop() {
  devicesUpdate();

  driveTrainUpdate();

  // driveTrainUpdate();
  // intakeUpdate();
}
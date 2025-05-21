#include <Arduino.h>
#include "Devices.h"
#include "Configs.h"
#include "DriveTrain/DriveTrain.h"
#include "Intake.h"

void setup() {
  Serial.begin(115200);

  devicesBegin();

  while (!startButton.readState());
  while (startButton.readState());
  
  // driveTrainBegin();
  // intakeBegin();
  // detectFloor();
  
  while (!startButton.readState());
  while (startButton.readState());

  // intakeStart();
  // driveTrainStart();
}

void loop() {
  devicesUpdate();

  // driveTrainUpdate();

  // driveTrainUpdate();
  // intakeUpdate();
}
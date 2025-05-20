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

  brushServoLeft.write(180);
  brushServoRight.write(0);

  clampServo.write(100);

  brushMotor.setDirection(REVERSE);
  brushMotor.setPower(1.0f);
}

void loop() {
  devicesUpdate();

  // driveTrainUpdate();

  // driveTrainUpdate();
  // intakeUpdate();
}
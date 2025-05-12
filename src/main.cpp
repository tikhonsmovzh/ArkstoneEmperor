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
  
  // intakeBegin();
  // detectFloor();

  // driveTrainBegin();
  
  while (!startButton.readState());
  while (startButton.readState());

  // intakeStart();
  // driveTrainStart();
}

void loop() {
  static uint32_t oldUpdateTime = 0;
  while (micros() - oldUpdateTime > UPDATE_TIME_MICROS);
  oldUpdateTime = micros();

  forwardDistanceFilter.update(forwardDistanceSensor.readDistance());
  leftDistanceFilter.update(leftDistanceSensor.readDistance());
  rightDistanceFilter.update(rightDistanceSensor.readDistance());

  
  // driveTrainUpdate();
  // intakeUpdate();
}
#include <Arduino.h>
#include "Devices.h"
#include "Configs.h"
// #include "DriveTrain/DriveTrain.h"
// #include "Intake.h"

void setup() {
  Serial.begin(115200);

  devicesBegin();

  // while (!startButton.readState());
  // while (startButton.readState());
  
  // intakeBegin();
  // detectFloor();

  // driveTrainBegin();
  
  // while (!startButton.readState());
  // while (startButton.readState());

  // intakeStart();
  // driveTrainStart();
}

void loop() {
  devicesUpdate();

  delay(15);

  Serial.print("forward = ");
  Serial.print(forwardDistanceFilter.getCurrentValue());
  Serial.print("  left = ");
  Serial.print(leftDistanceFilter.getCurrentValue());
  Serial.print("  right = ");
  Serial.println(rightDistanceFilter.getCurrentValue());

  // driveTrainUpdate();
  // intakeUpdate();
}
#include <Arduino.h>
#include "Devices.h"
#include "Configs.h"
#include "DriveTrain/DriveTrain.h"
#include "Intake.h"

void setup() {
  Serial.begin(115200);

  devicesBegin();

  startButton.setReversed(true);

  // while (!startButton.readState());
  // while (startButton.readState());

  delay(100);
  
  driveTrainBegin();
  intakeBegin();
  detectFloor();

  delay(100);
  
  // while (!startButton.readState());
  // while (startButton.readState());
  delay(100);

  intakeStart();
  driveTrainStart();
}

void loop() {
  // static uint32_t oldTime = 0;

  // while (oldTime + 10 > millis())
  //   oldTime = millis();

  devicesUpdate();

  driveTrainUpdate();

  intakeUpdate();
}
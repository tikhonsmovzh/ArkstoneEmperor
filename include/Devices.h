#pragma once

#include <Arduino.h>
#include "drivers/DcMotor.h"
#include "drivers/MultiWire.h"
#include "drivers/Gyro.h"
#include "drivers/ColorSensor.h"
#include "drivers/DistanceSensor.h"
#include "drivers/Button.h"
#include <Servo.h>
#include "utils/MedianFilter.h"
#include "utils/ISRValue.h"
#include "utils/ElapsedTime.h"
#include "Configs.h"

HardwareWire hardwareWire;
SoftwareWire softwareWire(2, 3);

DcExpansion dcExpansion1(1, &hardwareWire);
DcExpansion dcExpansion3(3, &hardwareWire);

DcMotor leftMotor(&dcExpansion1, 1);
DcMotor rightMotor(&dcExpansion1, 2);

DcMotor brushMotor(&dcExpansion3, 1);
DcMotor separatorMotor(&dcExpansion3, 2);

TCS34725ColorSensor separatorColorSensor(&hardwareWire);
TCS34725ColorSensor clampColorSenor(&softwareWire);

// black list 2; 5;

DistanceSensor forwardDistanceSensor(7, 4);
DistanceSensor leftDistanceSensor(9, 3);
DistanceSensor rightDistanceSensor(6, 8);

MedianFilter<uint16_t> forwardDistanceFilter(0);
MedianFilter<uint16_t> leftDistanceFilter(0);
MedianFilter<uint16_t> rightDistanceFilter(0);

BNO055Gyro gyro(&hardwareWire);

Button startButton(2);
Button forwardtButton(2);

Servo clampServo;
Servo brushServoLeft, brushServoRight;

void devicesBegin(){
    // clampServo.attach(11);

    brushServoLeft.attach(10);
    brushServoRight.attach(5);

    hardwareWire.begin();
    // softwareWire.begin();

    dcExpansion1.begin();
    dcExpansion3.begin();

    // separatorColorSensor.begin();
    // clampColorSenor.begin();

    forwardDistanceSensor.begin();
    leftDistanceSensor.begin();
    rightDistanceSensor.begin();

    startButton.begin();
    forwardtButton.begin();

    // gyro.begin();

    leftMotor.begin();
    brushMotor.begin();
    separatorMotor.begin();
    rightMotor.begin();
}

ElapseTime _deviceUpdateTime;

void devicesUpdate(){
    if(_deviceUpdateTime.milliseconds() > 6){
        forwardDistanceFilter.update(forwardDistanceSensor.readDistance());
        leftDistanceFilter.update(leftDistanceSensor.readDistance());
        rightDistanceFilter.update(rightDistanceSensor.readDistance());

        _deviceUpdateTime.reset();
    }
}
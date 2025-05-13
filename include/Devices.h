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

DistanceSensor _forwardDistanceSensor(6, 5);
DistanceSensor _leftDistanceSensor(3, 4);
DistanceSensor _rightDistanceSensor(8, 7);

ISRValue<uint16_t> forwardDistanceSensor;
ISRValue<uint16_t> leftDistanceSensor;
ISRValue<uint16_t> rightDistanceSensor;

MedianFilter<uint16_t> forwardDistanceFilter(0);
MedianFilter<uint16_t> leftDistanceFilter(0);
MedianFilter<uint16_t> rightDistanceFilter(0);

BNO055Gyro gyro(&hardwareWire);

Button startButton(2);

Servo clampServo;
Servo brushServoLeft, brushServoRight;

void devicesBegin(){
    // clampServo.attach(11);

    // brushServoLeft.attach(12);
    // brushServoRight.attach(13);

    hardwareWire.begin();
    // softwareWire.begin();

    dcExpansion1.begin();
    dcExpansion3.begin();

    // separatorColorSensor.begin();
    // clampColorSenor.begin();

    _forwardDistanceSensor.begin();
    _leftDistanceSensor.begin();
    _rightDistanceSensor.begin();

    startButton.begin();

    // gyro.begin();

    leftMotor.begin();
    brushMotor.begin();
    separatorMotor.begin();
    rightMotor.begin();

    cli();

    TCCR2A = 0;
    TCCR2B = 0;
  
    OCR2A = 93;
    TCCR2A |= (1 << WGM21);
    TCCR2B |= (1 << CS22) | (1 << CS21) | (1 << CS20);
    TIMSK2 |= (1 << OCIE2A);

    sei();
}

void devicesUpdate(){
    cli();

    forwardDistanceSensor.update();
    leftDistanceSensor.update();
    rightDistanceSensor.update();

    sei();
}

ISR(TIMER2_COMPA_vect) {
    forwardDistanceSensor.ISRSet(forwardDistanceFilter.update(_forwardDistanceSensor.readDistance()));
    leftDistanceSensor.ISRSet(leftDistanceFilter.update(_leftDistanceSensor.readDistance()));
    rightDistanceSensor.ISRSet(rightDistanceFilter.update(_rightDistanceSensor.readDistance()));
}

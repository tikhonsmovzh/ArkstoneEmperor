#pragma once

#include <Arduino.h>
#include "Devices.h"
#include "Configs.h"
#include "utils/PDRegulator.h"
#include "utils/Sgn.h"
#include "utils/ElapsedTime.h"

class DriveSample
{
protected:
    void Drive(float forward, float turn)
    {
        leftMotor.setPower(forward + turn);
        rightMotor.setPower(forward - turn);
    }

    float GetOriantation()
    {
        return gyro.getOrientation().x;
    }

    void encoderReset()
    {
        rightMotor.softwareEncoderReset();
        leftMotor.softwareEncoderReset();
    }

    void dropProcess()
    {
        Drive(0.0, 0.0);
        encoderReset();
    }

    PDRegulator<float> *PDreg;

public:
    float kp = 1.0f;
    float kd = 1.0f;

    DriveSample() {}; // для очереди

    DriveSample(PDRegulator<float> &PDr)
    {
        PDreg = &PDr;
    }

    virtual void Start()
    {
        encoderReset();

        PDreg->start();
    }

    virtual bool Execute()
    {
        return true;
    }

    void ResetPd()
    {
        PDreg->reset(kp, kd);
    }
};
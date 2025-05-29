#pragma once

#include <Arduino.h>
#include "Devices.h"
#include "Configs.h"
#include "utils/PDRegulator.h"
#include "utils/Queue.h"
#include "utils/Sgn.h"
#include "utils/ElapsedTime.h"
#include "DriveTrain/DriveSample.h"
#include "Intake.h"

/*
enum SimpleActions{
    DriveToWall,        // + +
    TurnOnWall,         // + -
    DriveAlongWall,     // + -
    DriveOnEncoder,     // + -
    TurnLocal,          // + +
    TurnGlobal          // + -
};
*/

class DriveForwardToTheLimit : public DriveSample
{
    float _distance;

public:
    DriveForwardToTheLimit(PDRegulator<int32_t> &PDr, float distance) : DriveSample(PDr)
    {
        abc = false;
        _distance = distance;
        kp = 0.017f; // нужны норм каэфициенты!
        // kd = 0.0000000005f;
        kd = 0.0f;
    }

    bool Execute() override
    { // энкодеры сбрасываются, все норм. ПД тоже сбрасывается
        float dist = forwardDistanceFilter.getCurrentValue();
        if (dist > _distance)
        {
            float err = PDreg->update(rightMotor.readCurrentPosition() - leftMotor.readCurrentPosition());
            Drive(forward, err);

            return false;
        }
        dropProcess();

        return true;
    }
};

class TurnToTheWall : public DriveSample
{
    float _distance;

public:
    TurnToTheWall(PDRegulator<int32_t> &PDr, float distance) : DriveSample(PDr)
    {
        _distance = distance;
        kp = 1.0f; // нужны норм каэфициенты!
        kd = 1.0f;
    }

    bool Execute() override
    {
        if (forwardDistanceFilter.getCurrentValue() < _distance)
        {
            Drive(stop, -ROBOT_SPEED);
            return false;
        }
        dropProcess();

        return true;
    }
};

class DrivingAlongTheWall : public DriveSample
{
    float _distance;

public:
    
    DrivingAlongTheWall(PDRegulator<int32_t> &PDr, float distance) : DriveSample(PDr)
    {
        _distance = distance;
        kp = 0.015f; // нужны норм каэфициенты!
        kd = 0.000000005f;
    }

    bool Execute() override
    {
        if (forwardDistanceFilter.getCurrentValue() > _distance)
        {
            float errValue = _distance - leftDistanceFilter.getCurrentValue(); //потом свапнуть на райт
            errValue *= kp;
            // float err = PDreg->update(errValue);
            Drive(forward, errValue);
            return false;
        }
        dropProcess();

        return true;
    }
};

class TravelByEncoderValue : public DriveSample
{
    float _encPos;

public:
    TravelByEncoderValue(PDRegulator<int32_t> &PDr, float encPos) : DriveSample(PDr)
    {
        _encPos = encPos;
        kp = 1.0f; // нужны норм каэфициенты!
        kd = 1.0f;
    }

    bool Execute() override
    {
        if (((leftMotor.readCurrentPosition() + rightMotor.readCurrentPosition()) / 2) > _encPos)
        {
            Drive(backward, PDreg->update(rightMotor.readCurrentPosition() - leftMotor.readCurrentPosition()));
            return false;
        }
        dropProcess();

        return true;
    }
};

class TurnByGlobalCoordinates : public DriveSample
{
    float _targetRotate;

public:
    TurnByGlobalCoordinates(PDRegulator<int32_t> &PDr, float targetRotate) : DriveSample(PDr)
    {
        _targetRotate = targetRotate;
        kp = 1.0f; // нужны норм каэфициенты!
        kd = 1.0f;
    }

    bool Execute() override
    {
        if (!IS_GYRO)
            return true; // просто пропустит

        float error = chopDegrees(_targetRotate - GetOriantation());

        if (abs(error) > ANGLE_ERROR)
        {
            Drive(stop, ROBOT_SPEED * sgn(error));
            return false;
        }
        dropProcess();

        return true;
    }
};

class TurnByLocalCoordinates : public DriveSample
{
private:
    float _targetTurn;
    float _startCoords;

public:
    TurnByLocalCoordinates(PDRegulator<int32_t> &PDr, float targetTurn) : DriveSample(PDr)
    {
        abc = true;
        _targetTurn = chopDegrees(targetTurn);
        kp = 1.0f; // нужны норм каэфициенты!
        kd = 1.0f;
    }

    void Start() override
    {
        DriveSample::Start();

        if (IS_GYRO)
            _startCoords = GetOriantation();
    }

    bool Execute() override
    {
        if (IS_GYRO)
        {
            float error = chopDegrees(_targetTurn - (_startCoords - GetOriantation()));

            if (abs(error) > ANGLE_ERROR)
            {
                Drive(stop, ROBOT_SPEED * sgn(error) * 0.5f);
                return false;
            }
            dropProcess();
            return true;
        }
        else
        {
            float Angle = (DIAMETR * PI) / SINGLE_ENCODER_STEP * leftMotor.readCurrentPosition();

            float angle = (Angle / WHEEL_DISTANCE) / PI * 180;
            angle = chopDegrees(angle);

            float error = chopDegrees(_targetTurn - angle);
            

            if (abs(error) > ANGLE_ERROR)
            {
                Drive(stop, ROBOT_SPEED * sgn(error));
                return false;
            }

            dropProcess();
            return true;
        }
    }
};
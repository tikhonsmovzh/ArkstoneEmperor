
#pragma once

#include <Arduino.h>
#include "DriveTrain/DriveSamples.h"
#include "utils/Queue.h"
#include "utils/ElapsedTime.h"

// Queue<DriveSample *> _trajectory;
DriveSample* mainMovement;
ElapseTime _Timer;
PDRegulator<int32_t> MainPDregulator(0.1f, 0.1f); 

void driveTrainBegin()
{
    rightMotor.setEncoderDirection(REVERSE);

    mainMovement = new DriveForwardToTheLimit(MainPDregulator, ETALON_DISTANCE * 2);

    // здесь надо вставить каэфициенты для первого действия (у нас это DriveForwardToTheLimit)

    // _trajectory.enqueue(new DriveForwardToTheLimit(MainPDregulator, ETALON_DISTANCE * 2));


    // _trajectory.enqueue(new DriveForwardToTheLimit(MainPDregulator, ETALON_DISTANCE));

    // if (IS_GYRO)
    //     _trajectory.enqueue(new TurnByGlobalCoordinates(MainPDregulator, 90));

    // for (int step = 1; step < 4; step++)
    // { // три круга
    //     for (int actions = 1; actions < 5; actions++)
    //     { // в каждом круге полный проезд, из четырех проездов по стене, и четырех поворотов
    //         _trajectory.enqueue(new DrivingAlongTheWall(MainPDregulator, ETALON_DISTANCE * step));

    //         if (IS_GYRO)
    //             _trajectory.enqueue(new TurnByGlobalCoordinates(MainPDregulator, chopDegrees(90 + 90 * actions)));
    //         else
    //             _trajectory.enqueue(new TurnToTheWall(MainPDregulator, ETALON_DISTANCE * step));
    //     }
    // }

    // if (IS_GYRO)
    // {
    //     _trajectory.enqueue(new TurnByGlobalCoordinates(MainPDregulator, -90));
    //     _trajectory.enqueue(new DriveForwardToTheLimit(MainPDregulator, ETALON_DISTANCE));
    //     _trajectory.enqueue(new TurnByGlobalCoordinates(MainPDregulator, -180));
    //     _trajectory.enqueue(new DriveForwardToTheLimit(MainPDregulator, ETALON_DISTANCE));
    //     _trajectory.enqueue(new TurnByGlobalCoordinates(MainPDregulator, 0));
    // }
    // остается только дописать генерацию рандомной езды (опционально), или есть идея интереснее, но ее позже расскажу
}

void driveTrainStart()
{
    _Timer.reset();
    mainMovement->ResetPd();
    mainMovement->Start();
    /*

    if (!_trajectory.isEmpty()){
        _trajectory.front()->ResetPd();
        _trajectory.front()->Start();
    }
        */
}

void driveTrainUpdate()
{
    bool E_Ans = mainMovement->Execute() || (EXECUTION_LIMIT - _Timer.seconds()) < TIME_ERROR;
    if (E_Ans){
        _Timer.reset();

        if (!mainMovement->abc){
            mainMovement = new TurnByLocalCoordinates(MainPDregulator, 30 * (rand() % 2 == 0 ? -1 : 1));
        }else{
            mainMovement = new DriveForwardToTheLimit(MainPDregulator, ETALON_DISTANCE * 2);
        }
        mainMovement->ResetPd();
        mainMovement->Start();
    }
    /*
    if (_trajectory.isEmpty()){
        return;
    }

    bool E_Ans = _trajectory.front()->Execute() || (EXECUTION_LIMIT - _Timer.seconds()) < TIME_ERROR;
    if (E_Ans)
    {
        _Timer.reset();
        DriveSample* a = _trajectory.frontAndDequeue();
        if (_trajectory.isEmpty()){
            if (!a->abc){
                _trajectory.enqueue(new TurnByLocalCoordinates(MainPDregulator, 30 * sgn(rand())));
            }else{
                _trajectory.enqueue(new DriveForwardToTheLimit(MainPDregulator, ETALON_DISTANCE * 2));
            }
        }

        delete a;

        if (!_trajectory.isEmpty())
        {
            _trajectory.front()->ResetPd();
            _trajectory.front()->Start();
        }
    }
        */
}
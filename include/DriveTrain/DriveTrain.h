
#pragma once

#include <Arduino.h>
#include "DriveTrain/DriveSamples.h"
#include "utils/Queue.h"
#include "utils/ElapsedTime.h"
#include "Intake.h"

// Queue<DriveSample *> _trajectory;
DriveSample* mainMovement;
ElapseTime _Timer;
PDRegulator<int32_t> MainPDregulator(0.1f, 0.1f); 

void driveTrainBegin()
{
    rightMotor.setEncoderDirection(REVERSE);

    mainMovement = new DriveForwardToTheLimit(MainPDregulator, ETALON_DISTANCE);

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

    bool ans = !forwardtButton.readState();
    bool E_Ans = mainMovement->Execute();

    if (ans){
        delete mainMovement;
        mainMovement = new TravelByEncoderValue(MainPDregulator, -200.0f);
        mainMovement->ResetPd();
        mainMovement->Start();
        _Timer.reset();
    }

    // ans = ourColor == floorColor;

    // if (ans && mainMovement->abc){
    //     delete mainMovement;
    //     mainMovement = new StopOnPark(MainPDregulator, 0.25f, _Timer);
    //     mainMovement->ResetPd();
    //     mainMovement->Start();
    //     _Timer.reset();
    // }

    if (E_Ans || (EXECUTION_LIMIT - _Timer.seconds()) < TIME_ERROR){
        _Timer.reset();

        if (!mainMovement->abc){
            delete mainMovement;
            mainMovement = new TurnByTime(MainPDregulator, 0.25f, _Timer, (rand() % 2 == 0 ? true : false));
        }else{
            delete mainMovement;
            mainMovement = new DriveForwardToTheLimit(MainPDregulator, ETALON_DISTANCE);
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
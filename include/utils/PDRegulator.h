#pragma once

#include <Arduino.h>

template <typename T>
class PDRegulator{
private:
    float _kp, _kd;

    T _oldErr = 0;

    uint32_t _lastUpdateTime = 0;

public:
    PDRegulator(float kp, float kd){
        _kp = kp;
        _kd = kd;
    }

    float update(T err){
        float uP = err * _kp;

        float uD = ((err - _oldErr) / (millis() - _lastUpdateTime)) * _kd;

        _oldErr = err;

        _lastUpdateTime = millis();

        return uP + uD;
    }

    void start(){
        _lastUpdateTime = millis();
    }

    void reset(float kp = 0.0f, float kd = 0.0f){
        _kp = kp;
        _kd = kd;

        start();

        _oldErr = 0;
    }
};
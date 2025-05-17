#pragma once

#include <Arduino.h>


template <typename undefinedType>
class PDRegulator
{
  private:

    float _coefP = 0.0f, _coefD = 0.0f;
    //  Coefficients for the P, and D regulator corrections
    undefinedType _prevError = 0;
    //  Previous error


  public:

    void Reset (float setP, float setD)
    {
        _coefP = setP;
        _coefD = setD;
        //  Reset coefficients

        _prevError = 0;
        //  reset previous error
    }


    float UpdateCorrection (undefinedType error)
    {
        float correctionP =  error * _coefP;
        float correctionD = (error - _prevError) * _coefD;
        //  Calculate P and D corrections

        _prevError = error;
        //  Save previous error for future D correction

        //  Return the sum of both regulators correction
        return correctionP + correctionD;
    }
};
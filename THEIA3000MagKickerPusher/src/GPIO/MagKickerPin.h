#ifndef MAGKICKERPIN_H
#define MAGKICKERPIN_H

#include <Arduino.h>

enum MagSide {
    Left = 0,
    Right = 1
};

class MagKickerPin {
private:
    MagSide mag;
    
    void _setPinR();
    void _setPinL();
    void _initPin();

public:
    /* Output */
    uint8_t op_FWD;         // MotorForward
    uint8_t op_REV;         // MotorReverse
    uint8_t op_PF_FULL;     // PLATFORM_FULL
    uint8_t op_Status;      // FinishProcess
    uint8_t op_Alarm;       // Alarm
    
    /* Input */
    uint8_t ip_Sen_HM;      // SensorHOME
    uint8_t ip_Sen_LIM;     // SensorLIMIT
    uint8_t ip_Sen_PF;      // SensorPLATFORM
    uint8_t ip_Control;     // ControlByIO
    uint8_t ip_ClrAlarm;    // ClrAlarm
    
    uint8_t op_PWM;         // PWM_Hardware
    uint8_t ip_an_Sen;      // Analog
    
    MagKickerPin(MagSide mag);
    void init_io();
    void set_pwm(uint8_t duty);
};

#endif  // MAGKICKERPIN_H

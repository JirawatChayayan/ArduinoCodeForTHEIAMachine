#include <Arduino.h>
#include "MagKickerPin.h"

MagKickerPin::MagKickerPin(MagSide mag)
{
    if (mag == MagSide::Left) {
        _setPinL();
    } else {
        _setPinR();
    }   
    _initPin();
}

void MagKickerPin::_setPinR() {
    /* Output */
    op_FWD = 40;        // MotorForward
    op_REV = 41;        // MotorReverse
    op_PF_FULL = 42;    // PLATFORM_FULL
    op_Status = 43;     // FinishProcess
    op_Alarm = 44;      // Alarm
    
    /* Input */
    ip_Sen_HM = 45;     // SensorHOME
    ip_Sen_LIM = 46;    // SensorLIMIT
    ip_Sen_PF = 47;     // SensorPLATFORM
    ip_Control = 48;    // ControlByIO
    ip_ClrAlarm = 49;   // ClrAlarm
    
    op_PWM = 3;         // PWM_Hardware
    ip_an_Sen = A1;     // Analog
}

void MagKickerPin::_setPinL() {
    /* Output */
    op_FWD = 30;        // MotorForward
    op_REV = 31;        // MotorReverse
    op_PF_FULL = 32;    // PLATFORM_FULL
    op_Status = 33;     // FinishProcess
    op_Alarm = 34;      // Alarm
    
    /* Input */
    ip_Sen_HM = 35;     // SensorHOME
    ip_Sen_LIM = 36;    // SensorLIMIT
    ip_Sen_PF = 37;     // SensorPLATFORM
    ip_Control = 38;    // ControlByIO
    ip_ClrAlarm = 39;   // ClrAlarm
    
    op_PWM = 2;         // PWM_Hardware
    ip_an_Sen = A0;     // Analog
}

void MagKickerPin::_initPin() {
    pinMode(op_FWD, OUTPUT);
    pinMode(op_REV, OUTPUT);
    pinMode(op_PF_FULL, OUTPUT);
    pinMode(op_Status, OUTPUT);
    pinMode(op_Alarm, OUTPUT);
    
    pinMode(ip_Sen_HM, INPUT_PULLUP);
    pinMode(ip_Sen_LIM, INPUT_PULLUP);
    pinMode(ip_Sen_PF, INPUT_PULLUP);
    pinMode(ip_Control, INPUT);
    pinMode(ip_ClrAlarm, INPUT);
    
    digitalWrite(op_FWD, LOW);
    digitalWrite(op_REV, LOW);
    digitalWrite(op_PF_FULL, LOW);
    digitalWrite(op_Status, LOW);
    digitalWrite(op_Alarm, LOW);
    analogWrite(op_PWM, 0);
}

void MagKickerPin::init_io() {
    _initPin();
}

void MagKickerPin::set_pwm(uint8_t duty) {
    //Serial.println(String(op_PWM) + " duty "+ String(duty));
    analogWrite(op_PWM, duty);
}

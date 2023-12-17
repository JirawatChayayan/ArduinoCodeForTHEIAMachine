#ifndef EJECTOR_H
#define EJECTOR_H

#include <Arduino.h>
#include "GPIO/EjectorPin.h"

class Ejector
{
private:
    EjectorPin _pin;
    static uint8_t count_isr;  // Declare count_isr as static
    enum EjectorStateMachine {
        CheckIsHoming = 0,
        MoveREV = 1,
        MoveFWD = 2,
        CounterHole = 3,
        GoBackToHome = 4,
    };
    uint8_t _ejecterLength = 0; 
    uint8_t CountLen = 0;




    EjectorStateMachine state_machine = EjectorStateMachine::CheckIsHoming;
    // Private member functions
    bool Handle();
    bool SensorLimit();
    bool SensorHome();
    bool IsHomming();
    uint8_t ReadLengthEjector();
    void Enable(bool enable);
    void FWD_Ejector();
    void REV_Ejector();
    void Break_Ejector();
    void Ejector_Status_out(bool statusIO);
    uint8_t CheckLength();
    unsigned long T_update = millis();
    bool serial_control = false;
    uint8_t serial_length = 3;
    bool serial_finish = false;
    bool serial_on_moving = false;

public:
    bool stop_motion = false;
    static void sensor_interrupt();
    Ejector(EjectorPin pin);
    void control();
    void set_length(String length);
    void set_control(bool control);
    String update();
};

#endif  // EJECTOR_H

#ifndef GRIPPER_H
#define GRIPPER_H

#include <Arduino.h>
#include "GPIO/GripperPin.h"

class Gripper
{
private:
    GripperPin _pin;
    void Enable(bool enable);
    unsigned long T_update = millis();
    bool serial_control = false;
    bool haveControl = false;
    bool gripper_1_active = false;
    bool gripper_2_active = false;
    bool gripper_3_active = false;
    bool gripper_4_active = false;

    bool gripper_1_control = false;
    bool gripper_2_control = false;
    bool gripper_3_control = false;
    bool gripper_4_control = false;

    void refresh_io();
    uint8_t pinSelect(int ch);
    bool haveIOControl();

public:
    Gripper(GripperPin pin);
    void control();
    String update();
    void gripperAction(int ch,bool state);
    void gripperBitAction(String bit);
    void set_control(bool status);

};

#endif  // GRIPPER_H

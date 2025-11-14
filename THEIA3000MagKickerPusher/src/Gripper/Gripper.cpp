#include "Gripper.h"

Gripper::Gripper(GripperPin pin)
{
    _pin = pin;
}

void Gripper:: refresh_io()
{
    gripper_1_control = !digitalRead(_pin.ip_1);
    gripper_2_control = !digitalRead(_pin.ip_2);
    gripper_3_control = !digitalRead(_pin.ip_3);
    gripper_4_control = !digitalRead(_pin.ip_4);
    gripper_1_active = digitalRead(_pin.op_1);
    gripper_2_active = digitalRead(_pin.op_2);
    gripper_3_active = digitalRead(_pin.op_3);
    gripper_4_active = digitalRead(_pin.op_4);
}

uint8_t Gripper::pinSelect(int ch)
{
    uint8_t p;
    switch (ch)
    {
        case 1:
            p = _pin.op_1;
            break;
        case 2:
            p = _pin.op_2;
            break;
        case 3:
            p = _pin.op_3;
            break;
        case 4:
            p = _pin.op_4;
            break;
    }
    return p;
}

bool Gripper::haveIOControl()
{
    haveControl =  gripper_1_control || gripper_2_control || gripper_3_control || gripper_4_control;
    return haveControl;
}

void Gripper::control()
{
    refresh_io();
    haveIOControl();
    if(haveControl)
    {
        serial_control = false;
    }
    if(!serial_control)
    {        
        //copy signal 
        digitalWrite(_pin.op_1,gripper_1_control);
        digitalWrite(_pin.op_2,gripper_2_control);
        digitalWrite(_pin.op_3,gripper_3_control);
        digitalWrite(_pin.op_4,gripper_4_control);
    }
}

void Gripper::gripperAction(int ch,bool state)
{
    if(haveIOControl())
    {
        return;
    }
    serial_control = true;
    uint8_t p = pinSelect(ch);
    digitalWrite(p,state);
}

void Gripper::gripperBitAction(String binaryString)
{
    serial_control = true;
    if(binaryString.length() != 4)
        return;
    // Convert the binary string to a byte array
    byte bitArray[4];  // We'll store a maximum of 8 bits (1 byte)
    for (int i = 0; i < binaryString.length(); i++) {
        if (binaryString[i] == '1') {
        bitArray[i] = 1;
        } else if (binaryString[i] == '0') {
        bitArray[i] = 0;
        } else {
            return;
        }
    }

    for (size_t i = 0; i < 4; i++)
    {
        gripperAction(i+1,bitArray[i]);
    }

}

void Gripper::set_control(bool status)
{
    serial_control = status;
}

String Gripper::update()
{
    String msg = "gs,"+String(gripper_1_active)+","
                       +String(gripper_2_active)+","
                       +String(gripper_3_active)+","
                       +String(gripper_4_active)
               +";gc,"+String(gripper_1_control)+","
                       +String(gripper_2_control)+","
                       +String(gripper_3_control)+","
                       +String(gripper_4_control); 

    return msg;
}
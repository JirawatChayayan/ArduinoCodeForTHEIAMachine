#ifndef GRIPPER_H
#define GRIPPER_H

#include <Arduino.h>
class Gripper
{
    private:

        uint8_t _activePin = 3;
        uint8_t _controlPin = A5;
        uint8_t _active_val = 255;
        uint8_t _hold_val = 128;
        uint8_t _eeprom_addr = 0;
        bool _isActive = false;
        unsigned long _counter = 0;
        unsigned long _changePWM = 2000;
        void write_eeprom();

    public:
        Gripper(uint8_t active_pin,uint8_t control_pin,uint8_t eeprom_addr);
        void set_value(uint8_t active_val,uint8_t hold_val,unsigned long changePWM);
        void control();
        void read_eeprom();
};

#endif  // GRIPPER_H

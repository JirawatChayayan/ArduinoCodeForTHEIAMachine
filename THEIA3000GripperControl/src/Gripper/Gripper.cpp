#include "Gripper.h"
#include <Arduino.h>
#include <EEPROM.h>

Gripper::Gripper(uint8_t active_pin,uint8_t control_pin,uint8_t eeprom_addr)
{
    _activePin = active_pin;
    _controlPin = control_pin;
    _eeprom_addr = eeprom_addr;
    pinMode(_controlPin,INPUT_PULLUP);
    //read_eeprom();
    
}

void Gripper::set_value(uint8_t active_val,uint8_t hold_val,unsigned long changePWM)
{
    _active_val = active_val;
    _hold_val = hold_val;
    _changePWM = changePWM;
    write_eeprom();
}

void Gripper::read_eeprom()
{
    uint8_t active_val = 0;
    uint8_t hold_val = 0;
    unsigned long change_PWM = 0;

    EEPROM.get(_eeprom_addr, active_val);
    EEPROM.get(_eeprom_addr+2, hold_val);
    EEPROM.get(_eeprom_addr+4, change_PWM);

    if(active_val == 0 || active_val == 255)
    {
        Serial.println("No active val set to default");
        EEPROM.put(_eeprom_addr, _active_val);
        active_val = _active_val;
    }
    if(hold_val == 0 || hold_val == 255)
    {
        Serial.println("No hold val set to default");
        EEPROM.put(_eeprom_addr+2, _hold_val);
        hold_val = _hold_val;
    }
    if(change_PWM == 0 || change_PWM == 4294967295)
    {
        Serial.println("No change pwm val set to default");
        EEPROM.put(_eeprom_addr+4, _changePWM);
        change_PWM = _changePWM;
    }

    _active_val = active_val;
    _hold_val = hold_val;
    _changePWM = change_PWM;
    Serial.println("EEPROM data active "+String(_active_val) +", hold "+String(_hold_val)+", change "+String(_changePWM));
}

void Gripper::write_eeprom()
{
    EEPROM.put(_eeprom_addr, _active_val);
    EEPROM.put(_eeprom_addr+2, _hold_val);
    EEPROM.put(_eeprom_addr+4, _changePWM);
    Serial.println("EEPROM data active "+String(_active_val) +", hold "+String(_hold_val)+", change "+String(_changePWM));
}

void Gripper::control()
{
    bool control = (digitalRead(_controlPin)&&digitalRead(_controlPin)&&digitalRead(_controlPin)&&digitalRead(_controlPin));
    if(control)
    {
        if(!_isActive)
        {
            _counter = millis();
            _isActive = true;
            analogWrite(_activePin,_active_val);
        }
        if(_isActive && (millis() - _counter) > _changePWM)
        {
            analogWrite(_activePin,_hold_val);
        }
    }
    else
    {
        _isActive = false;
        analogWrite(_activePin,0);
        _counter = 0;
    }
}


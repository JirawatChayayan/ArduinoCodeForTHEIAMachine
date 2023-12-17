#include "Ejector.h"

uint8_t Ejector::count_isr = 0;

Ejector::Ejector(EjectorPin pin)
{
    _pin = pin;
    attachInterrupt(digitalPinToInterrupt(_pin.ip_Sen_Limit), sensor_interrupt, FALLING);  // FALLING or RISING
}

bool Ejector::Handle()
{
    bool pc_handle = digitalRead(_pin.ip_Control) && 
                     digitalRead(_pin.ip_Control) && 
                     digitalRead(_pin.ip_Control);
    return pc_handle;
}

bool Ejector::SensorLimit()
{
    bool lim = digitalRead(_pin.ip_Sen_Limit) && 
                digitalRead(_pin.ip_Sen_Limit) && 
                digitalRead(_pin.ip_Sen_Limit);
    return lim;
}

bool Ejector::SensorHome()
{
    bool hm = digitalRead(_pin.ip_Sen_HOME) && 
              digitalRead(_pin.ip_Sen_HOME) && 
              digitalRead(_pin.ip_Sen_HOME);
    return hm;
}

bool Ejector::IsHomming()
{
  bool hm = SensorLimit() && SensorHome();
  return hm;
}

uint8_t Ejector::ReadLengthEjector()
{
    bool bit_1 = (digitalRead(_pin.ip_bit_L1) && digitalRead(_pin.ip_bit_L1) && digitalRead(_pin.ip_bit_L1));
    bool bit_2 = (digitalRead(_pin.ip_bit_L2) && digitalRead(_pin.ip_bit_L2) && digitalRead(_pin.ip_bit_L2));

    if (!bit_1 && !bit_2)
    {
        return 1;
    }
    else if (!bit_2 && bit_1)
    {
        return 2;
    }
    else if (bit_2 && !bit_1)
    {
        return 3;
    }
    else if (bit_2 && bit_1)
    {
        return 4;
    }

    return 4;
}

void Ejector::Enable(bool enable)
{
    digitalWrite(_pin.op_PWM,enable);
}

void Ejector::FWD_Ejector()
{
    digitalWrite(_pin.op_FWD, HIGH);
    digitalWrite(_pin.op_REV, LOW);
}

void Ejector::REV_Ejector()
{
    count_isr = 0;
    digitalWrite(_pin.op_REV, HIGH);
    digitalWrite(_pin.op_FWD, LOW);
}

void Ejector::Break_Ejector()
{
    digitalWrite(_pin.op_REV, LOW);
    digitalWrite(_pin.op_FWD, LOW);
}

void Ejector::Ejector_Status_out(bool statusIO)
{
    digitalWrite(_pin.op_Status, statusIO);
}

void Ejector::sensor_interrupt()
{
    count_isr++;
    if (count_isr > 10)
    {
        count_isr = 0;
    }
    //Serial.println(count_isr);
}

uint8_t Ejector::CheckLength()
{
    if(serial_control)
    {
        _ejecterLength = serial_length;
    }
    else
    {
        _ejecterLength = ReadLengthEjector();
    }
    return _ejecterLength;
}

void Ejector::control()
{
    if(stop_motion)
    {
        Enable(false);
        Ejector_Status_out(false);
        Break_Ejector();
        serial_finish = false;
        serial_on_moving = false;
        serial_control = false;
        return;
    }
    else
    {
        Enable(true);
        if(Handle() || serial_control)
        {
            _ejecterLength = CheckLength(); 
            serial_finish = false;
            serial_on_moving = true;
            switch (state_machine)
            {
                case EjectorStateMachine::CheckIsHoming:
                    count_isr = 0;
                    if(IsHomming())
                    {
                        state_machine = EjectorStateMachine::MoveFWD;
                    }
                    else
                    {
                        state_machine = EjectorStateMachine::MoveREV;
                    }
                    break;
                case EjectorStateMachine::MoveREV:
                    REV_Ejector();
                    state_machine = EjectorStateMachine::CheckIsHoming;
                    break;
                case EjectorStateMachine::MoveFWD:
                    FWD_Ejector();
                    state_machine = EjectorStateMachine::CounterHole;
                    break;
                case EjectorStateMachine::CounterHole:
                    if(count_isr>= _ejecterLength+1)
                    {
                        state_machine = EjectorStateMachine::GoBackToHome;
                    }
                    else
                    {
                        state_machine = EjectorStateMachine::MoveFWD;
                    }
                    break;
                case EjectorStateMachine::GoBackToHome:
                    if(IsHomming())
                    {
                        serial_control = false;
                        serial_finish = true;
                        serial_on_moving = false;
                        Ejector_Status_out(true);
                        Break_Ejector();
                    }
                    else
                    {
                        REV_Ejector();
                    }

                default:
                    break;
            }
        }
        else
        {
            serial_on_moving = false;
            Ejector_Status_out(false);
            state_machine = EjectorStateMachine::CheckIsHoming;
            if(IsHomming())
            {
                Break_Ejector();
            }
            else
            {
                REV_Ejector();
            }
        }
    }

    //update();
}
void Ejector::set_length(String length)
{
    uint8_t len = 3;
    if(length == "4")
    {
        len = 4;
    }
    else if(length == "3")
    {
        len = 3;
    }
    else if(length == "2")
    {
        len = 2;
    }
    else if(length == "1")
    {
        len = 1;
    }
    serial_length = len;
    
}
void Ejector::set_control(bool control)
{
    serial_control = control;
}

String Ejector::update()
{
    String msg = "e_s,"+String(serial_finish)+","+String(serial_on_moving)+","+String(serial_control)+","+String(serial_length); 
    // unsigned long T_now = millis();
    // if(T_now - T_update > 200)
    // {
    //     T_update = T_now;
    //     Serial.println();
    // }
    return msg;
}
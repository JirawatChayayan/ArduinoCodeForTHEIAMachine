#include "MagKicker.h"

MagKicker::MagKicker(MagKickerPin pin_mag) : _pin_mag(pin_mag)
{
    _pin_mag = pin_mag;
}

bool MagKicker::setAlarm(bool alarm)
{
    digitalWrite(_pin_mag.op_Alarm, alarm);
    return alarm;
}

void MagKicker::goHome(bool SensorHM)
{
    if(SensorHM)
    {
      stopMotion();
    }
    else
    {
      moveToHome();
    }
}

void MagKicker::statusFinishProcess(bool status)
{
    digitalWrite(_pin_mag.op_Status, status);
}

void MagKicker::stopMotion()
{
    digitalWrite(_pin_mag.op_FWD, LOW);
    digitalWrite(_pin_mag.op_REV, LOW);
}

void MagKicker::moveToHome()
{
    digitalWrite(_pin_mag.op_FWD, LOW);
    digitalWrite(_pin_mag.op_REV, HIGH);
}

void MagKicker::moveToLimit()
{
    digitalWrite(_pin_mag.op_FWD, HIGH);
    digitalWrite(_pin_mag.op_REV, LOW);
}

bool MagKicker::readHandle()
{
    bool Control = digitalRead(_pin_mag.ip_Control) && digitalRead(_pin_mag.ip_Control) && digitalRead(_pin_mag.ip_Control);;
    return Control;
}

bool MagKicker::readSensHome()
{
    bool SensorHM = !(digitalRead(_pin_mag.ip_Sen_HM) && digitalRead(_pin_mag.ip_Sen_HM) && digitalRead(_pin_mag.ip_Sen_HM));
    return SensorHM;
}

bool MagKicker::readSensLimit()
{
    bool SensorLIMIT = !(digitalRead(_pin_mag.ip_Sen_LIM) && digitalRead(_pin_mag.ip_Sen_LIM) && digitalRead(_pin_mag.ip_Sen_LIM));
    return SensorLIMIT;
}

bool MagKicker::readAlarm()
{
    bool Alarm = digitalRead(_pin_mag.ip_ClrAlarm) && digitalRead(_pin_mag.ip_ClrAlarm) && digitalRead(_pin_mag.ip_ClrAlarm);
    return Alarm;
}

bool MagKicker::readPlatform()
{
    bool SensorPF = !(digitalRead(_pin_mag.ip_Sen_PF) && digitalRead(_pin_mag.ip_Sen_PF) && digitalRead(_pin_mag.ip_Sen_PF));
    digitalWrite(_pin_mag.op_PF_FULL,SensorPF);
    return SensorPF;
}

float MagKicker::readCurrentSens()
{
    float MaxAnalogVal = max_analog_resolution;
    uint8_t analogpin = _pin_mag.ip_an_Sen;
    int sensorValue1 = analogRead(analogpin);
    int sensorValue2 = analogRead(analogpin);
    int sensorValue3 = analogRead(analogpin);
    int sensorValue4 = analogRead(analogpin);
    int sensorValue5 = analogRead(analogpin);
    int sensorValue6 = analogRead(analogpin);
    int sensorValue7 = analogRead(analogpin);
    int sensorValue8 = analogRead(analogpin);
    int sensorValue9 = analogRead(analogpin);
    int sensorValue10 = analogRead(analogpin);
    int sensorValue11 = analogRead(analogpin);
    int sensorValue12 = analogRead(analogpin);
    int sensorValue = (sensorValue1+sensorValue2+sensorValue3+sensorValue4+sensorValue5+sensorValue6+sensorValue7+sensorValue8+sensorValue9+sensorValue10+sensorValue11+sensorValue12)/12.0;
    float voltage = sensorValue * (5.0 / MaxAnalogVal);
    float current = (voltage/r_sense);
    // if(current >0.3)
    // {
    //  Serial.println(current);
    // }

    return current;
}

void MagKicker::alarmRecovery(unsigned long loop_count)
{
    if(count_protect >0 &&!Alarm)
    {
        iCountNormalLoop++;
        // if(iCountNormalLoop >= loop_count)
        // {
        //     count_protect = 0;
        //     iCountNormalLoop = 0;
        // }
        if(iCountNormalLoop <= 1)
        {
            //Serial.println("Set alarm recovery");
            count_recovery = millis();
        }
        else if(millis() - count_recovery > loop_count)
        {
            //Serial.println("Reset alarm recovery");
            count_protect = 0;
            iCountNormalLoop = 0;
        }
    }
}

/// @brief stop = true stop_motor , false free_motor
void MagKicker::processCurrentAlarmCheck(bool stop)
{
    float current = readCurrentSens();
    if(current>=current_Protect)
    {
        count_protect++;
        if(count_protect <= 1)
        {
            //Serial.println("Start count alarm");
            count_alarm = millis();
        }   
        else if(millis()-count_alarm>40)
        {
            //Serial.println("Set alarm");
            Alarm = setAlarm(true);
            _pin_mag.set_pwm(0);
            if(stop)
            {
                stopMotion();
            }
        }
    }
}

void MagKicker::unHandleProcess()
{
    state_machine_mag = MagKickState::CheckIsHoming;
    alarmRecovery(500);
    readPlatform();
    processCurrentAlarmCheck(true);
    if(!Alarm)
    {
        if(finishHandleprocess)
        {
            statusFinishProcess(false);
        }
        finishHandleprocess = false;
        _pin_mag.set_pwm(pulseWidth);
        goHome(readSensHome());
    }
    else
    {

        if(readAlarm())
        {
            statusFinishProcess(false);
            _pin_mag.set_pwm(pulseWidth);
            Alarm = setAlarm(false);
            finishHandleprocess = false;
            count_protect = 0;
        }
    }
}

void MagKicker::handleProcess()
{
    if(!Alarm)
    {
        alarmRecovery(300);
        processCurrentAlarmCheck(false);
    }
    if(!Alarm)
    {
        if(readPlatform())
        {
            _pin_mag.set_pwm(0);
        }
        else
        {
            bool home_sens = readSensHome();
            bool limit_sens = readSensLimit();
            //Serial.println("SENSOR Home :="+ String(home_sens)+" limit :="+String(limit_sens));
            switch (state_machine_mag)
            {
                case MagKickState::CheckIsHoming:
                    if(!home_sens)
                    { 
                        //Serial.println("0 Not Homming");
                        state_machine_mag = MagKickState::CheckIsHoming;
                        moveToHome();
                        _pin_mag.set_pwm(pulseWidth);

                    }
                    else
                    {
                        //Serial.println("0 Homming");
                        state_machine_mag = MagKickState::MoveToLimit;
                        _pin_mag.set_pwm(0);
                        moveToLimit();
                        _pin_mag.set_pwm((pulseWidth/3)*2);
                        last_moving = millis();
                    }
                    break;
                case MagKickState::MoveToLimit:
                    if(millis() - last_moving > 500)
                    {
                        if(!limit_sens)
                        {
                            //Serial.println("1 Go limit");
                            state_machine_mag = MagKickState::MoveToLimit;
                            moveToLimit();
                            _pin_mag.set_pwm(pulseWidth);
                        }
                        else
                        {
                            //Serial.println("1 Islimit");
                            state_machine_mag = MagKickState::GoBackToHome;
                            _pin_mag.set_pwm(0);
                            
                            moveToHome();
                            _pin_mag.set_pwm((pulseWidth/3)*2);
                            last_moving = millis();
                        }
                    }
                    break;
                case MagKickState::GoBackToHome:
                    if(millis() - last_moving > 500)
                    {
                        if(!home_sens)
                        {
                            //Serial.println("2 goback home");
                            state_machine_mag = MagKickState::GoBackToHome;
                            moveToHome();
                            _pin_mag.set_pwm(pulseWidth);
                        }
                        else
                        {
                            //Serial.println("2 is homming");
                            state_machine_mag = MagKickState::Finished;
                            _pin_mag.set_pwm(0);
                            //Serial.println("3 Finished");
                        }
                    }
                case MagKickState::Finished:
                    finishHandleprocess = true;
                    _pin_mag.set_pwm(pulseWidth);
                    statusFinishProcess(finishHandleprocess);
                    goHome(readSensHome());
            }
        }
    }
}

void MagKicker::control()
{
    if(readHandle())
    {
      // Serial.println("Handle");
        handleProcess();
    }
    else
    {
        //Serial.println("Un Handle");
        unHandleProcess();
    }
}

#ifndef MAGKICKER_H
#define MAGKICKER_H

#include <Arduino.h>
#include "GPIO/MagKickerPin.h"

class MagKicker
{
    private:
        MagKickerPin _pin_mag;
        float r_sense = 0.5;
        float max_analog_resolution = 4095.0;//1023.0;
        enum MagKickState {
            CheckIsHoming = 0,
            MoveToLimit = 1,
            WatiState = 2,
            GoBackToHome = 3,
            Finished = 4,
        };


        uint8_t pulseWidth = 255;
        bool Alarm = false;
        bool PlatefromFull = false;
        MagKickState state_machine_mag = MagKickState::CheckIsHoming;
        int count_protect = 0;
        bool finishHandleprocess = false;
        int iCountNormalLoop = 0;
        float current_Protect = 0.50;
        unsigned long last_moving = 0;
        unsigned long count_recovery = 0;
        unsigned long count_alarm = 0;





        bool setAlarm(bool alarm);
        void goHome(bool SensorHM);
        void statusFinishProcess(bool status);
        void stopMotion();
        void moveToHome();
        void moveToLimit();
        bool readHandle();
        bool readSensHome();
        bool readSensLimit();
        bool readAlarm();
        bool readPlatform();
        float readCurrentSens();
        void alarmRecovery(unsigned long loop_count);
        void processCurrentAlarmCheck(bool stop);
        void unHandleProcess();
        void handleProcess();
    public:
        MagKicker(MagKickerPin pin_mag);
        void control();

};

#endif  // MAGKICKER_H

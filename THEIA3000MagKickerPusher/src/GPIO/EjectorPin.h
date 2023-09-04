#ifndef EJECTORPIN_H
#define EJECTORPIN_H

#include <Arduino.h>

class EjectorPin {
public:
   /*OUTPUT*/
    uint8_t op_FWD = 22;         //MotorForward
    uint8_t op_REV = 23;         //MotorReverse
    uint8_t op_Status = 24;      //FinishProcess
    uint8_t op_PWM = 4;          //PWM_Connect Enable

    /*INPUT*/
    uint8_t ip_Sen_HOME = 25;    //SensorHOME
    uint8_t ip_Sen_Limit = 19;   //SensorLIMIT
    uint8_t ip_Control = 27;     //ControlByIO 


    /*Control Length*/
    uint8_t ip_bit_L1 = 28;    //Bit 1
    uint8_t ip_bit_L2 = 29;    //bit 2

    EjectorPin(bool init = false);
    void init_io();
};

#endif  // EJECTORPIN_H

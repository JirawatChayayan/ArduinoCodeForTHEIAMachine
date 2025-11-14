#ifndef GRIPPERPIN_H
#define GRIPPERPIN_H

#include <Arduino.h>

class GripperPin {
public:
   /*INPUT*/
    uint8_t ip_1 = 13;         
    uint8_t ip_2 = 12; 
    uint8_t ip_3 = 11;
    uint8_t ip_4 = 10;  

    /*OUTPUT*/
    uint8_t op_1 = A8;    
    uint8_t op_2 = A9;   
    uint8_t op_3 = A10;
    uint8_t op_4 = A11;     

    GripperPin(bool init = false);
    void init_io();
};

#endif  // GRIPPERPIN_H

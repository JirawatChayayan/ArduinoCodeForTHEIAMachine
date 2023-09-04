#include <Arduino.h>
#include "EjectorPin.h"

EjectorPin::EjectorPin(bool init) {
    if (init) {
        init_io();
    }
}

void EjectorPin::init_io() {
    // Output
    pinMode(op_FWD, OUTPUT);
    pinMode(op_REV, OUTPUT);
    pinMode(op_Status, OUTPUT);
    pinMode(op_PWM, OUTPUT);
    
    // Input
    pinMode(ip_Sen_HOME, INPUT);
    pinMode(ip_Sen_Limit, INPUT);
    pinMode(ip_Control, INPUT);
    
    // Control Length
    pinMode(ip_bit_L1, INPUT_PULLUP);
    pinMode(ip_bit_L2, INPUT_PULLUP);
    
    digitalWrite(op_FWD, LOW);
    digitalWrite(op_REV, LOW);
    digitalWrite(op_Status, LOW);
    digitalWrite(op_PWM, LOW);
}

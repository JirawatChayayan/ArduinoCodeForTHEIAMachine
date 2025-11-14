#include <Arduino.h>
#include "GripperPin.h"

GripperPin::GripperPin(bool init) {
    if (init) {
        init_io();
    }
}

void GripperPin::init_io() {
    // Output
    pinMode(op_1, OUTPUT);
    pinMode(op_2, OUTPUT);
    pinMode(op_3, OUTPUT);
    pinMode(op_4, OUTPUT);
    
    pinMode(ip_1, INPUT_PULLUP);
    pinMode(ip_2, INPUT_PULLUP);
    pinMode(ip_3, INPUT_PULLUP);
    pinMode(ip_4, INPUT_PULLUP);
    
    digitalWrite(op_1, LOW);
    digitalWrite(op_2, LOW);
    digitalWrite(op_3, LOW);
    digitalWrite(op_4, LOW);
}

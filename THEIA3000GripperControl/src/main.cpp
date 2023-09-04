#include <Arduino.h>
#include "Gripper/Gripper.h"

Gripper gripper1(3,A5,0);
Gripper gripper2(5,A4,10);
Gripper gripper3(6,A3,20);
Gripper gripper4(9,A2,30);

String Allkey = "";
String PWM_CH = "";
String ACTIVE_WIDTH = "";
String HOLD_WIDTH = "";
String TimeChangeWidthToHold = "";
int i = 0;

void reset_serial_param()
{
  Allkey = "";
  PWM_CH = "";
  ACTIVE_WIDTH = "";
  HOLD_WIDTH = "";
  TimeChangeWidthToHold = "";
  i = 0;
}

void serialEvent()
{
  while(Serial.available()>0)
  {
    char A = (char)Serial.read();
    Allkey +=A; 
    if(A != ':'&& A!='\n')
    {
      switch(i)
      {
        case 1:
          PWM_CH += A;
          break;
        case 2:
          ACTIVE_WIDTH += A;
          break;
        case 3:
          HOLD_WIDTH += A;
          break;
        case 4:
          TimeChangeWidthToHold += A;
          break;
      }
    }
    if(A == ':')
    {
      i++;
    }
    if(A == '\n')
    {
      int _pwm_ch = PWM_CH.toInt();
      int _active_width = ACTIVE_WIDTH.toInt(); 
      int _hold_width = HOLD_WIDTH.toInt(); 
      long _timer = atol(TimeChangeWidthToHold.c_str());
      reset_serial_param();
      switch(_pwm_ch)
      {
        case 1:
          gripper1.set_value(_active_width,_hold_width,_timer);
          break;
        case 2:
          gripper2.set_value(_active_width,_hold_width,_timer);
          break;
        case 3:
          gripper3.set_value(_active_width,_hold_width,_timer);
          break;
        case 4:
          gripper4.set_value(_active_width,_hold_width,_timer);
          break;
      }
    }
  }
}

void start_serial()
{
  Serial.begin(115200);
  delay(10);
  Serial.println("******************************************Gripper PWM*********************************");
  Serial.println("*  PWMOutPutPin  : (PWM1 = 3) ,(PWM2 = 5) ,(PWM3 = 6) ,(PWM4 = 9)                    *");
  Serial.println("*  PWMControlPin : (PWM1 = A5),(PWM2 = A4),(PWM3 = A3),(PWM4 = A2)                   *");
  Serial.println("*  Config        : Command  :PWM_CH:ACTIVE_WIDTH:HOLD_WIDTH:TimeChangeWidthToHold;   *");
  Serial.println("*  Example       : Command  :1:185:90:2000;   TimeChangeWidthToHold  cal   1 = 1ms   *");
  Serial.println("*  ComPort       : COM30                                                             *");
  Serial.println("**************************************************************************************");
}

void initial_parameter()
{
  gripper1.read_eeprom();
  gripper2.read_eeprom();
  gripper3.read_eeprom();
  gripper4.read_eeprom();
}

void setup() 
{
  start_serial();
  initial_parameter();
}

void loop() {
  gripper1.control();
  gripper2.control();
  gripper3.control();
  gripper4.control();
}

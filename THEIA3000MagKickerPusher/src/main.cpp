#include <Arduino.h>
#include <avr/wdt.h>
#include <avr/io.h>
#include "GPIO/MagKickerPin.h"
#include "GPIO/EjectorPin.h"
#include "Ejector/Ejector.h"
#include "MagKicker/MagKicker.h"

MagKickerPin mag_L_pin(MagSide::Left);
MagKickerPin mag_R_pin(MagSide::Right);
EjectorPin ejector_pin(true);
Ejector ejector(ejector_pin);
MagKicker magkick_L(mag_L_pin);
MagKicker magkick_R(mag_R_pin);

unsigned long scanStartTime = 0;
unsigned long totalScanTime = 0;
unsigned int numScans = 0;
unsigned long lastUpdateTime = 0;
float averageScanTime = 0;

unsigned long T_update = millis();


String inputStr;
String cmd_input;
String cmd_name = "";
String cmd_value = "";

void setup() 
{
  wdt_enable(WDTO_500MS);
  Serial.begin(115200);
  delay(10);
  Serial.println("***********  THEIA3000 MagKicker Pusher  **********");
  Serial.println("*  ScanTime      : ~3000us                        *");
  Serial.println("*  ComPort       : COM14                          *");
  Serial.println("***************************************************");
  magkick_L.ns = "l";
  magkick_R.ns = "r";
}

void calculate_scantime()
{
  unsigned long scanEndTime = micros();  // Record the end time of the loop iteration
  unsigned long scanTime = scanEndTime - scanStartTime;  // Calculate the time taken for the loop iteration
  
  totalScanTime += scanTime;  // Accumulate the scan time
  numScans++;  // Increment the number of scans
  
  unsigned long currentTime = millis();
  if (currentTime - lastUpdateTime >= 1000) {
    if (numScans > 0) {
      averageScanTime = (totalScanTime / numScans)/1000.0;  // Calculate the average scan time
      
      // Serial.println("Average Scan Time: " + String(averageScanTime) + " microseconds");
    }
    
    totalScanTime = 0;  // Reset the total scan time
    numScans = 0;  // Reset the number of scans
    lastUpdateTime = currentTime;  // Update the last update time
  }
}



void separate_cmd(String cmd,String seprator)
{
  cmd_name = "";
  cmd_value = "";
  int cmd_index = cmd.indexOf(seprator);
  if(cmd_index != -1)
  {
    cmd_name = cmd.substring(0,cmd_index);
    cmd_value = cmd.substring(cmd_index+1);
  }
  else
  {
    cmd_name = cmd;
    cmd_value = "";
  }
}

void serialHandle()
{
  separate_cmd(cmd_input,"=");
  if(cmd_name == "e_l")
  {
    ejector.set_length(cmd_value);
  }
  else if(cmd_name == "e_c")
  {
    ejector.set_control(cmd_value == "1");
  }
  else if(cmd_name == "m_l")
  {
    magkick_L.set_control(cmd_value == "1");
  }
  else if(cmd_name == "m_r")
  {
    magkick_R.set_control(cmd_value == "1");
  }
}

void serialEvent()
{
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    if(inChar == '\r' || inChar == '\n')
    {

    }
    else if(inChar == ';')
    {
      cmd_input = inputStr;
      inputStr = "";
      serialHandle();
    }
    else
    {
      inputStr += inChar;
    }
  }
}


void update_status()
{
    unsigned long T_now = millis();
    if(T_now - T_update > 200)
    {
      T_update = T_now;
      String msg = ejector.update()+";"+magkick_L.update()+";"+magkick_R.update()+";"+String(averageScanTime)+"ms";
      Serial.println(msg);
    }
   
}

void loop()
{
  scanStartTime = micros();  // Record the start time of the loop iteration
  ejector.control();
  magkick_L.control();
  magkick_R.control();
  update_status();
  calculate_scantime();
  
  wdt_reset();
}


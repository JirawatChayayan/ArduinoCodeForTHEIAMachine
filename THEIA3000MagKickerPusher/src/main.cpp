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

void setup() 
{
  wdt_enable(WDTO_500MS);
  Serial.begin(115200);
  delay(10);
  Serial.println("***********  THEIA3000 MagKicker Pusher  **********");
  Serial.println("*  ScanTime      : ~3000us                        *");
  Serial.println("*  ComPort       : COM14                          *");
  Serial.println("***************************************************");
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
      unsigned long averageScanTime = totalScanTime / numScans;  // Calculate the average scan time
      
      Serial.println("Average Scan Time: " + String(averageScanTime) + " microseconds");
    }
    
    totalScanTime = 0;  // Reset the total scan time
    numScans = 0;  // Reset the number of scans
    lastUpdateTime = currentTime;  // Update the last update time
  }
}

void loop()
{
  scanStartTime = micros();  // Record the start time of the loop iteration
  
  ejector.control();
  magkick_L.control();
  magkick_R.control();
  
  calculate_scantime();
  wdt_reset();
}


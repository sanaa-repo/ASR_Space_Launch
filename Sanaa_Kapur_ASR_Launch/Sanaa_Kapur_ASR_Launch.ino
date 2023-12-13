//Arduino Mega ASR Launch Code
//Author: @Sanaa Kapur

//Imported Classes
#include <Wire.h> //Needed for I2C to GNSS
#include <SparkFun_u-blox_GNSS_Arduino_Library.h> //http://librarymanager/All#SparkFun_u-blox_GNSS
#include <SPI.h>
#include <SD.h>
#include <SparkFun_RHT03.h>

//Initialized Local Variables
SFE_UBLOX_GNSS myGNSS;
RHT03 rht;
int lastTime = 0; 


void setup()
{
  Serial.begin(115200); // set baud rate

  if (myGNSS.begin() == false) //if there is no connection to the GPS
  {
    Serial.println(F("u-blox GNSS not detected at default I2C address. Please check wiring. Freezing."));
    while (1);
  }
 
  rht.begin(A1);   //Start Humidity sensor

  //Initialization completion printouts
  Serial.println("the start time is..");
  Serial.println(__TIME__);
  Serial.println("Setup Complete");
}

void loop()
{
  //Ask for data  only every 5 seconds. Doing it more often will just cause I2C traffic.
  if (millis() - lastTime > 5000)
  {
    lastTime = millis(); //Update the timer

    //time stamp    
    Serial.println("Time Stamp(after start)");
    Serial.println(millis()/1000);

    //GPS Code
    Serial.println("GPS Section");
    long latitude = myGNSS.getLatitude();
    Serial.print(F("Lat: "));
    Serial.print(latitude);

    long longitude = myGNSS.getLongitude();
    Serial.print(F(" Long: "));
    Serial.print(longitude);
    Serial.print(F(" (degrees * 10^-7)"));

    long altitude = myGNSS.getAltitude();
    Serial.print(F(" Alt: "));
    Serial.print(altitude);
    Serial.println(F(" (mm)"));

    //Humidity Sensor Code
    Serial.println("Humidity Sensor");
    int humidity = analogRead(A1);
    float voltage= humidity * (5.0 / 1023.0);
    float percentRH = (voltage - 0.958)/0.0307; 
    Serial.println(percentRH);
    Serial.println();
  }
}

/***************************************************************************
  This is a library for the APDS9960 digital proximity, ambient light, RGB, and gesture sensor

  This sketch puts the sensor in proximity mode and enables the interrupt
  to fire when proximity goes over a set value

  Designed specifically to work with the Adafruit APDS9960 breakout
  ----> http://www.adafruit.com/products/3595

  These sensors use I2C to communicate. The device's I2C address is 0x39

  Adafruit invests time and resources providing this open source code,
  please support Adafruit andopen-source hardware by purchasing products
  from Adafruit!

  Written by Dean Miller for Adafruit Industries.
  BSD license, all text above must be included in any redistribution
 ***************************************************************************/

#include "Adafruit_APDS9960.h" //include Adafruit library @ https://github.com/adafruit/Adafruit_APDS9960
#include <xCore.h> //include core library @ https://github.com/xinabox/xCore
#include <xOD01.h> //include sensor library @ https://github.com/xinabox/xOD01

#if defined(ESP8266)
#define RED   RED
#define GREEN GREEN
#define BLUE  BLUE
#include <ESP8266WiFi.h>
#define Serial Serial

#elif defined(ARDUINO_SAMD_ZERO)
#define RED   CC03_RED
#define GREEN CC03_GREEN
#define BLUE  CC03_BLUE
#define Serial SerialUSB

#elif defined(ARDUINO_AVR_PRO)
#define RED   CC01_RED
#define GREEN CC01_GREEN
#define BLUE  CC01_BLUE
#define Serial Serial

#elif defined(ESP32)
#define RED   CW02_RED
#define GREEN CW02_GREEN
#define BLUE  CW02_BLUE
#define Serial Serial
#endif


//the pin that the interrupt is attached to
#define INT_PIN 3

//create the APDS9960 object
Adafruit_APDS9960 apds;

void setup() {

  #if defined(ESP8266)
  WiFi.forceSleepBegin();
  Wire.begin(2, 14);
  Wire.setClockStretchLimit(15000);

#elif defined(ARDUINO_SAMD_ZERO)
  Wire.begin();

#elif defined(ARDUINO_AVR_PRO)
  Wire.begin();

#elif defined(ESP32)
  Wire.begin();

#endif

  OLED.begin();
  Serial.begin(115200);
  pinMode(INT_PIN, INPUT_PULLUP);

  if(!apds.begin()){
    Serial.println("failed to initialize device! Please check your wiring.");
  }
  else OD01.println("Device initialized!");

  //enable proximity mode
  apds.enableProximity(true);

  //set the interrupt threshold to fire when proximity reading goes above 175
  apds.setProximityInterruptThreshold(0, 175);

  //enable the proximity interrupt
  apds.enableProximityInterrupt();
}

void loop() {

  //print the proximity reading when the interrupt pin goes low
  if(!digitalRead(INT_PIN)){
    OD01.println(apds.readProximity());

    //clear the interrupt
    apds.clearInterrupt();
  }
}

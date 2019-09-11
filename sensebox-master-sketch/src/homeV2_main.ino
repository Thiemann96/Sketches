#include "senseBoxConfig.h"

#ifdef HDC1080_CONNECTED
    #include "HDC1080/HDC1080.h"
    #include "HDC1080/HDC1080.cpp"
#endif
#ifdef BMP280_CONNECTED
    #include "BMP280/BMP280.h"
    #include "BMP280/BMP280.cpp"
#endif
#ifdef LIGHT_CONNECTED 
    #include "Light/LIGHT.h"
    #include "Light/LIGHT.cpp"
#endif
#ifdef SDS_CONNECTED 
    #include "SDS/SDS.h"
    #include "SDS/SDS.cpp"
#endif
#ifdef DISPLAY_CONNECTED
    #include "Sb_display/Sb_display.h"
    #include "Sb_display/Sb_display.cpp"
#endif
#ifdef GPS_CONNECTED
    #include "GPS/GPS.h"
    #include "GPS/GPS.cpp"
#endif
#ifdef SD_CONNECTED
    #include "SDCARD/SDCARD.h"
    #include "SDCARD/SDCARD.cpp"
#endif
#include "System_functions/System_functions.h"
#include "System_functions/System_functions.cpp"

void setup(){
    #ifdef POWER_SAVE
        senseBoxIO.powerXB2(false);
    #endif
    MCU.startAllSensors();
    // test for wifi bee
    MCU.checkForWINC1500();
    // if test has passed connect to WiFi
    MCU.connectToWlan();
    MCU.testSensors();
}
void loop(){
    unsigned long start = millis();
    MCU.readSensorsAndSendData();
    // schedule next round of measurements
    Serial.println("Enaging power saving mode now!");
    #ifdef POWER_SAVE
        senseBoxIO.powerI2C(false);
        senseBoxIO.powerUART(false);
    #endif
    for (;;) 
    {
        unsigned long now = millis();
        unsigned long elapsed = now - start;
        if (elapsed >= postingInterval)
        {
            #ifdef POWER_SAVE
                senseBoxIO.powerI2C(true);
                senseBoxIO.powerUART(true);
                delay(250);
                MCU.startAllSensors();
                Serial.println("Warming up! SDS needs 30 seconds!");
                delay(30000);
                Serial.println("Warm up done!");
            #endif
            return;
        }
    }

    }
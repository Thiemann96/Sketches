#include "senseBoxConfig.h"

#ifdef HDC1080_CONNECTED
    #include "./HDC1080/HDC1080.h"
    #include "./HDC1080/HDC1080.cpp"
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
    #include "./Sb_display/Sb_display.h"
    #include "./Sb_display/Sb_display.cpp"
#endif
#ifdef SD_CONNECTED
    #include "senseBoxSD/senseBoxSD.h"
#endif
#include "System_functions/System_functions.h"
#include "System_functions/System_functions.cpp"

const long interval = 30000;

void setup(){
    #ifdef HDC1080_CONNECTED
        hdc.begin();
    #endif
    #ifdef BMP280_CONNECTED
        bmp.begin();
    #endif
    #ifdef LIGHT_CONNECTED
        veml.begin();
        tsl.begin();
    #endif
    #ifdef SDS_CONNECTED
        SDS.begin(9600);
    #endif
    MCU.checkForWINC1500();
    // if test has passed connect to WiFi
    MCU.connectToWlan();
    MCU.testSensors();
}
void loop(){
    Serial.println("going to sleep now");
    senseBoxIO.powerUART(false);
    senseBoxIO.powerI2C(false);
    senseBoxIO.powerXB2(false);
    delay(10000);
    Serial.println("Waking up");
    senseBoxIO.powerAll();
    delay(2000);
    MCU.readSensorsAndSendData();
    }
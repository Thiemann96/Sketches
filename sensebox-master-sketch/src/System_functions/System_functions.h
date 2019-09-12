#ifndef sys_func_H
#define sys_func_H
#include "../senseBoxIO/src/senseBoxIO.h"
#include "../senseBoxConfig.h"
// Ethernet mit einbinden
// Tests für jeden Sensor einbinden 
// Status an LED ablesen

class System_functions{

    private:
        void sendData();
        void checkWirePort(byte &address);
        int checkSerialPort(HardwareSerial &uart);
        void addMeasurementToCsv(const char *sensorId, float measurement);
        void addMeasurementToCsv(const char *sensorId,int measurement);
        void addMeasurementToCsv(const char *sensorId,long measurement);
  
    public:
        void startAllSensors();
        void connectToWlan();
        void testSensors();
        void readSensorsAndSendData();
        void checkForWINC1500();


};
int measuredPhenomena = 0;
// Display interval 
const long display_interval = 2000;
long time_start = 0;
long time_actual = 0;
unsigned long lastConnectionTime = 0 ;
int displayCounter = 0; 

/******* Network Client Setup *******/
#if defined AUTH_TOKEN
  WiFiSSLClient senseBox_client;
  // andere Port 443 / nicht 80
#else
  WiFiClient senseBox_client;
#endif
String sensorDataCsv = "";
System_functions MCU;
#endif
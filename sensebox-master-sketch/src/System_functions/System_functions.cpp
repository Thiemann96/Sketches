#include "System_functions.h"
void System_functions::connectToWlan(){
    senseBoxIO.statusNone();
  uint8_t status = WL_IDLE_STATUS;
  do{
    #ifdef DEBUG_MODE
        Serial.print("Attempting to connect to SSID: ");
        Serial.println(SSID);
     #endif
     senseBoxIO.statusGreen();
     delay(250);
     senseBoxIO.statusNone();
     delay(250);
     status = WiFi.begin(SSID, PW);
     senseBoxIO.statusGreen();
     delay(250);
     senseBoxIO.statusNone();
     delay(250);
  }while (status != WL_CONNECTED);
  #ifdef DEBUG_MODE
      Serial.println("Connection Successful");
  #endif
  senseBoxIO.statusNone();
  senseBoxIO.statusGreen();
}

int System_functions::checkSerialPort(HardwareSerial &uart){
    int sds_data;
    for (int i = 1; i < 3; i++){
      sds_data = 0;
      while (uart.available() > 0){
        byte buffer = uart.read();
        sds_data += int(buffer);
      }
      delay(1000);
    }
    delay(250);
    if (sds_data > 1){
      #ifdef SDS_CONNECTED
        sds_test_passed = true;
      #endif
      return 1;
    }
    else return 0;
  
}
void System_functions::checkWirePort(byte &address){
    if((address == 0) || (address > 127)) return;
   switch(address){
    case 0x29: 
    #if defined LIGHT_CONNECTED
      tsl.begin();
      tsl_test_passed = true;
      #ifdef DEBUG_MODE
        Serial.println("-TSL45315 connected.");
      #endif
      measuredPhenomena+=1;
    #else
      #ifdef DEBUG_MODE
        Serial.println("-Attention: TSL45315 found but not defined in hardware setup.");
      #endif
    #endif
      break;
    case 0x38:
    //case 0x39:
    #if defined LIGHT_CONNECTED
      #ifdef DEBUG_MODE
        Serial.println("-VEML6070 connected.");
      #endif
      veml.begin();
      veml_test_passed = true;
      measuredPhenomena+=1;
    #else
      #ifdef DEBUG_MODE
        Serial.println("-Attention: VEML6070 found but not defined in hardware setup.");
      #endif
    #endif
      break;
    case 0x40:  //HDC1080
    case 0x41:
    //case 0x42:
    case 0x43:
    #if defined HDC1080_CONNECTED
      hdc.begin();
      hdc_test_passed = true;
      #ifdef DEBUG_MODE
        Serial.println("-HDC1080 connected.");
      #endif
      measuredPhenomena+=2;
    #else
      #ifdef DEBUG_MODE
        Serial.println("-Attention: HDC1080 found but not defined in hardware setup.");
      #endif
    #endif
      break;
    case 0x76:
    case 0x77:
    #if defined BMP280_CONNECTED
      bmp.begin();
      bmp_test_passed = true;
      #ifdef DEBUG_MODE
        Serial.println("-BMP280 connected.");
      #endif
      measuredPhenomena+=1;

    #else
      #ifdef DEBUG_MODE
        Serial.println("-Attention: BMP280 found but not defined in hardware setup.");
      #endif
    #endif
      break;
  }
  delay(250);
}

void System_functions::testSensors(){
  // Check sensor communication //
  // Check if SDS connected //
  #if defined SDS_CONNECTED
    #ifdef DEBUG_MODE
      Serial.println("\nChecking UART port...");
    #endif
    //Scan serial ports for connection//
    senseBoxIO.powerUART(false);
    delay(250);
    senseBoxIO.powerUART(true);
    delay(5000);
    SDS_CONNECTED.begin(9600);
    if (checkSerialPort(SDS_CONNECTED) == 1)
    {
      #ifdef DEBUG_MODE
        Serial.println("-SDS011 connected.");
        measuredPhenomena+=2;
        delay(1000);
      #endif
    }
    else{
      #ifdef DEBUG_MODE
        Serial.println("-Warning: SDS011 not found on specified port.");
        delay(2000);
      #endif

      SDS_CONNECTED.end();
      //senseBoxIO.powerUART(false);
      
    }
  #endif
  #ifdef DEBUG_MODE
    Serial.println("\nChecking I2C ports...");
    delay(100);
  #endif
    // Init and scan I2C ports for connected sensors //
    senseBoxIO.powerI2C(false);
    delay(250);
    senseBoxIO.powerI2C(true);
    delay(250);
    Wire.begin();
    byte devices = 0;
    for(byte address = 1; address < 127; address++ )
    {
      Wire.beginTransmission(address);
      byte error = Wire.endTransmission();
      if(error == 0)
      {
        devices++;
      #ifdef DEBUG_MODE
          Serial.print("-Device found at 0x");
          Serial.println(address, HEX);
          delay(500);
      #endif
        checkWirePort(address);
      }
      else if (error ==4)
      {
      #ifdef DEBUG_MODE
          Serial.print("\n-Unknown Error at 0x");
          Serial.println(address, HEX);
          delay(1000);
      #endif
      }
    }
  }


void System_functions::sendData(){
    senseBox_client.stop();
  String httpRequestUrl = "POST /boxes/" + String(SENSEBOX_ID) + "/data HTTP/1.1";
  int contentLength = sensorDataCsv.length();
  #ifdef DEBUG_MODE
    Serial.println("\nCalling openSenseMap...");
    delay(500);
  #endif
  #if defined AUTH_TOKEN
    if (senseBox_client.connect(server, 443)) //port 443 for SSL and 80 for standart
  #else
    if (senseBox_client.connect(server, 80))
  #endif
  {
    lastConnectionTime = millis();
    #ifdef DEBUG_MODE
      Serial.println("-Connection established.\n");
      Serial.print("Sending HTTP request: ");
      Serial.println(httpRequestUrl);
      delay(1000);
    #endif
  // Construct HTTP header//
    senseBox_client.println(httpRequestUrl);
    senseBox_client.print  ("Host: ");senseBox_client.println(server);
  #if defined AUTH_TOKEN
    senseBox_client.print("Authorization: Basic ");senseBox_client.println(AUTH_TOKEN);
  #endif
    senseBox_client.println("Content-Type: text/csv");
    senseBox_client.println("Connection: close");
    senseBox_client.print  ("Content-Length: ");
    senseBox_client.println(contentLength);
    senseBox_client.println();
  // Send the data (CSV) //
    senseBox_client.println(sensorDataCsv);
    senseBox_client.println();
    #ifdef DEBUG_MODE
      Serial.println("-Done. Waiting for server response...\n");
      delay(500);
    #endif
    // Reading answer from server //
    while (senseBox_client.available()){
        char c = senseBox_client.read();
        Serial.write(c);
      }
  }
  else{
  // If connection failed report //
  #ifdef DEBUG_MODE
    Serial.println("-Connection failed.");
    delay(5000);
  #endif
    senseBoxIO.statusRed();
  }
}

void System_functions::addMeasurementToCsv(const char *sensorId, float measurement){
  String newRow = String(sensorId) + ',' + String(measurement) + '\n';
  sensorDataCsv += newRow;
  #ifdef DEBUG_MODE
    Serial.print(newRow);
    delay(500);
  #endif
}

void System_functions::addMeasurementToCsv(const char *sensorId, int measurement){
  String newRow = String(sensorId) + ',' + String(measurement) + '\n';
  sensorDataCsv += newRow;
  #ifdef DEBUG_MODE
    Serial.print(newRow);
    delay(500);
  #endif
}

void System_functions::addMeasurementToCsv(const char *sensorId, long measurement){
  String newRow = String(sensorId) + ',' + String(measurement) + '\n';
  sensorDataCsv += newRow;
  #ifdef DEBUG_MODE
    Serial.print(newRow);
    delay(500);
  #endif
}
[String(parseFloat((json[PM25_ID] / 10).toFixed(1))),String(json["year"])+"-"+String(("0" + json["month"]).slice(-2))+"-"+String(("0" + json["day"]).slice(-2))+"T"+String(("0" + json["hour"]).slice(-2))+":"+String(("0" + json["minute"]).slice(-2))+":"+String(("0" + json["second"]).slice(-2))+"Z",[json["lng1"]+json["lng2"]/10000,json["lat1"]+json["lat2"]/10000]];

void System_functions::readSensorsAndSendData(){
  #ifdef DEBUG_MODE
    Serial.println("\n\n------------------------------------------------------");
    Serial.println("Reading sensor data...");
    delay(1000);
  #endif
  sensorDataCsv = "";
/* ------------------------------------------------------------------------- */
/* - Use addMeasurementToCsv(*SENSOR_ID , MEASUREMENT) to extend CSV file. - */
/* -------- Put your code inside this block for manual sensor setup -------- */
/* ------------------------------------------------------------------------- */


/* --------------------------------------------------------------------- */
  #if defined HDC1080_CONNECTED
    if (hdc_test_passed){
      #ifdef DEBUG_MODE
        Serial.print("HDC...");
      #endif
      temperature = hdc.getTemperature();
      addMeasurementToCsv(TEMPSENSOR_ID, temperature);
      humidity = hdc.getHumidity();
      addMeasurementToCsv(RELLUFSENSOR_ID, humidity);
    }
  #endif
  #if defined BMP280_CONNECTED
    if (bmp_test_passed){
      #ifdef DEBUG_MODE
        Serial.print("BMP...");
      #endif
      temp = bmp.getTemperature();
      pressure = bmp.getPressure()/100; //pressure in hPa
      addMeasurementToCsv(LUFTDRSENSOR_ID, pressure);
    }
  #endif
  #if defined LIGHT_CONNECTED
    if (tsl_test_passed){
      #ifdef DEBUG_MODE
       Serial.print("TSL...");
      #endif
      illuminance = long(tsl.getIlluminance());
      addMeasurementToCsv(BELEUCSENSOR_ID, illuminance);
    }
    if (veml_test_passed){
      #ifdef DEBUG_MODE
        Serial.print("VEML...");
      #endif
      uvIntensity = veml.getUvIntensity();
      addMeasurementToCsv(UVINTENSENSOR_ID, uvIntensity);
    }
  #endif
  #if defined SDS_CONNECTED
    if(sds_test_passed){
      #ifdef DEBUG_MODE
        Serial.print("SDS...");
      #endif
      uint8_t attempt = 0;
      while (attempt < 5) {
        bool sds_error = SDS.read(&pm25, &pm10);
        if (!sds_error) {

          addMeasurementToCsv(PM10SENSOR_ID, pm10);
          addMeasurementToCsv(PM25SENSOR_ID, pm25);
          break;
        }
        attempt++;
      }
    }
  #endif
// Check if there is data available //
  if (sensorDataCsv.length() == 0){
    #ifdef DEBUG_MODE
      Serial.println("\n-Can not continue. No sensor data available!");
    #endif
    while (true){
      senseBoxIO.statusRed();
      delay(100);
      senseBoxIO.statusNone();
      delay(100);
    }
  }
  else
  {
    #ifdef DEBUG_MODE
      Serial.println("Done.");
    #endif
    sendData(); //send CSV to openSenseMap
  }

}

void System_functions::checkForWINC1500(){

  senseBoxIO.powerXB1(false);
  delay(250);
  senseBoxIO.powerXB1(true);
    //Blink red if bee not found
  if(WiFi.status() == WL_NO_SHIELD)
  {
    WiFi.end();
    senseBoxIO.powerXB1(false);
    #ifdef DEBUG_MODE
      Serial.println("No WiFi Module found. Please power off, check connection and restart device!");
    #endif
    while(true){
      senseBoxIO.statusRed();
      delay(1000);
      senseBoxIO.statusNone();
      delay(1000);
    }
  }

}

void System_functions::startAllSensors(){
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
        SDS_CONNECTED.begin(9600);
    #endif
    #ifdef GPS_CONNECTED
        gps.begin();
    #endif


}
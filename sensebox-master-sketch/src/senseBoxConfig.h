#include <SPI.h>
#include <Wire.h>
#include <WiFi101.h>

#ifndef senseBoxConfig_H
#define senseBoxConfig_H

/* Power save mode  */
#define POWER_SAVE
/* Serial Port for SDS */
#define DEBUG_MODE
/* Connected sensors */
#define HDC1080_CONNECTED
// #define BMP280_CONNECTED
// #define LIGHT_CONNECTED
#define SDS_CONNECTED (Serial1)
#define NUM_SENSORS 2
/* WIFI */
#define SSID "Reedu Guest"
#define PW "#we!Rock!Schools!2019"
/*Interval */
#define postingInterval 60000
#define displayInterval 2000
/* OSeM Stuff */
// configs are for this box : https://opensensemap.org/explore/5ca1e336cbf9ae001a6f1d81
#define server "ingress.opensensemap.org" 
#define SENSEBOX_ID "5ca1e336cbf9ae001a6f1d81"
/*Sensor IDS */
 #define TEMPSENSOR_ID "5ca1e336cbf9ae001a6f1d88"
 #define RELLUFSENSOR_ID "5ca1e336cbf9ae001a6f1d87"
//  #define LUFTDRSENSOR_ID "5ca1e336cbf9ae001a6f1d86"
// #define BELEUCSENSOR_ID "5ca1e336cbf9ae001a6f1d85"
// #define UVINTENSENSOR_ID "5ca1e336cbf9ae001a6f1d84"
#define PM10SENSOR_ID "5ca1e336cbf9ae001a6f1d83"
#define PM25SENSOR_ID "5ca1e336cbf9ae001a6f1d82"

#endif

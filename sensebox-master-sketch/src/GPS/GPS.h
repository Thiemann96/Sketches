#ifndef GPS_H
#define GPS_H
#include "TinyGPS++.h"
TinyGPSPlus gps_wrapper;

class GPS
{
    private:

    public:
        void begin(void);
        void getGPS(void);
};

typedef struct GPSLocation {
   double latitude;
   double longitude;
   double altitude;
} GPSLocation;
typedef struct GPSDateTime {
   uint16_t year;
   uint8_t month;
   uint8_t day;
   uint8_t hour;
   uint8_t minute;
   uint8_t second;
} GPSDateTime;
GPSLocation lastGPSLocation;
GPSDateTime lastGPSDateTime;
GPS gps;


#endif
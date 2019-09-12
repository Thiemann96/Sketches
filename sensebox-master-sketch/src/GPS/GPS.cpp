#include "GPS.h"
#include "Wire.h"
void GPS::begin(void)
{
    Wire.begin();
}

void GPS::getGPS(void)
{
  Wire.requestFrom(0x42, 10); 
  while (Wire.available()) {
    gps_wrapper.encode(Wire.read());
  }
}
  
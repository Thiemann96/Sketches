#ifndef LIGHT_H
#define LIGHT_H
#define VEML6070_ADDR 0x38
#define TSL45315_ADDR 0x29
#define VEML6070_INTEGRATION_TIME_1 0x01 //IT_1: 5.625 uW/cm2/step

class VEML6070
{
	public:
		uint8_t begin(void);
		double getUvIntensity(void);
};

class TSL45315
{
	public:
		uint8_t begin(void);
		unsigned long getIlluminance(void);
};
VEML6070 veml;
float uvIntensity;
bool veml_test_passed;
TSL45315 tsl;
long illuminance;
bool tsl_test_passed;
#endif

#ifndef HDC1080_H
#define HDC1080_H
#define HDC1080_ADDR 0x40

class HDC1080
{ //Ausgabe in Fahrenheit
	public:
		uint8_t begin(void);
		double getTemperature(void);
		double getHumidity(void);
};
HDC1080 hdc;
float temperature,humidity;
bool hdc_test_passed = false;
#endif
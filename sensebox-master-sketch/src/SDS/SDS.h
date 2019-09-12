#ifndef SDS_H
#define SDS_H
class SDS011
{
	public:
		SDS011(Stream& serial);
		float getPm10(void);
		float getPm25(void);
		int read(float *p25, float *p10);

	private:
		Stream& sds_data;
		float pm10 = 0;
		float pm25 = 0;
};
float pm10,pm25;
SDS011 SDS(SDS_CONNECTED);
bool sds_test_passed = false;
#endif

float SDS011::getPm10()
{
	this->read(&pm25, &pm10);
	return pm10;
}

float SDS011::getPm25()
{
	this->read(&pm25, &pm10);
	return pm25;
}

SDS011::SDS011(Stream& serial):sds_data(serial) {}

// --------------------------------------------------------
// SDS011:read
// --------------------------------------------------------
int SDS011::read(float *p25, float *p10) {
	byte buffer;
	int value;
	int len = 0;
	int pm10_serial = 0;
	int pm25_serial = 0;
	int checksum_is;
	int checksum_ok = 0;
	int error = 1;
	while ((sds_data.available() > 0) && (sds_data.available() >= (10-len))) {
		buffer = sds_data.read();
		value = int(buffer);
		switch (len) {
			case (0): if (value != 170) { len = -1; }; break;
			case (1): if (value != 192) { len = -1; }; break;
			case (2): pm25_serial = value; checksum_is = value; break;
			case (3): pm25_serial += (value << 8); checksum_is += value; break;
			case (4): pm10_serial = value; checksum_is += value; break;
			case (5): pm10_serial += (value << 8); checksum_is += value; break;
			case (6): checksum_is += value; break;
			case (7): checksum_is += value; break;
			case (8): if (value == (checksum_is % 256)) { checksum_ok = 1; } else { len = -1; }; break;
			case (9): if (value != 171) { len = -1; }; break;
		}
		len++;
		if (len == 10 && checksum_ok == 1) {
			*p10 = (float)pm10_serial/10.0;
			*p25 = (float)pm25_serial/10.0;
			len = 0; checksum_ok = 0; pm10_serial = 0.0; pm25_serial = 0.0; checksum_is = 0;
			error = 0;
		}
		yield();
	}
	return error;
}

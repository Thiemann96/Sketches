uint8_t VEML6070::begin(){
	delay(20);
	Wire.begin();
	Wire.beginTransmission(VEML6070_ADDR);
	Wire.write((VEML6070_INTEGRATION_TIME_1<<2) | 0x02);

	return Wire.endTransmission();
}

double VEML6070::getUvIntensity(){
	byte msb=0, lsb=0;
	uint16_t uv;

	Wire.requestFrom(VEML6070_ADDR+1, 1); //MSB
	delay(1);
	if(Wire.available())
		msb = Wire.read();

	Wire.requestFrom(VEML6070_ADDR+0, 1); //LSB
	delay(1);
	if(Wire.available())
		lsb = Wire.read();

	uv = (msb<<8) | lsb; //output in steps (16bit)
	return (double)(uv)*(5.625);
}

uint8_t TSL45315::begin(){
	Wire.begin();
	Wire.beginTransmission(TSL45315_ADDR);
	Wire.write(0x80|0x00); //0x00 control reg
	Wire.write(0x03); //power on
	Wire.endTransmission();
	delay(20);
	Serial.println("Config...");
	Wire.beginTransmission(TSL45315_ADDR);
	Wire.write(0x80|0x01); //0x01 config reg
	Wire.write(0x00); //M=1 IT=400ms
	// Wire.write(0x01); //M=2 IT=200ms
	// Wire.write(0x02); //M=4 IT=100ms
	return Wire.endTransmission();
}

unsigned long TSL45315::getIlluminance(){
	uint16_t l, h;
	uint32_t lux;

	Wire.beginTransmission(TSL45315_ADDR);
	Wire.write(0x80|0x04); //0x04 reg datalow
	Wire.endTransmission();
	Wire.requestFrom(TSL45315_ADDR, 2); //request 2 bytes
	l = Wire.read();
	h = Wire.read();
	while(Wire.available()){ Wire.read(); } //received more bytes?
	lux  = (h<<8) | (l<<0);
	lux *= 1; //M=1
	// lux *= 2; //M=2
	// lux *= 4; //M=4
	return (unsigned long)(lux);
}
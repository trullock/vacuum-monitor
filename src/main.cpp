#include <Arduino.h>
#include <Adafruit_BMP280.h>
#include <hd44780.h>
#include <hd44780ioClass/hd44780_I2Cexp.h>

#define BMP_SCK  (13)
#define BMP_MISO (12)
#define BMP_MOSI (11)
#define BMP_CS   (10)


hd44780_I2Cexp lcd;
Adafruit_BMP280 bmp; // I2C
//Adafruit_BMP280 bmp(BMP_CS); // hardware SPI
//Adafruit_BMP280 bmp(BMP_CS, BMP_MOSI, BMP_MISO,  BMP_SCK);

void setup() {
	Serial.begin(9600);
	Serial.println(F("BMP280 Forced Mode Test."));

	lcd.begin(16, 2);
	lcd.clear();
	lcd.setCursor(0, 0);
	lcd.print("Initializing...");

	//if (!bmp.begin(BMP280_ADDRESS_ALT, BMP280_CHIPID)) {
	if (!bmp.begin()) {
		Serial.println(F("Could not find a valid BMP280 sensor, check wiring or try a different address!"));
		while (1) 
			delay(10);
	}

	/* Default settings from datasheet. */
	bmp.setSampling(Adafruit_BMP280::MODE_FORCED,     /* Operating Mode. */
					Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
					Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
					Adafruit_BMP280::FILTER_X16,      /* Filtering. */
					Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */
}

void loop() {
	// must call this to wake sensor up and get new measurement data
	// it blocks until measurement is complete
	if (bmp.takeForcedMeasurement()) {
		lcd.print(bmp.readTemperature());
		lcd.println("*C  ");

		lcd.print(bmp.readPressure());
		lcd.println("Pa");


		lcd.setCursor(0, 1);
		lcd.print(bmp.readAltitude(1013.25)); /* Adjusted to local forecast! */
		lcd.println("m");

	} else {
		Serial.println("Forced measurement failed!");
	}
}
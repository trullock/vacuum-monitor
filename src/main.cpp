#include <Arduino.h>
#include <Adafruit_BMP280.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

Adafruit_SSD1306 lcd(128, 64, &Wire, -1);
Adafruit_BMP280 bmp;

void setup() {
	Serial.begin(9600);
	Serial.println(F("BMP280 Forced Mode Test."));

	if(!lcd.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
		Serial.println(F("SSD1306 allocation failed"));
		for(;;);
	}
	
	Serial.println(F("Setting up display"));

	lcd.clearDisplay();
	lcd.setTextSize(1);
	lcd.setTextColor(WHITE);
	lcd.setCursor(0, 10);
	lcd.print("Initializing...");
	lcd.display();

	if (!bmp.begin(BMP280_ADDRESS_ALT, BMP280_CHIPID)) {
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
	
	lcd.clearDisplay();
}

void loop() {
	// must call this to wake sensor up and get new measurement data
	// it blocks until measurement is complete
	if (bmp.takeForcedMeasurement()) {
		lcd.clearDisplay();
		
		lcd.setCursor(0, 0);
		lcd.print(bmp.readTemperature());
		lcd.print("*C  ");

		lcd.print(bmp.readPressure());
		lcd.print("Pa");


		lcd.setCursor(0, 20);
		lcd.print(bmp.readAltitude(1013.25)); /* Adjusted to local forecast! */
		lcd.print("m");
		lcd.display();

		delay(50);
	} else {
		Serial.println("Forced measurement failed!");
	}
}
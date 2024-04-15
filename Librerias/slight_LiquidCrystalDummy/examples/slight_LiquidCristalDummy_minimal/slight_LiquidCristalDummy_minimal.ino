/**************************************************************************************************

	LiquidCrystalDummy
		minimal example for LiquidCrystalDummy library
		debugout on usbserial interface: 115200baud

	hardware:
		Board:
			Arduino compatible
		LCD:
			LiquidCrystal compatible

	libraries used:
		~ LiquidCristal
			arduino internal
		~ slight_LiquidCristalDummy

	written by stefan krueger (s-light),
		git@s-light.eu, http://s-light.eu, https://github.com/s-light/

**************************************************************************************************/
/**************************************************************************************************
	The MIT License (MIT)

	Copyright (c) 2020 Stefan Krüger

	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in all
	copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
	SOFTWARE.
**************************************************************************************************/

#include <LiquidCrystal.h>
#include <slight_LiquidCrystalDummy.h>

// ------------------------------------------
// Display

// initialize the library with the numbers of the interface pins
// LiquidCrystal lcd(7, 8, 9, 10, 11, 12);
LiquidCrystal lcd_raw(7, 8, 9, 10, 11, 12);

// ------------------------------------------
// slight_LiquidCrystalDummy things

LiquidCrystalDummy lcd(lcd_raw);


void displayTime() {

	// first row
	uint8_t xPos = 0;
	uint8_t yPos = 0;
	lcd.setCursor(xPos, yPos);
	xPos += lcd.print(F("Hello World :-)"));
	// second row
	yPos = 1;
	xPos = 0;

	lcd.setCursor(xPos, yPos);
	xPos += lcd.print("H");

	lcd.setCursor(xPos, yPos);
	xPos += lcd.print("i");

	lcd.setCursor(xPos, yPos);
	xPos += lcd.print(":");

	lcd.setCursor(xPos, yPos);
	xPos += lcd.print(millis()/1000);

	// lcd.setCursor(16-3, yPos);
	// xPos += lcd.print(";-)");
	// xPos += lcd.print("   ");
}


void displayUpdate() {
	displayTime();
}


// ------------------------------------------
// setup
// ------------------------------------------
void setup() {
    // ------------------------------------------
    // init serial
    // wait for arduino IDE to release all serial ports after upload.
    delay(2000);
    Serial.begin(115200);
    delay(2000);
    Serial.println();

    // ------------------------------------------
    // print short welcome text
    Serial.println(F("slight_LiquidCrystalDummy__minimal.ino sketch."));
    Serial.println(F("minimal example for library usage."));

    // ------------------------------------------
    // start LCD
	Serial.println(F("setup Display:")); {
		// set up the LCD's number of columns and rows:
		Serial.println(F("  init to 2x16"));
		lcd.begin(16, 2);
		// Print a message to the LCD.
		lcd.print("Test");
	}
	Serial.println(F("  finished."));

    // ------------------------------------------
    Serial.println(F("Loop:"));
}


// ------------------------------------------
// main loop
// ------------------------------------------
void loop() {
    displayUpdate();
    delay(1000);
    // nothing else to do here...
}


// ------------------------------------------
// THE END :-)
// ------------------------------------------

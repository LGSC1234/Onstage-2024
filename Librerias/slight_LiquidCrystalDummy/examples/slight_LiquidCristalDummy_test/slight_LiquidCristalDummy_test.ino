/**************************************************************************************************

	LiquidCrystalDummy
		test for LiquidCrystalDummy library
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
			MIT
		~ slight_ButtonInput
			written by stefan krueger (s-light),
				git@s-light.eu, http://s-light.eu, https://github.com/s-light/
			cc by sa, Apache License Version 2.0, MIT

	written by stefan krueger (s-light),
		git@s-light.eu, http://s-light.eu, https://github.com/s-light/


	changelog / history
		05.10.2015 10:10 created
		06.10.2015 20:37 should work :-)


	TO DO:
		~ xx


**************************************************************************************************/
/**************************************************************************************************
	The MIT License (MIT)

	Copyright (c) 2015 Vincent Maurer & Stefan Krüger

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

/**************************************************************************************************/
/** Includes:  (must be at the beginning of the file.)                                           **/
/**************************************************************************************************/
// use "" for files in same directory as .ino
//#include "file.h"



#include <LiquidCrystal.h>

#include <slight_LiquidCrystalDummy.h>
// #include "slight_LiquidCrystalDummy.h"

#include <slight_ButtonInput.h>

/**************************************************************************************************/
/** info                                                                                         **/
/**************************************************************************************************/
void print_info(Print &pOut) {
	pOut.println();
	//             "|~~~~~~~~~|~~~~~~~~~|~~~..~~~|~~~~~~~~~|~~~~~~~~~|"
	pOut.println(F("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"));
	pOut.println(F("|                       ^ ^                      |"));
	pOut.println(F("|                      (0,0)                     |"));
	pOut.println(F("|                      ( _ )                     |"));
	pOut.println(F("|                       \" \"                      |"));
	pOut.println(F("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"));
	pOut.println(F("| test_LiquidCrystalDummy.ino"));
	pOut.println(F("|   test the LiquidCrystalDummy library"));
	pOut.println(F("|"));
	pOut.println(F("| This Sketch has a debug-menu:"));
	pOut.println(F("| send '?'+Return for help"));
	pOut.println(F("|"));
	pOut.println(F("| dream on & have fun :-)"));
	pOut.println(F("|"));
	pOut.println(F("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"));
	pOut.println(F("|"));
	//pOut.println(F("| Version: Nov 11 2013  20:35:04"));
	pOut.print(F("| version: "));
	pOut.print(F(__DATE__));
	pOut.print(F("  "));
	pOut.print(F(__TIME__));
	pOut.println();
	pOut.println(F("|"));
	pOut.println(F("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"));
	pOut.println();

	//pOut.println(__DATE__); Nov 11 2013
	//pOut.println(__TIME__); 20:35:04
}


/** Serial.print to Flash: Notepad++ Replace RegEx
	Find what:		Serial.print(.*)\("(.*)"\);
	Replace with:	Serial.print\1\(F\("\2"\)\);
**/


/**************************************************************************************************/
/** definitions (gloabl)                                                                         **/
/**************************************************************************************************/


/************************************************/
/**  DebugOut                                  **/
/************************************************/

boolean bLEDState = 0;
const byte cbID_LED_Info = 9; //D9

unsigned long ulDebugOut_LiveSign_TimeStamp_LastAction	= 0;
const uint16_t cwDebugOut_LiveSign_UpdateInterval		= 1000; //ms

boolean bDebugOut_LiveSign_Serial_Enabled	= 0;
boolean bDebugOut_LiveSign_LED_Enabled		= 1;
boolean bDebugOut_PrintContent_Enabled		= 1;


/************************************************/
/** Menu Input                                 **/
/************************************************/

// a string to hold new data
char  sMenu_Input_New[]				= "x:TestValue";
// flag if string is complete
bool bMenu_Input_Flag_BF		= false; // BufferFull
bool bMenu_Input_Flag_EOL		= false;
bool bMenu_Input_Flag_CR		= false;
bool bMenu_Input_Flag_LF		= false;
bool bMenu_Input_Flag_LongLine	= false;
bool bMenu_Input_Flag_SkipRest	= false;

// string for Currently to process Command
char  sMenu_Command_Current[]		= "x:TestValue ";


/************************************************/
/** Display                                    **/
/************************************************/

// initialize the library with the numbers of the interface pins
// LiquidCrystal lcd(7, 8, 9, 10, 11, 12);
LiquidCrystal lcd_raw(7, 8, 9, 10, 11, 12);

/**********************************************/
/**  Output system                           **/
/**********************************************/

LiquidCrystalDummy lcd(lcd_raw);


/**************************************************/
/**  slight ButtonInput                          **/
/**************************************************/

slight_ButtonInput myButton(
	42, // uint8_t cbID_New
	A2, // uint8_t cbPin_New,
	myButton_getInput, // tCbfuncGetInput cbfuncGetInput_New,
	myButton_onEvent, // tcbfOnEvent cbfCallbackOnEvent_New,
	  30, // const uint16_t cwDuration_Debounce_New = 30,
	2000, // const uint16_t cwDuration_HoldingDown_New = 1000,
	  50, // const uint16_t cwDuration_ClickSingle_New =   50,
	3000, // const uint16_t cwDuration_ClickLong_New =   3000,
	 100  // const uint16_t cwDuration_ClickDouble_New = 1000
);



/************************************************/
/** other things...                            **/
/************************************************/


/**************************************************************************************************/
/** functions                                                                                    **/
/**************************************************************************************************/

/************************************************/
/**  Debug things                              **/
/************************************************/

// http://forum.arduino.cc/index.php?topic=183790.msg1362282#msg1362282
int freeRam () {
  extern int __heap_start, *__brkval;
  int v;
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}


void printBinary8(byte bIn)  {

	for (unsigned int mask = 0b10000000; mask; mask >>= 1) {
		// check if this bit is set
		if (mask & bIn) {
			Serial.print('1');
		}
		else {
			Serial.print('0');
		}
	}
}

void printBinary12(uint16_t bIn)  {
	//                       B12345678   B12345678
	//for (unsigned int mask = 0x8000; mask; mask >>= 1) {
	for (unsigned int mask = 0b100000000000; mask; mask >>= 1) {
		// check if this bit is set
		if (mask & bIn) {
			Serial.print('1');
		}
		else {
			Serial.print('0');
		}
	}
}

void printBinary16(uint16_t wIn)  {
	for (unsigned int mask = 0b1000000000000000; mask; mask >>= 1) {
	// check if this bit is set
		if (mask & wIn) {
			Serial.print('1');
		}
		else {
			Serial.print('0');
		}
	}
}



/************************************************/
/**  Menu System                               **/
/************************************************/

// Modes for Menu Switcher
const uint8_t cbMenuMode_MainMenu	= 1;
const uint8_t cbMenuMode_SubMenu1	= 2;
uint8_t bMenuMode = cbMenuMode_MainMenu;


// SubMenu SetValues
void handleMenu_Sub1(Print &pOut, char *caCommand) {
	pOut.println(F("SubMenu1:"));
	pOut.println(F("\t nothing here."));
	pOut.println(F("\t finished."));
	// exit submenu
	// reset state manschine of submenu
	// jump to main
	bMenuMode = cbMenuMode_MainMenu;
	bMenu_Input_Flag_EOL = true;
}


// Main Menu
void handleMenu_Main(Print &pOut, char *caCommand) {
	/* pOut.print("sCommand: '");
	pOut.print(sCommand);
	pOut.println("'"); */
	switch (caCommand[0]) {
		case 'h':
		case 'H':
		case '?': {
			// help
			pOut.println(F("____________________________________________________________"));
			pOut.println();
			pOut.println(F("Help for Commands:"));
			pOut.println();
			pOut.println(F("\t '?': this help"));
			pOut.println(F("\t 'i': sketch info"));
			pOut.println(F("\t 'y': toggle DebugOut livesign print"));
			pOut.println(F("\t 'Y': toggle DebugOut livesign LED"));
			pOut.println(F("\t 'x': tests"));
			pOut.println();
			pOut.println(F("\t 'A': Show 'HelloWorld' "));
			pOut.println(F("\t 'F': toggle fixture 'F0'..'F2'"));
			pOut.println();
			pOut.println(F("\t 'set:' enter SubMenu1"));
			pOut.println();
			pOut.println(F("____________________________________________________________"));
		} break;
		case 'i': {
			print_info(pOut);
		} break;
		case 'y': {
			pOut.println(F("\t toggle DebugOut livesign Serial:"));
			bDebugOut_LiveSign_Serial_Enabled = !bDebugOut_LiveSign_Serial_Enabled;
			pOut.print(F("\t bDebugOut_LiveSign_Serial_Enabled:"));
			pOut.println(bDebugOut_LiveSign_Serial_Enabled);
		} break;
		case 'Y': {
			pOut.println(F("\t toggle DebugOut livesign LED:"));
			bDebugOut_LiveSign_LED_Enabled = !bDebugOut_LiveSign_LED_Enabled;
			pOut.print(F("\t bDebugOut_LiveSign_LED_Enabled:"));
			pOut.println(bDebugOut_LiveSign_LED_Enabled);
		} break;
		case 'x': {
			// get state
			pOut.println(F("__________"));
			pOut.println(F("Tests:"));

			pOut.println(F("nothing to do."));

			// // uint16_t wTest = 65535;
			// uint16_t wTest = atoi(&caCommand[1]);
			// pOut.print(F("wTest: "));
			// pOut.print(wTest);
			// pOut.println();
			//
			// pOut.print(F("1: "));
			// pOut.print((byte)wTest);
			// pOut.println();
			//
			// pOut.print(F("2: "));
			// pOut.print((byte)(wTest>>8));
			// pOut.println();

			lcd.printContent(pOut);

			pOut.println();

			pOut.println(F("__________"));
		} break;
		//--------------------------------------------------------------------------------
		case 'A': {
			pOut.println(F("\t Hello World! :-)"));
		} break;
		case 'F': {
			pOut.print(F("\t toggle fixture "));

			uint8_t bID = atoi(&caCommand[1]);

			pOut.print(bID);
			// pOut.print(F(" : "));
			// uint16_t wValue = atoi(&caCommand[3]);
			// pOut.print(wValue);
			pOut.println();

			// pOut.print(F("\t demo for parsing values --> finished."));
		} break;
		//--------------------------------------------------------------------------------
		case 's': {
			// SubMenu1
			if ( (caCommand[1] == 'e') && (caCommand[2] == 't') && (caCommand[3] == ':') ) {
				//if full command is 'set:' enter submenu
				bMenuMode = cbMenuMode_SubMenu1;
				if(1){	//if ( caCommand[4] != '\0' ) {
					//full length command
					//handle_SetValues(pOut, &caCommand[4]);
				} else {
					bMenu_Input_Flag_EOL = true;
				}
			}
		} break;
		//--------------------------------------------------------------------------------
		default: {
			pOut.print(F("command '"));
			pOut.print(caCommand);
			pOut.println(F("' not recognized. try again."));
			sMenu_Input_New[0] = '?';
			bMenu_Input_Flag_EOL = true;
		}
	} //end switch

	//end Command Parser
}


// Menu Switcher
void menuSwitcher(Print &pOut, char *caCommand) {
	switch (bMenuMode) {
			case cbMenuMode_MainMenu: {
				handleMenu_Main(pOut, caCommand);
			} break;
			case cbMenuMode_SubMenu1: {
				handleMenu_Sub1(pOut, caCommand);
			} break;
			default: {
				// something went wronge - so reset and show MainMenu
				bMenuMode = cbMenuMode_MainMenu;
			}
		} // end switch bMenuMode
}

// Check for NewLineComplete and enter menuSwitcher
// sets Menu Output channel (pOut)
void check_NewLineComplete() {
	// complete line found:
		if (bMenu_Input_Flag_EOL) {
			// Serial.println(F("check_NewLineComplete"));
			// Serial.println(F("  bMenu_Input_Flag_EOL is set. "));
			// Serial.print  (F("    sMenu_Input_New: '"));
			// Serial.print(sMenu_Input_New);
			// Serial.println(F("'"));

			// Serial.println(F("  Flags:"));
			// Serial.print  (F("    bMenu_Input_Flag_BF: '"));
			// Serial.println(bMenu_Input_Flag_BF);
			// Serial.print  (F("    bMenu_Input_Flag_CR: '"));
			// Serial.println(bMenu_Input_Flag_CR);
			// Serial.print  (F("    bMenu_Input_Flag_LF: '"));
			// Serial.println(bMenu_Input_Flag_LF);
			// Serial.print  (F("    bMenu_Input_Flag_EOL: '"));
			// Serial.println(bMenu_Input_Flag_EOL);


			// Serial.println(F("  copy sMenu_Input_New to sMenu_Command_Current."));
			// copy to current buffer
			strcpy(sMenu_Command_Current, sMenu_Input_New);

			// Serial.println(F("  clear sMenu_Input_New"));
			// reset memory
			memset(sMenu_Input_New,'\0',sizeof(sMenu_Input_New)-1);

			// Serial.println(F("  clear bMenu_Input_Flag_EOL"));
			// reset flag
			bMenu_Input_Flag_EOL = false;
			bMenu_Input_Flag_LF = false;

			// print info if things were skipped.
			if (bMenu_Input_Flag_BF) {
				Serial.println(F("input was to long. used first part - skipped rest."));
				bMenu_Input_Flag_BF = false;
			}

			// parse line / run command
			menuSwitcher(Serial, sMenu_Command_Current);


			// Serial.print  (F("    sMenu_Input_New: '"));
			// Serial.print(sMenu_Input_New);
			// Serial.println(F("'"));
			// Serial.print  (F("    sMenu_Command_Current: '"));
			// Serial.print(sMenu_Command_Current);
			// Serial.println(F("'"));


			// // Serial.println(F("  check bMenu_Input_Flag_SkipRest"));
			// if ( !bMenu_Input_Flag_SkipRest) {
				// // Serial.println(F("   parse Line:"));

				// if (bMenu_Input_Flag_BF) {
					// Serial.println(F("input was to long. used first part - skipped rest."));
					// bMenu_Input_Flag_BF = false;
				// }

				// // parse line / run command
				// menuSwitcher(Serial, sMenu_Command_Current);

				// if(bMenu_Input_Flag_LongLine) {
					// bMenu_Input_Flag_SkipRest = true;
					// bMenu_Input_Flag_LongLine = false;
				// }
			// } else {
				// // Serial.println(F("   skip rest of Line"));
				// bMenu_Input_Flag_SkipRest = false;
			// }

		}// if Flag complete
}

/************************************************/
/**  Serial Receive Handling                   **/
/************************************************/

void handle_SerialReceive() {
	// collect next input text
	while ((!bMenu_Input_Flag_EOL) && (Serial.available())) {
	// while (Serial.available()) {
		// get the new byte:
		char charNew = (char)Serial.read();
		/*Serial.print(F("charNew '"));
		Serial.print(charNew);
		Serial.print(F("' : "));
		Serial.println(charNew, DEC);*/

		// collect next full line
		/* http://forums.codeguru.com/showthread.php?253826-C-String-What-is-the-difference-between-n-and-r-n
			'\n' == 10 == LineFeed == LF
			'\r' == 13 == Carriage Return == CR
			Windows: '\r\n'
			Linux: '\n'
			Apple: '\r'
		*/
		// check for line end
		switch (charNew) {
			case '\r': {
				// Serial.println(F("incoming char is \\r: set EOL"));
				bMenu_Input_Flag_EOL = true;
				bMenu_Input_Flag_CR = true;
				// bMenu_Input_Flag_LF = false;
			} break;
			case '\n': {
				// Serial.println(F("incoming char is \\n: set EOL"));
				// Serial.println(F("  Flags:"));
					// Serial.print  (F("    bMenu_Input_Flag_BF: '"));
					// Serial.println(bMenu_Input_Flag_BF);
					// Serial.print  (F("    bMenu_Input_Flag_CR: '"));
					// Serial.println(bMenu_Input_Flag_CR);
					// Serial.print  (F("    bMenu_Input_Flag_LF: '"));
					// Serial.println(bMenu_Input_Flag_LF);
					// Serial.print  (F("    bMenu_Input_Flag_EOL: '"));
					// Serial.println(bMenu_Input_Flag_EOL);



				bMenu_Input_Flag_LF = true;

				// Serial.println(F("  check for CR"));
				// check if last char was not CR - if this is true set EOL - else ignore.
				if(!bMenu_Input_Flag_CR) {
					bMenu_Input_Flag_EOL = true;
				} else {
					bMenu_Input_Flag_CR = false;
				}

				// Serial.println(F("  Flags:"));
					// Serial.print  (F("    bMenu_Input_Flag_BF: '"));
					// Serial.println(bMenu_Input_Flag_BF);
					// Serial.print  (F("    bMenu_Input_Flag_CR: '"));
					// Serial.println(bMenu_Input_Flag_CR);
					// Serial.print  (F("    bMenu_Input_Flag_LF: '"));
					// Serial.println(bMenu_Input_Flag_LF);
					// Serial.print  (F("    bMenu_Input_Flag_EOL: '"));
					// Serial.println(bMenu_Input_Flag_EOL);


				// this check also works for windows double line ending
				//if (strlen(sMenu_Input_New) > 0) {
					// bMenu_Input_Flag_EOL = true;
				// }
			} break;
			default: {
				// normal char -
				// add it to the sMenu_Input_New:
				//check for length
				if (strlen(sMenu_Input_New) < (sizeof(sMenu_Input_New)-1) ) {
					sMenu_Input_New[strlen(sMenu_Input_New)] = charNew;
				} else {
					//Serial.println(F(" line to long! ignoring rest of line"));
					// set complete flag so line will be parsed
					// Serial.println(F("Buffer is full: set EOL; set LongLine"));
					//bMenu_Input_Flag_EOL = true;
					bMenu_Input_Flag_BF = true;
					// skip rest of line
					bMenu_Input_Flag_LongLine = true;
				}
			}// default
		}// switch charNew

		//check_NewLineComplete();
	}
}


/************************************************/
/** input handler                              **/
/************************************************/


/************************************************/
/** Display                                    **/
/************************************************/

void printByteAlignRight(Print &pOut, uint8_t bValue) {
	//uint8_t bOffset = 0;
	if (bValue < 100) {
		if (bValue < 10) {
			//bOffset = 2;
			pOut.print(F("  "));
		} else {
			//bOffset = 1;
			pOut.print(F(" "));
		}
	}
	pOut.print(bValue);
}

void printByteAsPercentValueAlignRight(Print &pOut, uint8_t bValue) {
	uint8_t valueP = map(bValue, 0, 255, 0, 100);
	if (valueP == 100) {
		// 100 = FF
		pOut.print(F("FF"));
	} else {
		if (valueP < 10) {
			pOut.print(F(" "));
		}
		pOut.print(valueP);
	}
}


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

/************************************************/
/**  slight_ButtonInput things                 **/
/************************************************/

boolean myButton_getInput(uint8_t bID, uint8_t bPin) {
	// read input invert reading - button closes to GND.
	// check HWB
	// return ! (PINE & B00000100);
	return ! digitalRead(bPin);
}


void myButton_onEvent(slight_ButtonInput *pInstance, uint8_t bEvent) {

	// Serial.print(F("Instance ID:"));
	// Serial.println((*pInstance).getID());

	// Serial.print(F("Event: "));
	// (*pInstance).printEvent(Serial, bEvent);
	// Serial.println();

	// show event additional infos:
	switch (bEvent) {
		/*case slight_ButtonInput::event_StateChanged : {
			Serial.print(F("\t state: "));
			(*pInstance).printState(Serial);
			Serial.println();
		} break;*/
		// click
		case slight_ButtonInput::event_Down : {
			Serial.println(F("down"));
			lcd.setCursor(16-3, 1);
			lcd.print(";-)");
		} break;
		/*case slight_ButtonInput::event_HoldingDown : {
			Serial.print(F("duration active: "));
			Serial.println((*pInstance).getDurationActive());
		} break;*/
		case slight_ButtonInput::event_Up : {
			Serial.println(F("up"));
			lcd.setCursor(16-3, 1);
			lcd.print("   ");
		} break;
		// case slight_ButtonInput::event_Click : {
		// 	Serial.println(F("click"));
		// } break;
		// case slight_ButtonInput::event_ClickLong : {
		// 	Serial.println(F("click long"));
		// } break;
		/*case slight_ButtonInput::event_ClickDouble : {
			Serial.println(F("click double"));
		} break;*/
		/*case slight_ButtonInput::event_ClickTriple : {
			Serial.println(F("click triple"));
		} break;*/
		/*case slight_ButtonInput::event_ClickMulti : {
			Serial.print(F("click count: "));
			Serial.println((*pInstance).getClickCount());
		} break;*/
	} //end switch
}


/**************************************************/
/**                                              **/
/**************************************************/






/****************************************************************************************************/
/** Setup                                                                                          **/
/****************************************************************************************************/
void setup() {

	/************************************************/
	/** Initialise PINs                            **/
	/************************************************/

		//LiveSign
		pinMode(cbID_LED_Info, OUTPUT);
		digitalWrite(cbID_LED_Info, HIGH);

		// as of arduino 1.0.1 you can use INPUT_PULLUP

	/************************************************/
	/** init serial                                **/
	/************************************************/

		// for ATmega32U4 devices:
		#if defined (__AVR_ATmega32U4__)
			//wait for arduino IDE to release all serial ports after upload.
			delay(2000);
		#endif

		Serial.begin(115200);

		// for ATmega32U4 devices:
		#if defined (__AVR_ATmega32U4__)
			// Wait for Serial Connection to be Opend from Host or 6second timeout
			unsigned long ulTimeStamp_Start = millis();
			while( (! Serial) && ( (millis() - ulTimeStamp_Start) < 6000 ) ) {
				1;
			}
		#endif

		Serial.println();

		Serial.print(F("# Free RAM = "));
		Serial.println(freeRam());

	/************************************************/
	/** Welcom                                     **/
	/************************************************/

		print_info(Serial);

	/************************************************/
	/** setup Display                              **/
	/************************************************/

		Serial.print(F("# Free RAM = "));
		Serial.println(freeRam());

		Serial.println(F("setup Display:")); {

			// set up the LCD's number of columns and rows:
			Serial.println(F("\t init to 2x16"));
			lcd.begin(16, 2);
			// Print a message to the LCD.
			lcd.print("M4gicDMX");
		}
		Serial.println(F("\t finished."));

	/************************************************/
	/** start slight_ButtonInput                   **/
	/************************************************/
		Serial.print(F("# Free RAM = "));
		Serial.println(freeRam());

		Serial.println(F("slight_ButtonInput:"));
		{
			Serial.println(F("\t pinMode INPUT_PULLUP"));
			// pinMode(myButton.getPin(), INPUT_PULLUP);
			pinMode(myButton.getPin(), INPUT);
			digitalWrite(myButton.getPin(), HIGH);

			Serial.println(F("\t myButton.begin();"));
			myButton.begin();

		}
		Serial.println(F("\t finished."));

	/************************************************/
	/** show Serial Commands                       **/
	/************************************************/

		// reset Serial Debug Input
		memset(sMenu_Input_New, '\0', sizeof(sMenu_Input_New)-1);
		//print Serial Options
		sMenu_Input_New[0] = '?';
		bMenu_Input_Flag_EOL = true;


	/************************************************/
	/** GO                                         **/
	/************************************************/

		Serial.println(F("Loop:"));



} /** setup **/


/****************************************************************************************************/
/** Main Loop                                                                                      **/
/****************************************************************************************************/
void loop() {

	/**************************************************/
	/** Menu Input                                   **/
	/**************************************************/
		// Serial
		handle_SerialReceive();
		check_NewLineComplete();

		// Ohter Input Things:
		//handle_EthTelnet_Server();
		//check_NewLineComplete();


	/**************************************************/
	/** handle input                                 **/
	/**************************************************/


	/**************************************************/
	/** my Button                                    **/
	/**************************************************/
		myButton.update();

	/**************************************************/
	/** update dispay                                **/
	/**************************************************/
		displayUpdate();

	/**************************************************/
	/** Timed things                                 **/
	/**************************************************/


		// every XXXXms
		// if ( ( millis() - ulTimeStamp_LastAction ) > cwUpdateInterval) {
		// 	ulTimeStamp_LastAction =  millis();
		// 	//
		// }



	/**************************************************/
	/** Debug Out                                    **/
	/**************************************************/

		if ( (millis() - ulDebugOut_LiveSign_TimeStamp_LastAction) > cwDebugOut_LiveSign_UpdateInterval) {
			ulDebugOut_LiveSign_TimeStamp_LastAction = millis();

			if ( bDebugOut_LiveSign_Serial_Enabled ) {
				Serial.print(millis());
				Serial.print(F("ms;"));
				Serial.print(F("  free RAM = "));
				Serial.println(freeRam());
			}

			if ( bDebugOut_PrintContent_Enabled ) {
				lcd.printContent(Serial);
			}

			if ( bDebugOut_LiveSign_LED_Enabled ) {
				bLEDState = ! bLEDState;
				if (bLEDState) {
					//set LED to HIGH
					digitalWrite(cbID_LED_Info, HIGH);
				} else {
					//set LED to LOW
					digitalWrite(cbID_LED_Info, LOW);
				}
			}

		}

	/**************************************************/
	/**                                              **/
	/**************************************************/

} /** loop **/


/****************************************************************************************************/
/** THE END                                                                                        **/
/****************************************************************************************************/

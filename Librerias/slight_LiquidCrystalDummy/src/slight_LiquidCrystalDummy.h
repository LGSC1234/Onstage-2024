// NOLINT(legal/copyright)
/******************************************
    slight_LiquidCrystalDummy Library
        for more information see slight_Button.cpp
    written by stefan krueger (s-light),
        git@s-light.eu, http://s-light.eu, https://github.com/s-light/
******************************************/

/******************************************
license

------------------------------------------
Apache License Version 2.0
Copyright 2020 Stefan Krueger

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

------------------------------------------
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
https://opensource.org/licenses/mit-license.php
******************************************/

// include guard
#ifndef slight_LiquidCristalDummy_h
#define slight_LiquidCristalDummy_h

// Includes Core Arduino functionality
#if ARDUINO < 100
#include <WProgram.h>
#else
#include <Arduino.h>
#endif

// include parrent class
#include "LiquidCrystal.h"

// forward declared dependencies
// http://www.cplusplus.com/forum/articles/10627/
// class LiquidCrystal;



class LiquidCrystalDummy : public Print {
	public:
		LiquidCrystalDummy(LiquidCrystal &lcd);

		void begin(uint8_t cols, uint8_t rows);
		void setCursor(uint8_t col, uint8_t row);
		size_t write( uint8_t u_Data );
		void createChar(uint8_t, uint8_t[]);

		void printContent(Print &pOut);

	protected:
		uint8_t size_rows;
		uint8_t size_cols;
		uint8_t pos_row;
		uint8_t pos_col;
		char *displaydata;
		uint8_t displaydata_length;

		boolean ready;

		LiquidCrystal &lcd;

};

// end include guard
#endif

// NOLINT(legal/copyright)
/******************************************
    slight_LiquidCrystalDummy
        simple library with partly identical api as LiquidCristal
        but for Serial-Debug output

    written by stefan krueger (s-light),
        git@s-light.eu, http://s-light.eu, https://github.com/s-light/
*******************************************/

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

Copyright 2020 Stefan Krüger

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

// Include yourselfs header file
// NOLINTNEXTLINE(build/include)
#include "./slight_LiquidCrystalDummy.h"
// use "" for files in same directory


// Includes Core Arduino functionality
#if ARDUINO < 100
#include <WProgram.h>
#else
#include <Arduino.h>
#endif


LiquidCrystalDummy :: LiquidCrystalDummy(
    LiquidCrystal &p_lcd
) :
    lcd(p_lcd)
{
    ready = false;
}

/**********************************************************************/
/** functions                                                        **/
/**********************************************************************/

/************************************************/
/**  public                                    **/
/************************************************/

void LiquidCrystalDummy::begin(uint8_t cols, uint8_t rows) {
    if (ready == false) {
        // call lcd begin
        lcd.begin(cols, rows);

    	size_cols = cols;
    	size_rows = rows;
        displaydata_length = sizeof(char) * size_rows * size_cols;
    	displaydata = (char *)malloc(displaydata_length);
        memset(displaydata, ' ', displaydata_length);
        ready = true;
    }
}

void LiquidCrystalDummy::setCursor(uint8_t col, uint8_t row) {
    lcd.setCursor(col, row);
	pos_col = col;
	pos_row = row;
}

size_t LiquidCrystalDummy::write( uint8_t u_Data ) {
    if (ready) {
        lcd.write(u_Data);
        displaydata[pos_col + (pos_row * size_cols)] = u_Data;
        // move cursor to next col
        pos_col = pos_col +1;
        if (pos_col >= size_cols) {
            pos_col = size_cols-1;
        }
	    return 0x01;
    } else {
        return 0x00;
    }
}

void LiquidCrystalDummy::createChar(uint8_t location, uint8_t charmap[]) {
    if (ready) {
        lcd.createChar(location, charmap);
    }
}

void LiquidCrystalDummy::printContent(Print &pOut) {
    if (ready) {
        // print top border
        pOut.print("#");
        for (size_t indexCol = 0; indexCol < size_cols+2; indexCol++) {
            if (indexCol == pos_col) {
                pOut.print("*");
            } else {
                pOut.print("-");
            }
        }
        pOut.println("#");

        // spacer
        pOut.print("|");
        for (size_t indexCol = 0; indexCol < size_cols+2; indexCol++) {
            pOut.print(" ");
        }
        pOut.println("|");

        char *p_displaydataStart = displaydata;
        for (size_t indexRow = 0; indexRow < size_rows; indexRow++) {
            // print line pre
            if (indexRow == pos_row) {
                pOut.print("* ");
            } else {
                pOut.print("| ");
            }

            pOut.write(p_displaydataStart, size_cols);
            p_displaydataStart = p_displaydataStart + size_cols;

            pOut.println(" |");
        }

        // spacer
        pOut.print("|");
        for (size_t indexCol = 0; indexCol < size_cols+2; indexCol++) {
            pOut.print(" ");
        }
        pOut.println("|");

        // print bottom border
        pOut.print("#");
        for (size_t indexCol = 0; indexCol < size_cols+2; indexCol++) {
            pOut.print("-");
        }
        pOut.println("#");
    }
}

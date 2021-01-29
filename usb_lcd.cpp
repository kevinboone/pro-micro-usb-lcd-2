/**

A program for the SparkFun Pro Micro, or similar, to display character
data sent via a USB connection to an LCD module with an I2C interface.
This version has some limited terminal capabilities, that might be useful 
on larger LCD displays. 

See README.md for more information.

Copyright (c)2021 Kevin Boone, January 2021
Distributed according to the terms of the GPL, v3.0

*/

#include <Arduino.h>
#include <HardwareSerial.h>
#include "lcd8574arduino.h" 
#include "lcdterm.h" 

#define I2C_ADDR 0x27
#define LCD_ROWS 4
#define LCD_COLS 20

#define BANNER "usb-lcd\r\n(c)2021 K Boone"

// Create LCD panel instance, specifying size
LCD8574Arduino lcd (I2C_ADDR, LCD_COLS, LCD_ROWS);
LCDTerm term (lcd, LCDTERM_LF_IS_CRLF);

// Clear banner will be set after the initial banner is cleared,
// after receiving the first character from USB
bool cleared_banner = false;

/** 
 * setup
 * Initialize the USB port and the LCD panel
 */
void setup()
  {
  Serial.begin (57600); 

  term.init();
  term.backlight_on();
  term.cursor_on();
  term.print ((Char *)BANNER);
  }


/** 
 * loop 
 * Repeat every time a character is received 
 */
void loop()
  {
  Serial.flush();

  // Wait until a character has been received
  while (!Serial.available());

  // Clear the banner if necessary
  if (!cleared_banner)
    {
    term.clear();
    cleared_banner = true;
    }

  // Read and display the character
  uint8_t c = Serial.read();
  term.print (c);
  }


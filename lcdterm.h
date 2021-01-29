/*============================================================================

  lcdterm.h

  LCDTerm is a class that wraps a character matrix, and provides some
  of the features of a proper terminal, like scrolling, maintaining
  cursor position, etc. The constructor takes a reference to an instance
  of some class that implement the CharacterMatrix interface. It is
  this instance that does the actual hardware manipulation. This class,
  LCDTerm, knows nothing about the hardware.

  General usage with the LCD8574Arduino class, which is an implementation
  of the CharacterMatrix interface, is like this:

  LCD8574Arduino lcd (0x27, 20, 4);
  LCDTerm term (lcd, LCDTERM_LF_IS_CRLF);

  void setup()
    {
    term.init();
    term.backlight_on();
    term.cursor_on();
    term.print ("Hello, World\n");
    ...

  Copyright (c)1990-2020 Kevin Boone. Distributed under the terms of the
  GNU Public Licence, v3.0

  ==========================================================================*/

#pragma once

#include "charactermatrix.h"

// These constants are used as the flags argument to the constructor
// LF will be interpreted as CR/LF
#define LCDTERM_LF_IS_CRLF  0x01
// Backspace will be interpreted as DEL, and vice-versa
#define LCDTERM_SWAP_BS_DEL 0x02

#define LCDTERM_NORMAL      0x00
#define LCDTERM_NO_WRAP     0x01

class LCDTerm
  {
  public:

  LCDTerm (CharacterMatrix &cm, uint8_t flags = 0);
  void init (void);

  /** Print any character. Handle escapes, etc. */
  void print (Char c);
  /** Print any string of characters.  Handle escapes, etc. */
  void print (const Char *s);

  /** Print a character that is known not to be part of an
   *  escape sequence. The is quicker than print_normal_char(), 
   *  but a bad idea unless you know it's not an escape. */
  void print_nonescape_char (Char c);
  
  /** Print a line feed. If the flag LCDTERM_LF_IS_CRLF was set, 
   *  this will also emit a carriage return. */
  void print_line_feed (void);

  /** Print a form feed; that is, clear the screen. */
  void print_form_feed (void);

  /* Print a carriage return; that is, set the cursor to the start
   * of the line. */
  void print_cr (void);

  /** Print a non-destructive backspace, that is, move the cusor
   *  back one position. */
  void print_bs (void); 

  /** Print a delete, aka destructive backspace. Overwrite the 
   *  character behind the cursor. */
  void print_del (void); 

  /** Sound a bell, if the hardware supports it. */
  void print_bell (void); 

  /** Print spaces until the current cursor position is a multiple of
   * the tab_space value. This might cause the cursor to wrap onto the
   * next line, which is fine (I think). */
  void print_tab (void);

  /** Print a printing ASCII characters. Don't handle escapes, control
   *  characters, etc. */
  void print_normal_char (Char c);

  /** Clear the screen, and home the cursor. */
  void clear (void);

  /** Show the cursor. */
  void cursor_on (void);

  /** Hide the cursor. */
  void cursor_off (void);

  /** Turn on the display backlight. */
  void backlight_on (void);

  /** Turn off the display backlight. */
  void backlight_off (void);

  /** Send the cursor to the home position. */
  void home (void);

  /** Scroll up the whole display, keeping the cursor in the same place. */
  void scroll_up (void);

  /** Set the cursor position. Note that row and column numbers start
   *  at  zero. */
  void set_cursor (uint8_t row, uint8_t col);

  protected:

  CharacterMatrix &cm;
  uint8_t current_row; // Current cursor row
  uint8_t current_col; // Current cursor column
  uint8_t rows;        // Number of rows available
  uint8_t cols;        // Number of columns available
  Char *curr_buff;
  int col_stride;      // Total memory occupied by a row
  bool lf_is_crlf;
  bool swap_bs_del;    // Swap backspace and del
  /** Distance between tab stops. It's advisable to make this a divisor
   *  of the display width. */
  uint8_t tab_space;

  void buff_to_display (void);
  void clear_buff (void);
  };


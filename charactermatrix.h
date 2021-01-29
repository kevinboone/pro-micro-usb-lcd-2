/*============================================================================

  charactermatrix.h

  This is an interface that describes the methods that a class 
  must implement, in order to work with the LCDTerm class. The
  design rationale is that a class that controls an LCD matrix like
  the HD44780 will implement this interface, so LCDTerm will be able
  to use the hardware in a terminal-like way.

  This is an interface because all the methods are pure virtual. It
  makes no sense to try to instantiate this class.

  Copyright (c)1990-2020 Kevin Boone. Distributed under the terms of the
  GNU Public Licence, v3.0

  ==========================================================================*/

#pragma once

#include <stdint.h>

typedef uint8_t Char;

class CharacterMatrix 
  {
  public:

  /** init provides provision for hardware initialization. LCDTerm::init()
   * calls this before anything else. */
  virtual void init (void) = 0;

  /** Return the number of rows. An implementation of this interface must
   *  be able to tell LCDTerm how many rows it has, because LCDTerm
   *  itself does not interact with hardware. */
  virtual uint8_t get_rows() = 0;

  /** Return the number of rows. An implementation of this interface must
   *  be able to tell LCDTerm how many rows it has, because LCDTerm
   *  itself does not interact with hardware. */

  virtual uint8_t get_cols() = 0;
  /** Write the specific character at the specified position, and
   *   place the cursor in the next cell to the right. This method must
   *   _not_ wrap if the cursor is off the end of the line. It may,
   *   or may not, display the cursor somewhere. */
  virtual void write_char_at (uint8_t row, uint8_t col, Char c) = 0; 

  /** Show a cursor at the selected point. The implementation need not
   *  keep any record of the cursor position, because LCDTerm will
   *  always call write_char_at with a specific position. */
  virtual void set_cursor (uint8_t row, uint8_t col) = 0;

  /** Clear the display. If there is a cursor, show it in the home
   *  position. */
  virtual void clear (void) = 0;

  /** Turn on the backlight, if there is one. */
  virtual void backlight_on (void) = 0;

  /** Turn off the backlight, if there is one. */
  virtual void backlight_off (void) = 0;

  /** Show the cursor, if there is one. */
  virtual void cursor_on (void) = 0;

  /** Hide the cursor, if there is one. */
  virtual void cursor_off (void) = 0;

  /** Sound a bell, or provide some other kind of alert, if possible. */
  virtual void bell (void) = 0;
  };



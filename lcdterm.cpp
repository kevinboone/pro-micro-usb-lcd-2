/**

Kevin Boone, January 2021

*/

#include <Arduino.h>
#include "lcdterm.h" 
#include "charactermatrix.h" 

LCDTerm::LCDTerm (CharacterMatrix &cm, uint8_t flags) : 
    cm (cm),
    current_row (0),
    current_col (0),
    lf_is_crlf (false),
    swap_bs_del (false),
    tab_space (5)
  {
  rows = cm.get_rows();
  cols = cm.get_cols();
  if (flags && LCDTERM_LF_IS_CRLF)
    lf_is_crlf = true;
  if (flags && LCDTERM_SWAP_BS_DEL)
    swap_bs_del = true;
  }

/**
 * init 
 */
void LCDTerm::init (void)
  {
  col_stride = cols * sizeof (Char);
  curr_buff = (Char *)malloc (rows * col_stride);
  clear_buff();
  cm.init();
  cm.clear();
  home();
  }

/**
 * home 
 */
void LCDTerm::home (void)
  {
  cm.set_cursor (0, 0);
  current_row = 0;
  current_col = 0;
  }

/** set_cursor */ 
void LCDTerm::set_cursor (uint8_t row, uint8_t col)
  {
  if (row < rows && col < cols)
    {
    cm.set_cursor (row, col);
    current_row = row;
    current_col = col;
    }
  }


/**
 * print (char)
 */
void LCDTerm::print (Char c)
  {
  print_nonescape_char (c);
  }

/**
 * print (char)
 */
void LCDTerm::print_nonescape_char (Char c)
  {
  switch (c)
    {
    case 0:; // Do nothing
    case 7:  // Ring bell
      print_bell();
      break;
    case 8:  // Backspace
      if (swap_bs_del)
        print_del();
      else
        print_bs();
      break;
    case 9:  // Tab
      print_tab ();
      break;
    case 10: // Line feed
      print_line_feed ();
      break;
    case 12: // Form feed
      print_form_feed ();
      break;
    case 13: // Carriage return
      print_cr ();
      break;
    case 17: // DC1 
      cm.backlight_off();
      break;
    case 18: // DC2 
      cm.backlight_on();
      break;
    case 19: // DC3 
      cm.cursor_off();
      break;
    case 20: // DC3 
      cm.cursor_on();
      break;
    case 127: // Del
      if (swap_bs_del)
        print_bs();
      else
        print_del();
      break;
    default:
      print_normal_char (c);
    }
  }

/**
 * print (const char *)
 */
void LCDTerm::print (const Char *s)
  {
  while (*s)
    {
    print (*s);
    s++;
    }
  }

/**
 * print_line_feed
 */
void LCDTerm::print_line_feed (void)
  {
  if (lf_is_crlf) print_cr();
  if (current_row < rows - 1)
    current_row++;
  else
    scroll_up ();
  cm.set_cursor (current_row, current_col);
  }

/**
 * print_line_feed
 */
void LCDTerm::print_form_feed (void)
  {
  cm.clear();
  clear_buff();
  current_row = 0;
  current_col = 0;
  }

/**
 * print_line_feed
 */
void LCDTerm::print_cr (void)
  {
  current_col = 0;
  cm.set_cursor (current_row, current_col);
  }

/**
 * print_normal_char
 */
void LCDTerm::print_normal_char (Char c)
  {
  if (current_row < rows)
    {
    curr_buff [current_row * cols + current_col] = c;
    cm.write_char_at (current_row, current_col, c);
    current_col++;
    if (current_col >= cols)
      {
      if (current_row >= rows - 1)
        scroll_up();
      else
        current_row++;
      current_col = 0;
      cm.set_cursor (current_row, current_col);
      }
    }
  else
    {
    // TODO -- overflow, scroll, etc
    }
  }


/**
 * cursor_on
 */
void LCDTerm::cursor_on (void)
  {
  cm.cursor_on();
  }

/**
 * cursor_off
 */
void LCDTerm::cursor_off (void)
  {
  cm.cursor_off();
  }

/**
 * backlight_on
 */
void LCDTerm::backlight_on (void)
  {
  cm.backlight_on();
  }

/**
 * backlight_off
 */
void LCDTerm::backlight_off (void)
  {
  cm.backlight_off();
  }

/**
 * clear 
 */
void LCDTerm::clear (void)
  {
  cm.clear();
  clear_buff();
  home();
  }

/** dump_buff */
void LCDTerm::buff_to_display (void)
  {
  cm.clear();
  for (uint8_t row = 0; row < rows; row++)
    {
    for (uint8_t col = 0; col < cols; col++)
      {
      cm.write_char_at (row, col, curr_buff[row * col_stride + col]);
      }
    }
  }

/**
 * scroll_up
 */
void LCDTerm::scroll_up (void)
  {
  // Shift up the buffer
  memmove (curr_buff, curr_buff + col_stride, (rows - 1) * col_stride);
  // Null the bottom line (nulls will print as spaces)
  memset (curr_buff + (rows - 1) * col_stride, 0, col_stride);
  buff_to_display();
  cm.set_cursor (current_row, current_col);
  }

/**
 * print_tab 
 */
void LCDTerm::clear_buff (void)
  {
  memset (curr_buff, 0, rows * col_stride);
  }

/**
 * print_tab 
 */
void LCDTerm::print_tab (void)
  {
  do
    {
    print (' ');
    } while ((current_col % tab_space) != 0);
  }

/**
 * print_bs
 * TODO: do we need to backspace back onto the previous line??
 */
void LCDTerm::print_bs (void)
  {
  if (current_col > 0)
    {
    current_col--;
    cm.set_cursor (current_row, current_col);
    }
  }

/**
 * print_del
 */
void LCDTerm::print_del (void)
  {
  print_bs();
  print (' ');
  print_bs();
  }

/**
 * print_bell
 */
void LCDTerm::print_bell (void)
  {
  cm.bell();
  }





# Pro Micro USB LCD (v2)

Firmware and circuit design for the SparkFun Pro Micro, for displaying data
sent to the USB port on a small HD44780-style LCD display with an I2C
interface. This provides an "auxilliary display" for showing status
information, etc., from a computer. There are, conceivably, other applications
as well.

Note that this code does not replace my original pro-micro-usb-lcd. For small
(one and two-line) displays, the extra features of this version are likely to
be a waste of flash ROM.

I've been told that this code works with several commercially available S2I
LCD display boards -- I guess they all use the same basic circuit
configuration. You'll need to change the display width and height, and perhaps
the I2C address. However, the only display I know this code works with is the
design in the accompanying circuit diagram (see circuit.png).

There are limited terminal capabilities.  Text that is too long for the line
automatically roles over to the next row, and when the bottom line is reached,
text scrolls up.

The program responds to the following control characters, as well as the
usual ASCII set:

Backspace (8) -- move the cursor back one position, without erasing

Tab (9) -- move to the next tab stop

Line feed (10) -- move to the next row. If set up for it, LF will also
emit a carriage return.

Form feed (12) -- clear the display

Carriage return (13) -- move to start of line

DC1 (17) -- backlight off
DC2 (18) -- backlight on
DC3 (19) -- cursor off
DC4 (20) -- cursor on

Del (127) -- erase character before the cursor

There is no support for ANSI escapes. It wouldn't be hard to add, but a
full implementation would probably exceed the storage capabilities of
a Pro Micro. Instead, the program uses a number of ASCII codes in
non-standard ways. The DCx codes, for example, are used to control
the hardware. 

On Linux you can use these codes like this:

Backlight off
$ printf "\x11" > /dev/ttyACM0 

Backlight on
$ printf "\x12" > /dev/ttyACM0 

Cursor off
$ printf "\x13" > /dev/ttyACM0 

Cursor on
$ printf "\x14" > /dev/ttyACM0 

By small code changes (see how the constructor for LCD term is 
invoked), it's possible to configure LF to be interpreted as CR/LF,
and to swap the roles of backspace and tell. 

The HD44780 supports 8-bit characters, but not in any standard encoding
-- you'll need to look at the datasheet to see the character table for
non-ASCII characters.

See the scripts `clock_sample.sh` and `status_sample.sh` to see how this 
program might be used in Linux. There's nothing Linux-specific about the
firmware, but I'm not sure how to interact with COM ports in Windows at
the script level (if it's even possible).

Note -- if you're seeing "ATxxx" commands being displayed when using this
design with a Linux host, this is probably because the Linux modem manager is
trying to control the USB port as if it were a serial model. This isn't
something that can be fixed in this software (except by parsing and handling AT
commands). Normal USB data sent by an application will clear 
these bogus AT commands from the display.

For full details of the hardware design, see

http://kevinboone.me/pro-micro-usb-lcd.html

Note that I don't do IDEs or, for the most part, Windows. This firmware
is intended to be built using command-line tools on Linux. For more
information on building Arduino programs this way, see:

http://kevinboone.me/pro-micro-blink.html


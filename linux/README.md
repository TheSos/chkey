This is a linux version of chkey.

Usage
=====
USAGE: controller [comPort e.g. /dev/ttyUSB0 ] A-Mapping B-Mapping C-Mapping [verbose]
For mappings use the KeySyms provided in /usr/include/X11/keysymdef.h AS DECIMAL-Values ( https://www.cl.cam.ac.uk/~mgk25/ucs/keysymdef.h )

Here are some symvalues:

XK_0                             48 /* U+0030 DIGIT ZERO */
XK_A                             65  /* U+0041 LATIN CAPITAL LETTER A */
XK_a                             97  /* U+0061 LATIN SMALL LETTER A */
XK_space                         32  /* U+0020 SPACE */

Sample: controller /dev/ttyUSB0 48 49 50
        controller /dev/ttyUSB0 48 49 50 verbose

Warning: This is a very limited proof of concept. Pressing keys simulatenously will only fire the last pressed button.

There is a compiled version in the repositiory (linux/controller)

Building
========

You need libxtst and cmake.  so if those are not present yet, install them:
sudo apt-get install libxtst-dev cmake

* cd linux
* mkdir t
* cd t
* cmake ..
* make

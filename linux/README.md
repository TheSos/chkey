This is a linux version of chkey.

Usage
=====
USAGE:

* controller [comPort e.g. /dev/ttyUSB0 ] A-Mapping B-Mapping C-Mapping [verbose]

For mappings use the KeySyms provided in /usr/include/X11/keysymdef.h AS DECIMAL-Values ( https://www.cl.cam.ac.uk/~mgk25/ucs/keysymdef.h )

Here are some symvalues:

* XK_0                             48 
* XK_A                             65 
* XK_a                             97 
* XK_space                         32 

Sample: 

* controller /dev/ttyUSB0 48 49 50
* controller /dev/ttyUSB0 48 49 50 verbose


Warning: This is a very limited proof of concept. Pressing keys simulatenously will only fire the last pressed button.


There is a compiled version in the repositiory (linux/controller)


Building
========

You need libxtst. if this is not present yet, install it:

* sudo apt-get install libxtst-dev
* cd linux
* mkdir t
* cd t
* cmake ..
* make

Troubleshooting
===============

To use the precompiled version you might need to make it executable:

* chmod a+x controller


If there is an error like "bad descriptor":

1) you might have used the wrong serial-port (/dev/ttyUSB0 or /dev/ttyUSB1) check in the arduino-ide (TOOL>SERIAL_PORT) 


2) you might not have the kernel driver for ch940g installed:

* Download http://www.wch.cn/download/CH341SER_LINUX_ZIP.html
* unzip
* make
* sudo make load
* maybe you need to restart!?!

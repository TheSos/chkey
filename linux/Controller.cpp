//============================================================================
// Name        : Controller.cpp
//============================================================================

#include <sys/time.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <termios.h>

#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <cstdlib>
#include <iostream>


#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/extensions/XTest.h>

using namespace std;

// this snippet reads data from a serialport (arg[1])
// and maps A/D,B/E,C/F to the keycodes arg2,arg3,arg4 (pressed/released) to fake key-inputs

// actually only orchestrated some stackoverflow lookups:
// http://stackoverflow.com/questions/27835191/open-write-and-read-ttyusb0
// http://stackoverflow.com/questions/1262310/simulate-keypress-in-a-linux-c-console-application
//
// libraries needed: Xtst
// sudo apt-get install libxtst-dev

int serial_open(char *serial_name, speed_t baud)
{
      struct termios newtermios;
      int fd;
      fd = open(serial_name,O_RDWR | O_NOCTTY);
      newtermios.c_cflag= CBAUD | CS8 | CLOCAL | CREAD;
      newtermios.c_iflag=IGNPAR;
      newtermios.c_oflag=0;
      newtermios.c_lflag=0;
      newtermios.c_cc[VMIN]=1;
      newtermios.c_cc[VTIME]=0;
      cfsetospeed(&newtermios,baud);
      cfsetispeed(&newtermios,baud);
      if (tcflush(fd,TCIFLUSH)==-1) return -1;
      if (tcflush(fd,TCOFLUSH)==-1) return -1;
      if (tcsetattr(fd,TCSANOW,&newtermios)==-1) return -1;
      return fd;
}
void serial_send(int serial_fd, char *data, int size)
{
  write(serial_fd, data, size);
}

int serial_read(int serial_fd, char *data, int size, int timeout_usec)
{
      fd_set fds;
      struct timeval timeout;
      int count=0;
      int ret;
      int n;
      do {
        FD_ZERO(&fds);
        FD_SET (serial_fd, &fds);
        timeout.tv_sec = 0;
        timeout.tv_usec = timeout_usec;
        ret=select (FD_SETSIZE,&fds, NULL, NULL,&timeout);
        if (ret==1) {
          n=read (serial_fd, &data[count], size-count);
          count+=n;
          data[count]=0;
   }
 } while (count<size && ret==1);
 return count;
}

void serial_close(int fd)
{
   close(fd);
}

int main(int argc, char *argv[])
{
	if (argc<5) {
		printf("USAGE: controller [comPort e.g. /dev/ttyUSB0 ] A-Mapping B-Mapping C-Mapping [verbose]\n\n");
		printf("For mappings use the KeySyms provided in /usr/include/X11/keysymdef.h AS DECIMAL-Values!!\n");
		printf("XK_0                             48 /* U+0030 DIGIT ZERO */ \n");
		printf("XK_A                             65  /* U+0041 LATIN CAPITAL LETTER A */\n");
		printf("XK_a                             97  /* U+0061 LATIN SMALL LETTER A */\n");
		printf("XK_space                         32  /* U+0020 SPACE */\n");
		printf("\nSample: controller /dev/ttyUSB0 48 49 50\n");
		printf("\n        controller /dev/ttyUSB0 48 49 50 verbose\n");
		printf("\n");
		printf("Warning: This is a very limited proof of concept. Pressing key simulatenously will only fire the last pressed button.\n");
		return -1;
	}

   int serial_fd, n, longitud;
//   char *device=”at”;

//   longitud=strlen(device);

   char data[256];

   Display *display;
   unsigned int keycode;
   display = XOpenDisplay(NULL);

   XFlush(display);

   serial_fd = serial_open(argv[1],B9600);
    if (serial_fd == -1) {
            printf ("Error opening the serial device: %s\n",argv[1]);
            perror("OPEN");
            return 0;
    }

    int a;
    int b;
    int c;

    a = atoi(argv[2]);
    b = atoi(argv[3]);
    c = atoi(argv[4]);

    bool verbose = argc>5;

    while(true){
        n=serial_read(serial_fd,data,1,10000);
        if (n>0){
            if (data[0]=='A') {
                keycode = XKeysymToKeycode(display, a);
                XTestFakeKeyEvent(display, keycode, True, 0);

                if (verbose) {
                	printf("A pressed\n");
                }

                XFlush(display);
            }
            else if (data[0]=='D') {
                if (verbose) {
                	printf("A released\n");
                }

               	keycode = XKeysymToKeycode(display, a);
                XTestFakeKeyEvent(display, keycode, False, 0);
                XFlush(display);
            }

            else if (data[0]=='B') {
                keycode = XKeysymToKeycode(display, b);
                XTestFakeKeyEvent(display, keycode, True, 0);
                if (verbose) {
                	printf("B pressed\n");
                }
                XFlush(display);
            }

            else if (data[0]=='E') {
            	if (verbose) {
                	printf("B released\n");
            	}
               	keycode = XKeysymToKeycode(display, b);
                XTestFakeKeyEvent(display, keycode, False, 0);
                XFlush(display);
            }

            else if (data[0]=='C') {
                keycode = XKeysymToKeycode(display, c);
                XTestFakeKeyEvent(display, keycode, True, 0);
                if (verbose) {
                    printf("C pressed\n");
                }
                XFlush(display);
            }
            else if (data[0]=='F') {
            	if (verbose) {
                	printf("C released\n");
            	}
                keycode = XKeysymToKeycode(display, c);
                XTestFakeKeyEvent(display, keycode, False, 0);
                XFlush(display);
            }
            else {
            	printf("UNKNOWN DATA:%c\n",data[0]);
            }

        }
    }
    serial_close(serial_fd);
//    cout << "Hello world!" << endl;
    return 0;
}

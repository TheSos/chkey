#include <stdio.h>
#include <windows.h>


   void SetNumLock( BOOL bState )
   {
      BYTE keyState[256];

      GetKeyboardState((LPBYTE)&keyState);
      if( (bState && !(keyState[VK_NUMLOCK] & 1)) ||
          (!bState && (keyState[VK_NUMLOCK] & 1)) )
      {
      // Simulate a key press
         keybd_event( VK_NUMLOCK,
                      0x45,
                      KEYEVENTF_EXTENDEDKEY | 0,
                      0 );

      // Simulate a key release
         keybd_event( VK_NUMLOCK,
                      0x45,
                      KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP,
                      0);
      }
   }

   int main(int argc, char** argv)
   {
     HANDLE hComm;
     char buf[10],path[MAX_PATH];
     int a,b,c;
     long num;
     int state=0,k=0;
          printf("\nArduino chekeyizer\n");
     printf("(c) Sos Sosowski 2016\n");
     if (argc<4)
     {


         printf("\nConverts serial input from arduino to key strokes and shit.\n\n");
        printf("USAGE: chkey [comport] [vkey1] [vkey2] [vkey3]\n");
        printf(" * comport - COM1 or COM30 or stuff ike that\n");
        printf(" * vkey* - character on keyboard or virtual code for one\n");
        printf("           can be 37 (up key) or a for A key or stuff\n\n");
        printf("VKey table here: https://msdn.microsoft.com/en-us/library/windows/desktop/dd375731(v=vs.85).aspx\n");
        return 0;
     }
     sprintf(path,"\\\\.\\%s",argv[1]);
     a = atoi(argv[2]);
     if (!a) a=toupper(argv[2][0]);
     b = atoi(argv[3]);
     if (!b) b=toupper(argv[3][0]);
     c = atoi(argv[4]);
     if (!c) c=toupper(argv[4][0]);
      //SetNumLock( TRUE );
      printf("Starting chkeyizer with keys %02X, %02X, %02X\n",a,b,c);
       printf("Opening port %s...",argv[1]);

hComm = CreateFile( path,
                    GENERIC_READ | GENERIC_WRITE,
                    0,
                    0,
                    OPEN_EXISTING,
                    0,
                    0);
if (hComm == INVALID_HANDLE_VALUE)
      {

        printf("ERROR: %08x\n",GetLastError());
        return -1;
      }
      printf("DONE!\n");
      while (!GetAsyncKeyState(VK_ESCAPE))
      {
        ReadFile(hComm,buf,1,&num ,NULL );
        if (buf[0]=='A') {k=a;state=0;}
        if (buf[0]=='D') {k=a;state=KEYEVENTF_KEYUP;}

        if (buf[0]=='B') {k=b;state=0;}
        if (buf[0]=='E') {k=b;state=KEYEVENTF_KEYUP;}

        if (buf[0]=='C') {k=c;state=0;}
        if (buf[0]=='F') {k=c;state=KEYEVENTF_KEYUP;}

        keybd_event(k,MapVirtualKey(k,0),state,0);
        printf("Got char %c (%02X), %s button %02X\n",buf[0],buf[0],state?"releasing":"pressing",k);
      }
      CloseHandle(hComm);
      return 0;
   }

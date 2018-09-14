#include <stdio.h>
#include <rt_misc.h>
#pragma import(__use_no_semihosting_swi)

//#if defined( __UART1_DEBUG__) || defined(__UART2_DEBUG__)
extern int  SendChar(int ch); // ??????,?main?????
extern int  GetKey(void);
//#endif

struct __FILE {
  int handle;                 // Add whatever you need here
};
FILE __stdout;
FILE __stdin;
int fputc(int ch, FILE *f) {
	//#if defined( __UART1_DEBUG__) || defined(__UART2_DEBUG__)
  return (SendChar(ch));
	//#else
	//return 0;
	//#endif
}
int fgetc(FILE *f) {
	//#if defined( __UART1_DEBUG__) || defined(__UART2_DEBUG__)
  return (SendChar(GetKey()));
	//#else
	//return 0;
	//#endif
}
void _ttywrch(int ch) {
//#if defined( __UART1_DEBUG__) || defined(__UART2_DEBUG__)
		SendChar (ch);
//#endif
}
int ferror(FILE *f) {                            // Your implementation of ferror
  return EOF;
}
void _sys_exit(int return_code) {
label:  goto label;           // endless loop
}


#ifndef APP_MAIN_H_
#define APP_MAIN_H_

#ifdef FREERTOS
int freertos_main (void);
#else
void arduino_setup();
void arduino_loop();
#endif

#endif  /* APP_MAIN_H_*/
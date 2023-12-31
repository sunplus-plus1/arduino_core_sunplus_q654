#include "WSerial.h"

WEAK void serialEventRun(void)
{
#if defined(HAVE_HWSERIAL1)
  if (serialEvent1 && Serial1.available()) {
    serialEvent1();
  }
#endif
#if defined(HAVE_HWSERIAL2)
  if (serialEvent2 && Serial2.available()) {
    serialEvent2();
  }
#endif
#if defined(HAVE_HWSERIAL3)
  if (serialEvent3 && Serial3.available()) {
    serialEvent3();
  }
#endif
#if defined(HAVE_HWSERIAL4)
  if (serialEvent4 && Serial4.available()) {
    serialEvent4();
  }
#endif
#if defined(HAVE_HWSERIAL6)
  if (serialEvent6 && Serial6.available()) {
    serialEvent6();
  }
#endif
#if defined(HAVE_HWSERIAL7)
  if (serialEvent7 && Serial7.available()) {
    serialEvent7();
  }
#endif
}


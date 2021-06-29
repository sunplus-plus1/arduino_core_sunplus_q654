#include "HardwareSerial.h"

#ifdef __cplusplus
extern "C" {
#endif

void uart_test(void)
{

	HardwareSerial uart;
	uart.begin(115200);

	uart.print("abcdef123456798abdefdefg");
	uart.print("9999");

	printf("get = %c  \n",uart.read());

}


#ifdef __cplusplus
}
#endif



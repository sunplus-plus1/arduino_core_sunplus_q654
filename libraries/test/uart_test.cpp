#include "HardwareSerial.h"

#ifdef __cplusplus
extern "C" {
#endif

void uart_test(void)
{

	HardwareSerial uart1(SP_UART1);
	uart1.begin(115200);

	uart1.print("abcdef123456798abdefdefg");
	uart1.print("9999");

	printf("get = %c  \n",uart1.read());

}


#ifdef __cplusplus
}
#endif



#include "HardwareSerial.h"
#include "Arduino.h"
#include "Arduino.h"
#include "Wire.h"

#ifdef __cplusplus
extern "C" {
#endif

//HardwareSerial uart = HardwareSerial(SP_UART1);

void uart_test(void)
{
	char buf[20]={0};
	int time_cnt=0,i=0;


	HardwareSerial uart(SP_UART1);
	uart.begin(115200);

	/* write */
	uart.write("abcdef123456798abdefdefg", sizeof("abcdef123456798abdefdefg"));
	delay(200);

	/* read */
	time_cnt = millis();
	while(millis()-time_cnt < 2000)
	{
		if(uart.available())
		{
			buf[i++] = uart.read();
		}
		delay(200);
	}

	printf("getchar = %s  \n",buf);

}


void setup()
{
	uart_test();
}

void loop()
{

}

#ifdef __cplusplus
}
#endif



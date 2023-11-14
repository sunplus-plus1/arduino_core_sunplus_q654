#include "Arduino.h"
#include "VirtIOSerial.h"

VirtIOSerial uart2, uart3;

void setup()
{
	SerialVirtIO.begin();
	uart1.begin();
	uart2.begin();
}

void loop()
{
	if (SerialVirtIO.available()) {
		String s = SerialVirtIO.readString();
		printf("VIO:%s \n",s.cstr());
		SerialVirtIO.print(s); // write back
	}
	if (uart1.available()) {
		String s = uart1.readString();
		printf("uart1:%s \n",s.cstr());
		uart1.print(s); // write back
	}
	if (uart2.available()) {
		String s = uart2.readString();
		printf("uart2:%s \n",s.cstr());
		uart2.print(s); // write back
	}
}

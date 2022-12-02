#include "Arduino.h"
#include "VirtIOSerial.h"

VirtIOSerial uart2, uart3;

void setup()
{
	SerialVirtIO.begin();
	uart2.begin();
	uart3.begin();
}

void loop()
{
	if (SerialVirtIO.available()) {
		String s = SerialVirtIO.readString();
		SerialVirtIO.print(s); // write back
	}
	if (uart2.available()) {
		String s = uart2.readString();
		uart2.print(s); // write back
	}
	if (uart3.available()) {
		String s = uart3.readString();
		uart3.print(s); // write back
	}
}

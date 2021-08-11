#include "VirtIOSerial.h"

void setup()
{
	SerialVirtIO.begin();
}

void loop()
{
	if (SerialVirtIO.available()) {
		String s = SerialVirtIO.readString();
		SerialVirtIO.print(s); // write back
	}
}

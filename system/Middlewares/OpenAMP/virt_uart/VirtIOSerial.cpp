#include "VirtIOSerial.h"
#include "virt_uart.h"
#include "emuio.h"

VirtIOSerial::VirtIOSerial()
{
  UART_puts("VirtIOSerial::VirtIOSerial()\n");
  virt_uart_open();
}

VirtIOSerial::~VirtIOSerial()
{
  virt_uart_close();
}

int VirtIOSerial::available()
{
  return virt_uart_available();
}

int VirtIOSerial::availableForWrite()
{
  // Just return max length of virt_uart_write() can transmit.
  return RPMSG_VRING_PAYLOAD_SIZE;
}

int VirtIOSerial::peek()
{
  return virt_uart_peek();
}

int VirtIOSerial::read()
{
  if (available() > 0) {
    char ch;
    readBytes(&ch, 1);
    return ch;
  } else {
    return -1;
  }
}

size_t VirtIOSerial::readBytes(char *buffer, size_t length)
{
  return virt_uart_read(buffer, length);
}

String VirtIOSerial::readString()
{
  String ret;
  int c = read();
  while (c >= 0) {
    ret += (char)c;
    c = read();
  }
  return ret;
}

size_t VirtIOSerial::write(uint8_t ch)
{
  return write(&ch, 1);
}

size_t VirtIOSerial::write(const uint8_t *buffer, size_t length)
{
  return virt_uart_write((void *)buffer, length);
}

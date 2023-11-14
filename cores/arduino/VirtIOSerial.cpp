
#if defined (VIRTIOCON)

#include "VirtIOSerial.h"
#include "virt_uart.h"

VirtIOSerial SerialVirtIO, uart1, uart2;

void VirtIOSerial::begin(void)
{
  uart = virt_uart_open();
}

void VirtIOSerial::begin(uint32_t /* baud_count */)
{
  begin();
}

void VirtIOSerial::begin(uint32_t /* baud_count */, uint8_t /* config */)
{
  begin();
}

void VirtIOSerial::end()
{
  virt_uart_close(uart);
}

int VirtIOSerial::available()
{
  return virt_uart_available(uart);
}

int VirtIOSerial::availableForWrite()
{
  // Just return max length of virt_uart_write() can transmit.
  return RPMSG_VRING_PAYLOAD_SIZE;
}

int VirtIOSerial::peek()
{
  return virt_uart_peek(uart);
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
  return virt_uart_read(uart, buffer, length);
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
  return virt_uart_write(uart, (void *)buffer, length);
}


#endif

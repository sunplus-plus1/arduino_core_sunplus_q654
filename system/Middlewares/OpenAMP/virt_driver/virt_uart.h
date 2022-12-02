#ifndef __VIRT_UART_H
#define __VIRT_UART_H

#include "virtio_config.h"
#include "virtio_buffer.h"

#ifdef __cplusplus
extern "C" {
#endif

void *virt_uart_open(void);
void virt_uart_close(void *uart);
void virt_uart_poll();
int virt_uart_read(void *uart, void *buf, int len);
int virt_uart_write(void *uart, void *buf, int len);
int virt_uart_available(void *uart);
int virt_uart_peek(void *uart);

#ifdef __cplusplus
}
#endif

#endif /* __VIRT_UART_H */

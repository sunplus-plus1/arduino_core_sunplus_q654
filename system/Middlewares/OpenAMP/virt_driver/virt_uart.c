#ifdef VIRTIOCON

#include "platform_info.h"
#include "virt_uart.h"

#define RPMSG_SERVICE_NAME  "rpmsg-tty-raw"

static void *platform = NULL;
static struct rpmsg_device *rpdev = NULL;
static int src_addr = 0;

typedef struct virt_uart_s {
        struct rpmsg_endpoint ept;
        virtio_buffer_t ring;
} virt_uart_t;

static int virt_uart_cb(struct rpmsg_endpoint *ept, void *data, size_t len,
			uint32_t src, void *priv)
{
	virt_uart_t *uart = priv;
	virtio_buffer_write(&uart->ring, data, len);
	return RPMSG_SUCCESS;
}

void *virt_uart_open(void)
{
	int ret;
	virt_uart_t *uart;

	// initial remoteproc platform
	if (!platform) {
		platform_init(0, NULL, &platform);
		if (!platform)
			return NULL;

		// create remoteproc rpmsg dev
		rpdev = platform_create_rpmsg_vdev(platform, 0,
						   VIRTIO_DEV_SLAVE, NULL, NULL);
		if (!rpdev) {
			platform_cleanup(platform);
			platform = NULL;
			return NULL;
		}
	}

	// create virt_uart
	uart = malloc(sizeof(virt_uart_t));
	uart->ept.priv = uart;

	// create rpmsg endpoint
	ret = rpmsg_create_ept(&uart->ept, rpdev, RPMSG_SERVICE_NAME,
				src_addr++, RPMSG_ADDR_ANY, virt_uart_cb, NULL);
	if (ret)
		return NULL;

	// initial virtio ring buffer
	virtio_buffer_init(&uart->ring);

	return uart;
}

void virt_uart_close(void *uart)
{
	rpmsg_destroy_ept(&((virt_uart_t *)uart)->ept);
	free(uart);
}

void virt_uart_poll(void)
{
	platform_poll(platform);
}

int virt_uart_read(void *uart, void *buf, int len)
{
	virt_uart_poll();
	return virtio_buffer_read(&((virt_uart_t *)uart)->ring, buf, len);
}

int virt_uart_write(void *uart, void *buf, int len)
{
	int wrote, left = len;

	while (left) {
		wrote = (left <= RPMSG_VRING_PAYLOAD_SIZE) ? left : RPMSG_VRING_PAYLOAD_SIZE;
		wrote = rpmsg_send(&((virt_uart_t *)uart)->ept, buf, wrote);
		if (wrote < 0) // error
			return wrote;
		buf += wrote;
		left -= wrote;
	}

	return len;
}

int virt_uart_available(void *uart)
{
	virt_uart_poll();
	return virtio_buffer_read_available(&((virt_uart_t *)uart)->ring);
}

int virt_uart_peek(void *uart)
{
	if (virt_uart_available(uart) > 0) {
		uint8_t tmp;
		virtio_buffer_peek(&((virt_uart_t *)uart)->ring, &tmp, 1);
		return tmp;
	} else {
		return -1;
	}
}

#endif

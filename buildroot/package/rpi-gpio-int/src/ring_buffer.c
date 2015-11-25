#include "ring_buffer.h"

// Buffer size must be a power of two
#define BUFFER_SIZE (1024)
#define INDEX_MASK (BUFFER_SIZE - 1)
#define WRITE_ADDR (&ringbuffer.buffer[ringbuffer.write_index++ & INDEX_MASK])
#define READ_ADDR (&ringbuffer.buffer[ringbuffer.read_index++ & INDEX_MASK])
#define BUFFER_USED (ringbuffer.write_index - ringbuffer.read_index)

static struct
{
	volatile struct gpio_event_t buffer[BUFFER_SIZE];
	volatile size_t write_index;
	volatile size_t read_index;
}
ringbuffer;

void ringbuffer_write(struct gpio_event_t *element)
{
	if (BUFFER_USED != BUFFER_SIZE)
		memcpy(
			(struct gpio_event_t *)WRITE_ADDR,
			element,
			sizeof(struct gpio_event_t)
		);
}

void ringbuffer_read(struct gpio_event_t *element)
{
	if (BUFFER_USED != 0)
		memcpy(
			element,
			(struct gpio_event_t *)READ_ADDR,
			sizeof(struct gpio_event_t)
		);
}

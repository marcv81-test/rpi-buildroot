#ifndef RPI_GPIO_INT_RING_BUFFER_H
#define RPI_GPIO_INT_RING_BUFFER_H

#include "core.h"

extern void ringbuffer_write(struct gpio_event_t *event);
extern void ringbuffer_read(struct gpio_event_t *event);

#endif

#ifndef RPI_GPIO_INT_H
#define RPI_GPIO_INT_H

#include <linux/time.h>

#define MODULE_NAME "rpi_gpio_int"

struct gpio_event_t
{
	enum { RISE = 1, FALL = 0 } type;
	struct timespec time;
};

#endif

#ifndef RPI_GPIO_INT_HARDWARE_H
#define RPI_GPIO_INT_HARDWARE_H

#include <linux/interrupt.h>

extern int hw_init(void);
extern int hw_pin_level(void);
extern int hw_request_irq(irq_handler_t irq_handler);
extern void hw_free_irq(void);

#endif

#include "core.h"

#include "hardware.h"
#include "ring_buffer.h"

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("marcv81");
MODULE_DESCRIPTION("Raspberry Pi interrupt-based GPIO data acquisition module");

static void create_event(struct gpio_event_t *event)
{
	getrawmonotonic(&event->time);
	event->type = hw_pin_level() > 0 ? RISE : FALL;
}

static irqreturn_t irq_handler(int i, void *blah, struct pt_regs *regs)
{
	struct gpio_event_t event;
	create_event(&event);
	ringbuffer_write(&event);
	return IRQ_HANDLED;
}

int __init init(void)
{
	printk(KERN_INFO MODULE_NAME ": init\n");

	if (hw_init() < 0)
		return -ENODEV;

	if (hw_request_irq((irq_handler_t)irq_handler) < 0)
		return -ENODEV;

	return 0;
}

void __exit cleanup(void)
{
	printk(KERN_INFO MODULE_NAME ": exit\n");

	hw_free_irq();
}

module_init(init);
module_exit(cleanup);

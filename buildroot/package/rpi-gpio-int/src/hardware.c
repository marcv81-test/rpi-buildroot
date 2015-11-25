#include "hardware.h"

#include "core.h"

#include <linux/irq.h>
#include <linux/gpio.h>
#include <linux/spinlock.h>

#define GPIO_CHIP "bcm2708_gpio"
#define GPIO_PIN (18)

static spinlock_t irq_config_lock;

static struct gpio_chip *gpio_chip;
static struct irq_chip *irq_chip;
static struct irq_data *irq_data;

static int pin_irq;

static int check_gpio_chip_label(struct gpio_chip *chip, void *data)
{
	return strcmp(data, chip->label) == 0 ? 1 : 0;
}

int hw_init(void)
{
	// Find the GPIO chip
	gpio_chip = gpiochip_find("bcm2708_gpio", check_gpio_chip_label);
	if (!gpio_chip)
	{
		printk(KERN_WARNING MODULE_NAME ": " GPIO_CHIP " chip not found!\n");
		return -ENODEV;
	}

	// Find the pin IRQ
	pin_irq = gpio_chip->to_irq(gpio_chip, GPIO_PIN);
	if (pin_irq < 0)
	{
		printk(KERN_WARNING MODULE_NAME ": IRQ not found!\n");
		return -ENODEV;
	}

	// Find the pin IRQ data/chip
	irq_data = irq_get_irq_data(pin_irq);
	if (!irq_data || !irq_data->chip)
	{
		printk(KERN_WARNING MODULE_NAME ": IRQ data/chip not found!\n");
		return -ENODEV;
	}
	irq_chip = irq_data->chip;

	return 0;
}

inline int hw_pin_level(void)
{
	return gpio_chip->get(gpio_chip, GPIO_PIN);
}

int hw_request_irq(irq_handler_t irq_handler)
{
	int status;
	unsigned long flags;

	// Request the IRQ and attach the ISR
	status = request_irq(pin_irq, irq_handler, 0, MODULE_NAME, (void*) 0);
	if ((status == -EBUSY) || (status == -EINVAL))
	{
		printk(KERN_WARNING MODULE_NAME ": unable to request IRQ!\n");
		return -ENODEV;
	}

	// Configure the IRQ
	spin_lock_irqsave(&irq_config_lock, flags);
	irq_chip->irq_set_type(irq_data, IRQ_TYPE_EDGE_RISING | IRQ_TYPE_EDGE_FALLING);
	irq_chip->irq_unmask(irq_data);
	spin_unlock_irqrestore(&irq_config_lock, flags);

	return 0;
}

void hw_free_irq(void)
{
	unsigned long flags;

	// Reset the IRQ settings
	spin_lock_irqsave(&irq_config_lock, flags);
	irq_chip->irq_set_type(irq_data, 0);
	irq_chip->irq_mask(irq_data);
	spin_unlock_irqrestore(&irq_config_lock, flags);

	// Free the IRQ
	free_irq(pin_irq, (void *) 0);
}

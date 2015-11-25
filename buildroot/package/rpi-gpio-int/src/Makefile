rpi_gpio_int-objs := core.o hardware.o ring_buffer.o
obj-m := rpi_gpio_int.o

PWD := $(shell pwd)

default:
	$(MAKE) SUBDIRS=$(PWD) modules

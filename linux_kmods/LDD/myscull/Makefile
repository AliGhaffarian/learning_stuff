# If KERNELRELEASE is defined, we've been invoked from the
# kernel build system and can use its language.
ifneq ($(KERNELRELEASE),)
	obj-m := myscull.o
# Otherwise we were called directly from the command
# line; invoke the kernel build system.
else
	KERNELDIR ?= /lib/modules/$(shell uname -r)/build
	PWD := $(shell pwd)
default: myscull.c
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
endif

# seperate rm's to avoid errors such as no such file or directory when two wild card match the same filename
clean:
	rm  *.ko 
	rm *.mod* 
	rm ./modules.order 
	rm ./Module.symvers 
	rm *.o

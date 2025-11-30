ifneq ($(KERNELRELEASE),)
obj-m := hello1.o hello2.o
ccflags-y += -g
else
KDIR ?= /lib/modules/$(shell uname -r)/build
CROSS_COMPILE ?= arm-linux-gnueabihf-
ARCH ?= arm

default:
  $(MAKE) -C $(KDIR) M=$(PWD) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) modules
  $(CROSS_COMPILE)strip -g hello1.ko hello2.ko

clean:
  $(MAKE) -C $(KDIR) M=$(PWD) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) clean

.PHONY: default clean
endif

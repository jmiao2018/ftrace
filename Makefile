#KVERS = $(shell uname -r)

KERN_DIR = /usr/src/linux-headers-$(shell uname -r)

# Kernel modules
obj-m += ftrace.o

#KERN_DIR = /lib/modules/$(KERN_VER)/build

# Specify flags for the module compilation.
#EXTRA_CFLAGS=-g -O0


all:
#	make -C /lib/modules/$(KVERS)/build M=$(CURDIR) modules
#	KERN_DIR = /usr/src/linux-headers-5.8.0-63-generic
	make -C $(KERN_DIR) M=`pwd` modules

clean:
#	make -C /lib/modules/$(KVERS)/build M=$(CURDIR)  modules clean
	make -C $(KERN_DIR) M=`pwd` modules clean

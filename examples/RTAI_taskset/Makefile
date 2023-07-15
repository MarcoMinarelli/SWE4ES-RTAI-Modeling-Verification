
obj-m += taskset.o
KDIR := /lib/modules/$(shell uname -r)/build 
EXTRA_CFLAGS := -I/usr/realtime/include -I/usr/include -ffast-math -mhard-float

default:
	$(MAKE) -C $(KDIR) SUBDIRS=$(shell pwd) modules
	
clean:
	rm -f *.o *.ko *~

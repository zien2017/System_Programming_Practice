ifeq ($(KERNELRELEASE),)  

KERNELDIR ?= /lib/modules/$(shell uname -r)/build 
PWD := $(shell pwd)  

.PHONY: build clean

all: clean build sneaky_process

build: sneaky_mod.c
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules  

sneaky_process: sneaky_process.c
	gcc -std=gnu99 -Wall -pedantic -o sneaky_process sneaky_process.c 

clean:
	rm -rf *.o *~ core .depend .*.cmd *.order *.symvers *.ko *.mod.c *.mod sneaky_process
else  

$(info Building with KERNELRELEASE = ${KERNELRELEASE}) 
obj-m :=    sneaky_mod.o  

endif

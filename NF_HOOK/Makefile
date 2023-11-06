obj-m += steph_hook_l3.o

ccflags-y += -DDEBUG

modules:
	make -C /usr/src/linux-obj/x86_64/default M=`pwd` modules

clean:
	make -C /usr/src/linux-obj/x86_64/default M=`pwd` clean

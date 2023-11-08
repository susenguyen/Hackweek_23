TARGETS = test

all: $(TARGETS)
.PHONY: all

$(TARGETS): %: %.bpf.o 

%.bpf.o: %.bpf.c
	clang \
	    -target bpf \
		-I/usr/src/linux/tools/lib \
		-g \
	    -O2 -o $@ -c $<

clean: 
	- rm *.bpf.o
	- sudo rm -f /sys/fs/bpf/test

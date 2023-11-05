all: test 

test: test.c
	gcc -o test -lnetfilter_queue -lnfnetlink -O2 test.c

clean:
	rm -f test 

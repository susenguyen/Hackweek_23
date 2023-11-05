all: test 

test: test.c
	gcc -o test -lnetfilter_queue -lnfnetlink -O2 test.c

client: client.c
	gcc -o client client.c

clean:
	rm -f test client

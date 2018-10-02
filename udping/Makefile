CC=clang
CFLAGS=-g -O0 -pedantic -std=c99 -Wall -Wextra
LFLAGS=-pthread

udping.out: main.o
	$(CC) $(LFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -rf udping.out *.o

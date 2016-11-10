CC=gcc
CFLAGS=-Wall -g -std=c99
LD=gcc
LDFLAGS=-g -lrt -lm -lpthread

OBJS=threads.o msgqueue.o

all: produce

produce: $(OBJS)
	$(LD) -o $@ $^ $(LDFLAGS)

.c.o:
	$(CC) $(CFLAGS) -c $<

.PHONY: clean
clean:
	rm -f *.o produce

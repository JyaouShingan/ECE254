CC=gcc
CFLAGS=-Wall -g
LD=gcc
LDFLAGS=-g -lrt

OBJS=threads.o msgqueue.o

all: produce

produce: $(OBJS)
	$(LD) -o $@ $^ $(LDFLAGS)

.c.o:
	$(CC) $(CFLAGS) -c $<

.PHONY: clean
clean:
	rm -f *.o produce
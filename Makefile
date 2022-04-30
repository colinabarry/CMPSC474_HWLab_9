CC = gcc
CFLAGS = -g -o
LFLAGS = -lrt -lpthread

default: m

m: MUnit.c Queue.c Util.c
	$(CC) $(CFLAGS) $@ $^ $(LFLAGS)

r: RUnit.c Queue.c Util.c
	$(CC) $(CFLAGS) $@ $^ $(LFLAGS)
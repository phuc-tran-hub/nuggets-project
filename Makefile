# Makefile for Project 

S = support

C = common

OBJS1 = server.o
OBJS2 = client.o
LLIBS = $C/common.a $S/support.a

CC = gcc

CFLAGS = -Wall -pedantic -std=c11 -ggdb -Isupport -Icommon

MAKE = make
VALGRIND = valgrind --leak-check=full --show-leak-kinds=all

all: common.a server client

server: $(OBJS1) $(LLIBS)
	$(CC) $(CFLAGS) $^ $(LLIBS) -o $@

server.o: $C/file.h $S/message.h $C/player.h $C/gold.h $C/grid.h $C/game.h $C/visibility.h $C/compress.h

client: $(OBJS2) $(LLIBS)
	$(CC) $(CFLAGS) $^ $(LLIBS) -lncurses -o $@


client.o: $S/message.h $S/log.h

common.a:
	make -C common

clean:
	rm -f *core*
	rm -f server
	rm -f client
	rm -f *.o
	rm -f *#
	rm -f *~
	rm -f *.out
	make -C common clean

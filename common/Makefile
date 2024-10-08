# Makefile for common lobrary
#   Builds and compiles all these utility functions into a library.
#
# object files, and the target library

S = ../support

OBJS = player.o gold.o grid.o file.o game.o compress.o visibility.o
LIB = common.a

CFLAGS = -Wall -pedantic -std=c11 -ggdb -I$S $(FLAGS)

CC = gcc
MAKE = make

# Build for common.a
$(LIB): $(OBJS)
	ar cr $(LIB) $(OBJS)

compress.o: compress.h visibility.h grid.h player.h gold.h

file.o: file.h

game.o: game.h file.h gold.h grid.h player.h compress.h

gold.o: grid.h

grid.o: player.h gold.h file.h

player.o: grid.h $S/message.h compress.h visibility.h

visibility.o: grid.h player.h gold.h

.PHONY: clean

# clean up after build
clean:
	rm -f core
	rm -f $(LIB) *~ *.o
	rm -f *.gch
	rm -f *#
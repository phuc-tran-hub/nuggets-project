# Makefile for testing of the common directory
#
# Stanley Gao '24, CS50 21F for Nuggets Project
# Ethan Thomas, 2021

LL1 = ../common.a
LL1DIR = ../
SUPPORTL = ../../support/support.a
SUPPORTLDIR = ../../support

CFLAGS = -Wall -pedantic -std=c11 -ggdb -I$(LL1DIR) -I$(SUPPORTLDIR)
CC = gcc
MAKE = make
VALGRIND = valgrind --leak-check=full --show-leak-kinds=all # "myvalgrind"

all: gridtest vistest vistest_direct

gridtest: gridtest.o $(LL1)
	$(CC) $(CFLAGS) $^ $(LL1) -o $@

vistest: vistest.o $(SUPPORTL) $(LL1) 
	$(CC) $(CFLAGS) $^ $(SUPPORTL) $(LL1) -o $@

vistest_direct: vistest_direct.o $(SUPPORTL) $(LL1) 
	$(CC) $(CFLAGS) $^ $(SUPPORTL) $(LL1) -o $@

gridtest.o: $(LL1)
vistest.o: $(SUPPORTL) $(LL1)
vistest_direct.o: $(SUPPORTL) $(LL1)

.PHONY: clean run_gridtest valgrind_gridtest run_vistest valgrind_vistest run_vistest_direct valgrind_vistest_direct

run_gridtest: gridtest
	./gridtest

valgrind_gridtest: gridtest
	$(VALGRIND) ./gridtest

run_vistest: vistest
	./vistest

valgrind_vistest: vistest
	$(VALGRIND) ./vistest

run_vistest_direct: vistest_direct
	./vistest_direct

valgrind_vistest_direct: vistest_direct
	$(VALGRIND) ./vistest_direct

clean:
	rm -rf *.dSYM  # MacOS debugger info
	rm -f *~ *.o
	rm -f core
	rm -f gridtest
	rm -f vistest

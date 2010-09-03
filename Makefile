
CC=g++
CFLAGS=-c -Wall -O3 -fomit-frame-pointer

all: main

main: main.o myrandom.o stats.o uct.o deal.o utils.o dummyplayer.o
	$(CC) -o bridge main.o myrandom.o stats.o uct.o deal.o utils.o dummyplayer.o

myrandom.o: myrandom.cc myrandom.h
	$(CC) $(CFLAGS) myrandom.cc

main.o: main.cc 
	$(CC) $(CFLAGS) main.cc

stats.o: stats.cc  stats.h
	$(CC) $(CFLAGS) stats.cc

uct.o: uct.cc uct.h
	$(CC) $(CFLAGS) uct.cc

deal.o: deal.cc deal.h
	$(CC) $(CFLAGS) deal.cc

dummyplayer.o: dummyplayer.cc dummyplayer.h
	$(CC) $(CFLAGS) dummyplayer.cc

utils.o: utils.cc utils.h
	$(CC) $(CFLAGS) utils.cc
  
clean:
	rm bridge *.o


CC=g++
CFLAGS=-c -Wall -O3 -fomit-frame-pointer

all: main

main: main.o myrandom.o stats.o uct.o deal.o dummyplayer.o utils.o
	$(CC) -o bridge main.o myrandom.o stats.o uct.o deal.o dummyplayer.o utils.o

myrandom.o: myrandom.cc myrandom.h
	$(CC) $(CFLAGS) myrandom.cc

main.o: main.cc 
	$(CC) $(CFLAGS) main.cc

stats.o: stats.cc 
	$(CC) $(CFLAGS) stats.cc

uct.o: uct.cc 
	$(CC) $(CFLAGS) uct.cc

deal.o: deal.cc
	$(CC) $(CFLAGS) deal.cc

dummyplayer.o: dummyplayer.cc
	$(CC) $(CFLAGS) dummyplayer.cc

utils.o: utils.cc
	$(CC) $(CFLAGS) utils.cc
  
clean:
	rm bridge *.o

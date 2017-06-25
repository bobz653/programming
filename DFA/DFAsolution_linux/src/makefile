.SUFFIXES:.hpp .h .c .cpp .o

TARGETDIR = $(PWD)/../bin

CFLAGS = -I/usr/local/include -I$(PWD)/../include/ -I. -g
LIBS= -L/usr/local/lib -L../lib/ -L. -ldl -ltouni -lhz

CC=g++


all: NFATest


NFATest: NFATest.o NFABuilder.o
	$(CC) $(CFLAGS) -g -o $(TARGETDIR)/NFATest NFATest.o NFABuilder.o $(LIBS) 


.cpp.o:
	$(CC) $(CFLAGS) -c $<


clean:
	rm -f *.o
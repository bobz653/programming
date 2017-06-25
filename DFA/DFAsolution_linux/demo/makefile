.SUFFIXES:.hpp .h .c .cpp .o

TARGETDIR = $(PWD)/../bin

CFLAGS = -I/usr/local/include -I$(PWD)/../include/ -I. -g
LIBS= -L/usr/local/lib -L../lib/ -L. -ldl -ltouni -lhz -lRegRul

CC=g++


all: find


find: find.o
	$(CC) $(CFLAGS) -g -o $(TARGETDIR)/find find.o $(LIBS) 


.cpp.o:
	$(CC) $(CFLAGS) -c $<


clean:
	rm -f *.o
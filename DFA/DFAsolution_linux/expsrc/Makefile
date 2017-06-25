.SUFFIXES:.hpp .cpp .o

TARGETDIR = $(PWD)/../lib/

ifneq ($(DEBUG), 1)
	CFLAGS = -I/usr/local/include -I../include/ -pipe -g  -W
else	
	CFLAGS = -I/usr/local/include -I../include/ -pipe -g -Wall -W
endif

CC=g++
MAKE=make

LIB = libRegRul.a

libRegRul.a : NFABuilder.o
	ar r $(TARGETDIR)/$@ $?

.cpp.o:
	$(CC) $(CFLAGS) -c $< 

clean:
	rm -f *.o

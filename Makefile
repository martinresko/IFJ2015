# Subor:    Makefile


CC = gcc
CFLAGS = -Wall -std=c99 -pedantic -W
BIN = projekt
RM=rm -f -v
OBJECTS = main.o scaner.o parser.o

all: $(OBJECTS) error.h 
	$(CC) $(CFLAGS) $(OBJECTS) -o $(BIN)

scaner.o: scaner.h scaner.h
main.o: main.c
parser.o: parser.c parser.h

clean:
	$(RM) *.o $(BIN)

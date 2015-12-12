# Subor:    Makefile


CC = gcc
CFLAGS = -Wall -std=c99 -pedantic -W
BIN = projekt
RM=rm -f -v
OBJECTS = main.o scaner.o parser.o list.o expression.o stack.o bintree.o garbage.o interpret.o frames.o ial.o

all: $(OBJECTS) error.h 
	$(CC) $(CFLAGS) $(OBJECTS) -o $(BIN)

scaner.o: scaner.c scaner.h
main.o: main.c
parser.o: parser.c parser.h
expression.o: expression.c expression.h
list.o: list.c list.h
stack.o: stack.c stack.h
binTree.o: bintree.c bintree.h
garbage.o: garbage.c garbage.h
interpret.o: interpret.c interpret.h
frames.o : frames.c frames.h
ial.o : ial.c ial.h

clean:
	$(RM) *.o $(BIN)

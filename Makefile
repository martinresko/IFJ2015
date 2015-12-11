# Subor:    Makefile


CC = gcc
CFLAGS = -Wall -std=c99 -pedantic -W
BIN = projekt
RM=rm -f -v
OBJECTS = main.o scaner.o parser.o list.o expression.o table_of_symbols.o stack.o bintree.o garbage.o interpret.o frames.o

all: $(OBJECTS) error.h 
	$(CC) $(CFLAGS) $(OBJECTS) -o $(BIN)

scaner.o: scaner.c scaner.h
main.o: main.c
parser.o: parser.c parser.h
expression.o: expression.c expression.h
list.o: list.c list.h
table_of_symbols.o: table_of_symbols.c table_of_symbols.h
stack.o: stack.c stack.h
binTree.o: bintree.c bintree.h
garbage.o: garbage.c garbage.h
interpret.o: interpret.c interpret.h
frames.o : frames.c frames.h

clean:
	$(RM) *.o $(BIN)

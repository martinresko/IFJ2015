#
# Predmet: IFJ / IAL
# Subor:     Makefile
# Projekt:   Implementace interpretu imperativního jazyka IFJ15
#            tým 21, varianta a/3/I
# Autori:    <xhlava42@stud.fit.vutbr.cz>, Dominik Hlaváč Ďurán
#            <xdurco00@stud.fit.vutbr.cz>, Marián Ďurčo
#            <xdomon00@stud.fit.vutbr.cz>, Dávid Domonkoš
#            <xcerna06@stud.fit.vutbr.cz>, Peter Čerňanský
#            <xbaric01@stud.fit.vutbr.cz>, Filip Barič

CC = gcc
CFLAGS = -Wall -std=c11 -pedantic -W
BIN = projekt
RM=rm -f -v
OBJECTS = main.o scaner.o parser.o list.o expression.o stack.o garbage.o interpret.o frames.o ial.o

all: $(OBJECTS) error.h 
	$(CC) $(CFLAGS) $(OBJECTS) -o $(BIN)

scaner.o: scaner.c scaner.h
main.o: main.c
parser.o: parser.c parser.h
expression.o: expression.c expression.h
list.o: list.c list.h
stack.o: stack.c stack.h
garbage.o: garbage.c garbage.h
interpret.o: interpret.c interpret.h
frames.o : frames.c frames.h
ial.o : ial.c ial.h

clean:
	$(RM) *.o $(BIN)

#Subor:    Makefile

PROJ=table_of_symbols
FILES=list.c stack.c binTree.c table_of_symbols.c error.c expression.c garbage.c parser.c scaner.c main.c
CFLAGS=-std=gnu99 -Wall -Wextra -Werror -pedantic -pthread
CC=gcc
RM=rm -f

all : $(PROJ)

$(PROJ) : $(FILES)
		$(CC) $(CFLAGS) -o $@ $(FILES) -lm

clean :
	$(RM) *.o $(PROJ) 



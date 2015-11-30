# MAKEFILE

PROJ=expression
FILES=list.c expression.c
CFLAGS=-std=gnu99 -Wall -Wextra -Werror -pedantic -pthread
CC=gcc

all : $(PROJ)

$(PROJ) : $(FILES)
	$(CC) $(CFLAGS) -o $@ $(FILES) -lm


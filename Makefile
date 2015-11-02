# Subor:    Makefile


PROJ=ial
CFLAGS=-std=gnu99 -Wall -Wextra -Werror -pedantic -pthread
CC=gcc
RM=rm -f

$(PROJ) : $(PROJ).c
		$(CC) $(CFLAGS) -o $(PROJ) $(PROJ).c

clean :
	$(RM) *.o $(PROJ) 



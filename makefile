CFLAGS=-g -lm
CC=gcc

EXECS= score
all: $(EXECS)

clean: 
	rm -f *.0 $(EXECS)
score:score.c
	gcc -g -o score score.c

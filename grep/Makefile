SHELL := /bin/bash
FLAGS =-Wall -Wextra -Werror
FILE = test1.txt test2.txt
PATTERNS = paattern.reg


all: mygrep

mygrep: ./grep.c
	gcc $(CFLAGS) -o mygrep ./grep.c

clean:
	rm -f mygrep

rebuild: clean all
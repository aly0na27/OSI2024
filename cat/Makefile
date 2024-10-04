SHELL := /bin/bash
FLAGS =-Wall -Wextra -Werror
FILE = cat.c


all: os_cat tests

os_cat: clean
	gcc $(FLAGS) cat.c -o cat

tests:
	-diff <(./cat $(FILE)) <(cat $(FILE))
	-diff <(./cat -e $(FILE)) <(cat -e $(FILE))
	-diff <(./cat -t $(FILE)) <(cat -t $(FILE))
	-diff <(./cat -s $(FILE)) <(cat -s $(FILE))
	-diff <(./cat --squeeze-blank $(FILE)) <(cat -s $(FILE))
	-diff <(./cat -n $(FILE)) <(cat -n $(FILE))
	-diff <(./cat -b $(FILE)) <(cat -b $(FILE))
	-diff <(./cat --number $(FILE)) <(cat -n $(FILE))
	-diff <(./cat --number-nonblank $(FILE)) <(cat -b $(FILE))



clean :
	-rm *.o
# Флаги компилятора
CFLAGS = -Wall -g

all: mycat

mycat: ./cat.c
	gcc $(CFLAGS) -o mycat ./cat.c

clean:
	rm -f mycat

rebuild: clean all

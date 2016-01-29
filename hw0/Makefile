all: compile

compile: main.o wc.o map.o
	gcc -c main.o wc.o map.o -o compile

main.o: main.c
	gcc -c main.c

wc.o: wc.c
	gcc -c wc.c

map.o: map.c
	gcc -c map.c

clean:
	rm *o compile
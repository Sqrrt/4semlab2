all: lab2a lab2

lab2a: lab2a.c
	gcc -o lab2a lab2a.c

lab2: lab2.c
	gcc -o lab2 lab2.c

clean:
	rm -f lab2a lab2

p5make: cs2123p5.c cs2123p5Driver.c cs2123p5.h
	gcc -g -c cs2123p5Driver.c
	gcc -g -c cs2123p5.c
	gcc -g -o p5 cs2123p5Driver.o cs2123p5.o
	./p5 < p5Input.txt
all: foo

foo: cachesim.c
	gcc -Wall -lm -fsanitize=address cachesim.c -o cachesim -lm

clean:
	rm -rf cachesim

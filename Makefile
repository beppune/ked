
ked: main.c
	gcc -o ked main.c -Wall -Wextra -pedantic -std=c99

clean:
	rm -f *.o ked

run: ked
	./ked

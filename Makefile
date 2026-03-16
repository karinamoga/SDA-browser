build:
	gcc -Wall -Wextra -std=c99 -o tema1 main.c functii.c functiitask.c

run:
	./tema1

clean:
	rm -f tema1 tema1.out

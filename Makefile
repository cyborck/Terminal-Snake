all:
	gcc -Wall -g src/application.c src/terminal_interface.c src/snake.c -o snake

clean:
	rm snake

.PHONY: clean

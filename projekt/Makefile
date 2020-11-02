CFLAGS = -Wall -g -O -Wextra -Wall -std=gnu99 -g -c
LDFLAGS = `pkg-config --cflags --libs ncurses `
main: main.o
	gcc -o projekt projekt.o $(LDFLAGS) -lgmp
	chmod +x projekt
main.o: main.c
	gcc $(CFLAGS) main.c -o projekt.o $(LDFLAGS) -lgmp
clean:
	rm *.o
	rm projekt

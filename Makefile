CC=gcc
CFLAGS=-O0 -Wall -Iinc/ -g
LIBS=-lcurl -lpng

locationFinder:
	$(CC) src/main.c src/tools.c src/query.c src/error.c src/image.c $(CFLAGS) -o locationFinder $(LIBS)

clean:
	rm -f locationFinder


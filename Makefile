CC = gcc
SRCDIR = ./src
CFLAGS = -O0 -Wall -Iinc/ -g
LIBS = -lcurl -lpng
OBJ = main.o tools.o query.o error.o image.o overlay.o
EXEC = locationFinder

%.o: $(SRCDIR)/%.c
	$(CC) -c -o $@ $< $(CFLAGS)

$(EXEC): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f *.o


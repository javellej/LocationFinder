CC = gcc
SRCDIR = ./src
CFLAGS = -g -O0 -Wall -Werror -Iinc/
LIBS = -lm -lcurl -lpng
OBJ = main.o context.o tools.o query.o error.o image.o overlay.o coordinates.o
EXEC = locationFinder

%.o: $(SRCDIR)/%.c
	$(CC) -c -o $@ $< $(CFLAGS)

$(EXEC): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f *.o


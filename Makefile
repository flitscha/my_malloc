CC = gcc
CFLAGS = -Wall -Wextra

all: my_malloc

my_malloc: my_malloc.c 
	$(CC) $(CFLAGS) my_malloc.c -o my_malloc

clean:
	rm my_malloc
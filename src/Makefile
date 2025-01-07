CC = gcc
CFLAGS = -Wall -Wextra -Werror -pedantic -std=c11

all: cat test_main

.PHONY: test
test: clean test_main.c cat
	$(CC) $(CFLAGS) test_main.c -o test_main
	./test_main

.PHONY: cat
cat: main.c
	$(CC) $(CFLAGS) main.c -o cat

clean:
	rm -f cat test_main

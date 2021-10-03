CC = clang
CFLAGS = -Wall -Wpedantic -Werror -Wextra

all: mathlib-test

mathlib-test: mathlib-test.o
	$(CC) -o mathlib-test mathlib-test.o -lm

mathlib-test.o: mathlib-test.c bbp.c e.c euler.c madhava.c mathlib.h newton.c viete.c
	$(CC) $(CFLAGS) -c mathlib-test.c

clean:
	rm -f mathlib-test mathlib-test.o

format:
	clang-format -i -style=file *.{c,h}

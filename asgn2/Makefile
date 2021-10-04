CC = clang
CFLAGS = -Wall -Wpedantic -Werror -Wextra

objects = mathlib-test.o bbp.o e.o euler.o madhava.o viete.o newton.o

all: mathlib-test

mathlib-test: $(objects)  mathlib.h
	$(CC) -o mathlib-test mathlib-test.o -lm

mathlib-test.o: mathlib-test.c
	$(CC) $(CFLAGS) -c mathlib-test.c

bbp.o: bbp.c
	$(CC) $(CFLAGS) -c bbp.c

e.o: e.c
	$(CC) $(CFLAGS) -c e.c

euler.o: euler.c
	$(CC) $(CFLAGS) -c euler.c

madhava.o: madhava.c
	$(CC) $(CFLAGS) -c madhava.c

newton.o: newton.c
	$(CC) $(CFLAGS) -c newton.c

viete.o: viete.c
	$(CC) $(CFLAGS) -c viete.c


clean:
	rm -f mathlib-test *.o

format:
	clang-format -i -style=file *.{c,h}

CC = clang
CFLAGS = -Wall -Wpedantic -Werror -Wextra

objects = graph.o path.o stack.o tsp.o
headers = graph.h path.h stack.h vertices.h

all: tsp

tsp: $(objects)
	$(CC) -o tsp $^ -lm

%.o: %.c $(headers)
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f tsp *.o

format:
	clang-format -i -style=file *.c *.h

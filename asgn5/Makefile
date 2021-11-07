CC = clang
CFLAGS = -Wall -Wpedantic -Werror -Wextra

objects = code.o huffman.o io.o node.o pq.o stack.o encode.o
headers = code.h huffman.h io.h node.h pq.h stack.h header.h defines.h

all: encode decode

encode: $(objects)
	$(CC) -o encode $^ -lm

decode: $(objects)
	$(CC) -o decode $^ -lm

%.o: %.c $(headers)
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f encode decode *.o

format:
	clang-format -i -style=file *.c *.h

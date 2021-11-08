CC = clang
CFLAGS = -Wall -Wpedantic -Werror -Wextra

objects = code.o huffman.o io.o node.o pq.o stack.o
headers = code.h huffman.h io.h node.h pq.h stack.h header.h defines.h

all: encode decode

encode: $(objects) encode.o
	$(CC) -o encode $^ -lm

decode: $(objects) decode.o
	$(CC) -o decode $^ -lm

%.o: %.c $(headers)
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f encode decode *.o

format:
	clang-format -i -style=file *.c *.h

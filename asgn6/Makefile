CC = clang
CFLAGS = -Wall -Wpedantic -Werror -Wextra $(pkg-config --cflags gmp)

objects = numtheory.o randstate.o rsa.o
headers = numtheory.h randstate.h rsa.h

all: keygen encrypt

keygen: $(objects) keygen.o
	$(CC) -o keygen $^ -lgmp

encrypt: $(objects) encrypt.o
	$(CC) -o encrypt $^ -lgmp

%.o: %.c $(headers)
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f keygen encrypt decrypt *.o

format:
	clang-format -i -style=file *.c *.h

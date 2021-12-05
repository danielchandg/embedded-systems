CC = clang
CFLAGS = -Wall -Wpedantic -Werror -Wextra

objects = ht.o bst.o node.o bf.o bv.o parser.o speck.o banhammer.o
headers = ht.h bst.h node.h bf.h bv.h parser.h speck.h messages.h salts.h

all: banhammer

banhammer: $(objects)
	$(CC) -o banhammer $^ -lm

%.o: %.c $(headers)
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f banhammer *.o

format:
	clang-format -i -style=file *.c *.h


CC = clang
CFLAGS = -Wall -Wpedantic -Werror -Wextra

objects = heap.o shell.o insert.o quick.o stats.o sorting.o
headers = heap.h shell.h insert.h quick.h stats.h set.h

all: sorting

sorting: $(objects)
	$(CC) -o sorting $^ -lm

%.o: %.c $(headers)
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f sorting *.o *.gch

format:
	clang-format -i -style=file *.c *.h

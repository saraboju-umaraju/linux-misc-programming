CC = gcc
CFLAGS = -Wall -ggdb

hash: name_hash.o _val.o
	$(CC) -o $@ $^

clean:
	$(RM) name_hash.o _val.o hash


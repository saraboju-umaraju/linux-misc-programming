MISC_FLAGS += -DDEBUG
CFLAGS += $(MISC_FLAGS)
CFLAGS += -Wall
CFLAGS += -ggdb

CC = gcc

OBJECTS = main.o val.o put.o delete.o search.o misc.o show.o easy_swap.o remdup.o reverse.o addone.o findNth.o seperate.o
release : $(OBJECTS)
		@$(CC) $(CFLAGS) -o $@ $^
clean:
	@$(RM) $(OBJECTS) release

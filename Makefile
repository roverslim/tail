CC = gcc
CFLAGS = -std=c99 -g -Wall
INCLUDES = -Iinclude
INCLUDES_TEST = -Itest/include

SRCS_DIR = src/
SRCS = arguments.c parse_arguments.c tail_n.c tail.c

OBJS_DIR = obj/
OBJS = $(patsubst %.c,$(OBJS_DIR)%.o,$(SRCS))

MAIN = bin/my_tail

.PHONY: depend clean

all: $(MAIN)

test: obj/tail_n.o obj/unity.o obj/tail_n_test.o
	$(CC) $(CFLAGS) $(INCLUDES) $(INCLUDES_TEST) -o bin/tail_n_test obj/tail_n.o obj/unity.o obj/tail_n_test.o

$(MAIN): $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $(MAIN) $(OBJS)

obj/arguments.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c src/arguments.c -o $@

obj/parse_arguments.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c src/parse_arguments.c -o $@

obj/tail_n.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c src/tail_n.c -o $@

obj/tail_n_test.o:
	$(CC) $(CFLAGS) $(INCLUDES) $(INCLUDES_TEST) -c test/tail_n_test.c -o $@

obj/tail.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c src/tail.c -o $@

obj/unity.o:
	$(CC) $(CFLAGS) $(INCLUDES_TEST) -c test/unity.c -o $@

clean:
	$(RM) bin/* obj/*

depend: $(patsubst %,$(SRCS_DIR)%,$(SRCS))
	makedepend $(INCLUDES) $^


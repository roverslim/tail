CC = gcc
CFLAGS = -std=c99 -g -Wall
INCLUDES = -Iinclude

SRCS_DIR = src/
SRCS = arguments.c parse_arguments.c tail_n.c tail.c

OBJS_DIR = obj/
OBJS = $(patsubst %.c,$(OBJS_DIR)%.o,$(SRCS))

MAIN = bin/my_tail

.PHONY: depend clean

all: $(MAIN)

$(MAIN): $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $(MAIN) $(OBJS) $(LFLAGS)

obj/arguments.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c src/arguments.c -o $@

obj/parse_arguments.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c src/parse_arguments.c -o $@

obj/tail_n.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c src/tail_n.c -o $@

obj/tail.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c src/tail.c -o $@

clean:
	$(RM) bin/* obj/*

depend: $(patsubst %,$(SRCS_DIR)%,$(SRCS))
	makedepend $(INCLUDES) $^


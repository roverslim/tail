CC = gcc
CFLAGS = -std=c99 -g -Wall
INCLUDES = -Iinclude

SRCS = src/arguments.c src/parse_arguments.c src/tail_n.c src/tail.c
OBJS = $(SRCS:.c=.o)

MAIN = my_tail

.PHONY: depend clean

all: $(MAIN)

$(MAIN): $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $(MAIN) $(OBJS) $(LFLAGS)

.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	$(RM) bin/* obj/*

depend: $(SRCS)
	makedepend $(INCLUDES) $^


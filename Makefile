CC = gcc
CFLAGS = -std=c99 -g -Wall
INCLUDES = -Iinclude
INCLUDES_TEST = $(INCLUDES) -Itest/include

vpath %.c src test
vpath %.h include test/include
vpath %.o obj

.PHONY: all clean test

all: bin/tail

clean:
		$(RM) bin/* obj/*
		
test: bin/arguments_test bin/parse_arguments_test bin/tail_n_test
		./bin/arguments_test
		./bin/parse_arguments_test
		./bin/tail_n_test

bin/arguments_test: arguments_test.o arguments.o unity.o
		$(CC) $(CFLAGS) $(INCLUDES_TEST) -o $@ $^

bin/parse_arguments_test: parse_arguments_test.o arguments.o parse_arguments.o unity.o
		$(CC) $(CFLAGS) $(INCLUDES_TEST) -o $@ $^

bin/tail: arguments.o parse_arguments.o main.o tail_n.o
		$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^

bin/tail_n_test: tail_n_test.o arguments.o parse_arguments.o tail_n.o unity.o
		$(CC) $(CFLAGS) $(INCLUDES_TEST) -o $@ $^

obj/arguments.o: arguments.c arguments.h tail.h
		$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

obj/arguments_test.o: arguments_test.c arguments.h tail.h unity.h
		$(CC) $(CFLAGS) $(INCLUDES_TEST) -c $< -o $@

obj/main.o: main.c tail_n.h
		$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

obj/parse_arguments.o: parse_arguments.c parse_arguments.h
		$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

obj/parse_arguments_test.o: parse_arguments_test.c arguments.h parse_arguments.h unity.h
		$(CC) $(CFLAGS) $(INCLUDES_TEST) -c $< -o $@

obj/tail_n.o: tail_n.c arguments.h parse_arguments.h tail.h tail_n.h
		$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

obj/tail_n_test.o: tail_n_test.c arguments.h parse_arguments.h tail.h unity.h
		$(CC) $(CFLAGS) $(INCLUDES_TEST) -c $< -o $@

obj/unity.o: unity.c unity.h
		$(CC) $(CFLAGS) $(INCLUDES_TEST) -c $< -o $@

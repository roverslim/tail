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
		
test: bin/arguments_test bin/parse_arguments_test bin/tail_test
		./bin/arguments_test
		./bin/parse_arguments_test
		./bin/tail_test

bin/arguments_test: arguments_test.o arguments.o unity.o
		$(CC) $(CFLAGS) $(INCLUDES_TEST) -o $@ $^

bin/parse_arguments_test: parse_arguments_test.o arguments.o parse_arguments.o unity.o
		$(CC) $(CFLAGS) $(INCLUDES_TEST) -o $@ $^

bin/tail: arguments.o parse_arguments.o main.o tail.o
		$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^

bin/tail_test: tail_test.o arguments.o parse_arguments.o tail.o unity.o
		$(CC) $(CFLAGS) $(INCLUDES_TEST) -o $@ $^

obj/arguments.o: arguments.c arguments.h types.h
		$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

obj/arguments_test.o: arguments_test.c arguments.h types.h unity.h
		$(CC) $(CFLAGS) $(INCLUDES_TEST) -c $< -o $@

obj/main.o: main.c tail.h
		$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

obj/parse_arguments.o: parse_arguments.c parse_arguments.h types.h
		$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

obj/parse_arguments_test.o: parse_arguments_test.c arguments.h parse_arguments.h types.h unity.h
		$(CC) $(CFLAGS) $(INCLUDES_TEST) -c $< -o $@

obj/tail.o: tail.c arguments.h parse_arguments.h types.h tail.h
		$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

obj/tail_test.o: tail_test.c arguments.h parse_arguments.h types.h unity.h
		$(CC) $(CFLAGS) $(INCLUDES_TEST) -c $< -o $@

obj/unity.o: unity.c unity.h
		$(CC) $(CFLAGS) $(INCLUDES_TEST) -c $< -o $@

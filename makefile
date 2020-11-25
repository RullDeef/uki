CC := gcc
CFLAGS := -std=c99 -Wall -Werror -pedantic -Wextra -I.
LFLAGS :=

TEST_SRC := $(shell find ./test -name *.c)

.PHONY : build clean test

build :
	py -m quom --include_directory include src/uki.c uki.h

test : build ./bin/test.exe
	./bin/test.exe

bin/test.exe : $(TEST_SRC)
	mkdir -p bin; $(CC) $(CFLAGS) -o $@ $^ $(LFLAGS)

clean :
	rm -rf obj bin *.stackdump

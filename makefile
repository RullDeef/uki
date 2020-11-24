CC := gcc
CFLAGS := -std=c99 -Wall -Werror -pedantic -Wextra -I.
LFLAGS :=

.PHONY : build clean test

build :
	py -m quom --include_directory include src/uki.c uki.h

test : build
test : bin/test.exe
	./bin/test.exe

bin/test.exe : test/main.c
	mkdir -p bin; $(CC) $(CFLAGS) -o $@ $< $(LFLAGS)

clean :
	rm -rf obj bin *.stackdump

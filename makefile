CC := gcc
CFLAGS := -std=c99 -Wall -Werror -pedantic -Wextra -I.
LFLAGS :=

UKI_SRC := $(shell find ./src -name *.c)
UKI_HDR := $(shell find ./include -name *.h)

TEST_SRC := $(shell find ./test -name *.c)

.PHONY : build test clean

build : uki.h
	@echo "uki.h builded successfuly"

uki.h : $(UKI_HDR) $(UKI_SRC)
	py -m quom --include_directory include src/uki.c uki.h

test : ./bin/test.exe
	./bin/test.exe

bin/test.exe : $(TEST_SRC) uki.h
	mkdir -p bin; $(CC) $(CFLAGS) -o $@ $(TEST_SRC) $(LFLAGS)

clean :
	rm -rf obj bin *.stackdump

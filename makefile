CC := gcc
CFLAGS := -std=c99 -Wall -Werror -pedantic -Iinclude
LFLAGS := -Llib -luki

.PHONY : build clean test

build : CFLAGS += -DUKI_COMPILE
build : lib/libuki.a

test : bin/test.exe

bin/test.exe : \
		test/main.c \
		lib/libuki.a
	mkdir -p bin; $(CC) $(CFLAGS) -o $@ $< $(LFLAGS) 

lib/libuki.a : \
		obj/uki_input.o
	mkdir -p lib; ar -rc $@ $^

obj/uki_input.o : \
		src/uki_input.c \
		include/uki_defines.h \
		include/uki_input.h
	mkdir -p obj; $(CC) $(CFLAGS) -o $@ -c $<

clean :
	rm -rf obj bin lib *.stackdump

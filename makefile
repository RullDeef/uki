CC := gcc
CFLAGS := -std=c99 -Wall -Werror -pedantic -Wextra -Iheaders -Iinclude
LFLAGS := -Llib -luki

.PHONY : build clean test

build : lib/libuki.a
	./make_include.sh

test : build
test : bin/test.exe

bin/test.exe : \
		test/main.c \
		lib/libuki.a
	mkdir -p bin; $(CC) $(CFLAGS) -o $@ $< $(LFLAGS) 

lib/libuki.a : \
		obj/uki_input.o \
		obj/uki_table.o \
		obj/uki_menu.o \
		obj/uki_utils.o
	mkdir -p lib; ar -rc $@ $^

obj/uki_input.o : \
		src/uki_input.c \
		headers/uki_defines.h \
		headers/uki_input.h
	mkdir -p obj; $(CC) $(CFLAGS) -o $@ -c $<

obj/uki_table.o : \
		src/uki_table.c \
		headers/uki_defines.h \
		headers/uki_table.h
	mkdir -p obj; $(CC) $(CFLAGS) -o $@ -c $<

obj/uki_menu.o : \
		src/uki_menu.c \
		headers/uki_defines.h \
		headers/uki_menu.h
	mkdir -p obj; $(CC) $(CFLAGS) -o $@ -c $<
	
obj/uki_utils.o : \
		src/uki_utils.c \
		headers/uki_defines.h \
		headers/uki_utils.h
	mkdir -p obj; $(CC) $(CFLAGS) -o $@ -c $<

clean :
	rm -rf obj bin lib include *.stackdump

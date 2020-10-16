CC := gcc
CFLAGS := -std=c99 -Wall -Werror -pedantic -Iinclude
LFLAGS := -Llib -luki

.PHONY : build clean test

build : lib/libuki.a

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
		include/uki_defines.h \
		include/uki_input.h
	mkdir -p obj; $(CC) $(CFLAGS) -o $@ -c $<

obj/uki_table.o : \
		src/uki_table.c \
		include/uki_defines.h \
		include/uki_table.h
	mkdir -p obj; $(CC) $(CFLAGS) -o $@ -c $<

obj/uki_menu.o : \
		src/uki_menu.c \
		include/uki_defines.h \
		include/uki_menu.h
	mkdir -p obj; $(CC) $(CFLAGS) -o $@ -c $<
	
obj/uki_utils.o : \
		src/uki_utils.c \
		include/uki_defines.h \
		include/uki_utils.h
	mkdir -p obj; $(CC) $(CFLAGS) -o $@ -c $<

clean :
	rm -rf obj bin lib *.stackdump

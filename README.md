# Uki

Uki is a tiny singe-header C/C++ library for tiny CLI applications.

---

## Usage

Add header `uki.h` into your project and make a single `.c` file with the following code:

```C
#define UKI_IMPL
#include "uki.h"
```

Or you can insert implementation code (two lines up here ^) anywhere else inside **one and only one** of your `.c` files.

In order to use library functions you need to initialize library and destroy it properly

```C
int main()
{
    if (uki_init() != 0)
    {
        fprintf(stderr, "could not initialize uki lib\n");
        return -1;
    }

    // your code that uses uki_* funcs

    uki_destroy();
    return 0;
}
```

---

## Tables

```C
unsigned int rows = 2U, cols = 3U;

// create fixed-size table
uki_table_t table = uki_table_create(rows, cols);


unsigned int row = 0U, col = 1U;
char text[] = "some usefull text";

// to write something in a table, use the following function:
uki_table_write(table, row, col, text);
uki_table_write(table, 1U, col, "at row 1");

// to output a table to a file use `*_print` fucntion:
uki_table_print(table, stdout);

// output:
//
// ┌────────┬─────────────────┬┐
// │        │some usefull text││
// │at row 1│                 ││
// └────────┴─────────────────┴┘
```

---

## Context menu

Uki has different types of menu. Most simple is context menu.

Context menu provides you with clean and elegant way of receiving input of type *radio* from standard input.

```C
// creates completely empty menu, except prompt string is ">>> "
uki_menu_t menu = uki_menu_ctx_create();

// add an info message that would describe
// what you are expect user to input
uki_menu_ctx_info_set(menu, "8 / 2 + 2 = ?");

// add options to your menu
uki_menu_ctx_opt_add(menu, "maybe 2", (void *)2);
uki_menu_ctx_opt_add(menu, "o, no, it is 6!", (void *)6);

// rerun it any time you want
int answer = 0;
uki_menu_ctx_run(menu, (void **)&answer);

// check if the answer was correct
if (answer == 6)
    printf("You are right! It is 6.\n");
else
    printf("You are wrong. The answer is 6.\n");
```

If you run the code above you will see this menu output and prompt string:

```
8 / 2 + 2 = ?
   1. maybe 2
   2. o, no, it is 6!
>>>
```

Choosing second option by entering number 2 will produce the following output:

```
>>> 2
You are right! It is 6.
```

When menu becomes useless you need to destroy it in such way:

```C
uki_menu_destroy(menu);
```

---

## Further customization

The list of marcos that you can define just before implementation:

- **Memory allocators**
  - `UKI_MALLOC(size)` - function that allocates memory of size `size` (default: `malloc(size)`)
  - `UKI_FREE(ptr)` - function that frees memory allocated by `UKI_MALLOC` (default: `free(prt)`)
- **Regular table macros**
  - `UKI_TABLE_PRINT_MAX_WIDTH` - maximum width that table being printed can occupy (default: `80U`)
  - `UKI_TABLE_STRBUF_SIZE` - start string buffer size of each cell in all tables. (default: `64U`)
  - `UKI_TABLE_MAX_TABLES_COUNT` - maximum tables amount that can be handled simultaneously (default: `255U`)
- **Menu macros**
  - `UKI_MENU_INFO_SIZE` - the info message buffer size for any type of menu (default: `64U`)
  - `UKI_MENU_PROMPT_SIZE` - the prompt message buffer size for any type of menu (default: `32U`)
  - `UKI_MENU_MAX_MENUS_COUNT` - maximum menus amount that can be handled simultaneously (default: `31U`)

Example of usage:

```C
// file: uki_impl.c
#define UKI_TABLE_STRBUF_SIZE 16U
#define UKI_MENU_INFO_SIZE 20U

#define UKI_IMPL
#include "uki.h"
```
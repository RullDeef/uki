# Uki

Library for handling:
- console input
- table output
- menu loop

---

## Input usage

Most of the input functions have similar naming:

```C
bool uki_input_<type>[_force][_constraint|_minmax](const char* msg, const char *err_msg, <type> *value);
```

postfix `_force` means that input will happen in a loop until correct input not received.
postfix `_minmax` available for numeric data types. Suplies function with external args - minimum and maximum possible values.
postfix `_constraint` must be used for custom input validators.


input function returns `true` if it was successful.

## Table usage

In order to use a structured output in a form of a table you need to create a `uki_table_t` variable:

```C
int rows = 2;
int cols = 3;
const char *title = "my awesome uki_table";

uki_table_t table = uki_table_create(rows, cols, title);
```

All uki structs are static, so you dont need to manage dynamic memory allocations.

To write something in a table, use one of the following functions:

```C
int row = 0;
int col = 1;
char text[] = "some usefull text";

uki_table_set(&table, row, col, text);
// and/or
uki_table_set_fmt(&table, row + 1, col - 1, "at row %d", row);
```

To output a table use corresponding print fucntion:

```C
uki_table_print(&table);

// outputs:
//
// ┌─────────────────────────────────┐
// │       my awesome uki_table      │
// ├──────────┬───────────────────┬──┤
// │          │ some usefull text │  │
// │ at row 0 │                   │  │
// └──────────┴───────────────────┴──┘
```

## Menu usage

To add a menu in you program, firstly you need to define a functions, that uki could call within menu loop. To acheive that, you need to follow given functions signature:

```C
int my_menu_func_1(void *data);
int my_menu_func_2(void *data);
int my_menu_func_3(void *data);
```

After that, in place, where you want to start menu you need to create a `uki_menu_t` variable and run menu as follows:

```C
uki_menu_t menu = uki_menu_create("Menu title", 3,
        "cool func 1", my_menu_func_1,
        "next func", my_menu_func_2,
        "last one", my_menu_func_3
    );

uki_menu_run(&menu, NULL);
```

Instead of `NULL` you can pass pointer to any data you want - that is what those void pointers in your functions for. This data is shared between all your menu options.

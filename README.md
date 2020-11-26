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

---

## Table usage

In order to use a structured output in a form of a table you need to create a `uki_table_t` variable:

```C
unsigned int rows = 2U, cols = 3U;

uki_table_t table = uki_table_create(rows, cols);
```

To write something in a table, use the following function:

```C
unsigned int row = 0U, col = 1U;
char text[] = "some usefull text";

uki_table_write(table, row, col, text);
```

To output a table to a file use `*_print` fucntion:

```C
uki_table_print(table, stdout);

// output:
//
// ┌──────────┬───────────────────┬──┐
// │          │ some usefull text │  │
// │ at row 0 │                   │  │
// └──────────┴───────────────────┴──┘
```

---

## Further customization

The list of marcos that you can define just before implementation:

- **Memory allocators**
  - `UKI_MALLOC(size)` - function that allocates memory of size `size` (default: `malloc(size)`)
  - `UKI_FREE(ptr)` - function that frees memory allocated by `UKI_MALLOC` (default: `free(prt)`)
- **Regular table macros**
  - `UKI_TABLE_STRBUF_SIZE` - defines start string buffer size of each cell in all tables. (default: `64U`)

Example of usage:

```C
// file: uki_impl.c
#define UKI_TABLE_STRBUF_SIZE 16U

#define UKI_IMPL
#include "uki.h"
```
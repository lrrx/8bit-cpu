#pragma once

//TODO: replace these with custom implementation

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "types.h"

#define ASSERT(statement) \
if(!(statement)) {printf( "assertion failed: " #statement ); exit(-1);}

i16 find_str_in_list(cstr str, cstr* const list, size_t const listLength);

u16 str_to_i16(cstr str);

u16 hexstr_to_i16(cstr hexStr);

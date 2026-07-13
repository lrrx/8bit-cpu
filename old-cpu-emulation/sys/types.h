#pragma once

#include "platform.h"

// i16types
#ifdef EMULATOR
#include <inttypes.h>
#include <stddef.h>
#include <stdbool.h>

typedef uint8_t u8;
typedef uint16_t u16;

typedef int8_t i8;
typedef int16_t i16;

//fixed poi16floating point
typedef i16 ff;

#else

//Platform internals (only known to compiler for cpu arch)

typedef __cpu_u8__ u8;
typedef __cpu_u16__ u16;

typedef __cpu_i8__ i8;
typedef __cpu_i16__ i16;

#endif


typedef char const * const cstr;

typedef struct {
    u8 x, y;
} u8vec2;

typedef struct {
    u16 x, y;
} u32vec2;
#pragma once

#include "sys/sys.h"

typedef uint8_t Reg;
union RegWide {
    struct {
        Reg h, l;
    };
    uint16_t hl;
};

struct State {
    Reg a, b, c, d;
    Reg sr, sw; //segments for read/write
    union RegWide ip, bp, sp; //count each as double due to high and low part
    union {
        Reg f;
        struct {
            u16 : 12; //unused
            u16 equal : 1;
            u16 zero : 1;
            u16 less : 1;
            u16 carry : 1;
        } flags;
    };
};

extern struct State state;

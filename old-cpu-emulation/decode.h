#pragma once

#include "ops.h"

#include "sys/sys.h"

union RawRA {
    u16 bitsIn;
    struct {
        u8 : 2;
        u8 op : 2;
        u8 reg : 4;
        u8 addr : 8;
    };
};

union Raw {
    u16 bitsIn;
    struct {
        u8 high8;
        u8 low8;
    };
    struct {
        u8 mode : 2;
        u8 : 1;
        u8 op : 5;
        union {
            u8 addr : 8;
            struct {
                u8 reg2 : 4;
                u8 reg : 4;
            };
        };
    };
};

struct Instr {
    enum OP op;
    u8 addr;
    u8 reg;
    u8 reg2;
};

struct Instr decode(u16 const in);

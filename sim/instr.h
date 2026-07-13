#pragma once

#include "sys.h"


union RawImm {
    u16 in;
    struct {
        u8 cmd0 : 4,
        u8 : 1
        u8 reg : 3,
        u8 imm : 8
    };
};

union Raw3Reg {
    u16 in;
    struct {
        u8 cmd0 : 4,
        u8 : 1
        u8 reg : 3,
        u8 IMOPb : 1,
        u8 cmd1 : 1,
        u8 reg_op1 : 3,
        u8 reg_op0 : 3,
    };
};

struct Instr {
    int OP;
    u8 imm;
    u8 reg_result;
    u8 reg1, reg0;
}

struct Instr decode(u16 const in);

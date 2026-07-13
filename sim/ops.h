#pragma once

enum OP {
    ADD,
    SUB,
    MUL,
    DIV,
    NEG,

    SHL,
    SHR,
    AND,
    OR,
    NOT,
    XOR,
}

enum OPFORMAT {
    F_REG_IMM,
    F_3REG,
    F_REG
};

struct OpInfo {
    char name[4];
    enum OPFORMAT OpFormat;
    u8 OpEncoded;
};

struct OpInfo opInfos[OP_COUNT + 1] = {
    //reg and addr [12 bits arguments), first to ensure id 0 to 3, these four need to be handled separately because they have a different layout
    {"MMR", F_REG_IMM, 0x0 }, // memory read to register
    {"MMW", F_REG_IMM, 0x1 }, // memory write from register
    {"IMR", F_REG_IMM, 0x2 }, // write immediate to register
    {"IMW", F_REG_IMM, 0x3 }, // write immediate to address contained in register

    //two regs, or one and other is discarded or taken as immediate
    {"ADD", F_REG_REG, 0x00 | 0x40 }, // addition
    {"SUB", F_REG_REG, 0x01 | 0x40 }, // subtraction
    {"NOT", F_REG_REG, 0x02 | 0x40 }, // bitwise not
    {"AND", F_REG_REG, 0x03 | 0x40 }, // bitwise and
    {"ORR", F_REG_REG, 0x04 | 0x40 }, // bitwise or
    {"XOR", F_REG_REG, 0x05 | 0x40 }, // bitwise xor
    {"SHL", F_SIM_REG, 0x06 | 0x40 }, // left shift
    {"SHR", F_SIM_REG, 0x07 | 0x40 }, // rights shift
    {"SRL", F_REG_REG, 0x08 | 0x40 }, // left shift by value in register
    {"SRR", F_REG_REG, 0x09 | 0x40 }, // right shift by value register
    {"INC", F_REG_REG, 0x0A | 0x40 }, // ++
    {"DEC", F_REG_REG, 0x0B | 0x40 }, // --
    {"JPR", F_REG_REG, 0x0C | 0x40 }, // jump to addr in register
    {"CLR", F_REG_REG, 0x0D | 0x40 }, // call to subroutine at addr in register
    {"PSH", F_REG, 0x0E | 0x40 }, // push to stack
    {"POP", F_REG, 0x0F | 0x40 }, // pop from stack
    {"MRR", F_REG_REG, 0x10 | 0x40 }, // memory read *reg to reg
    {"MRW", F_REG_REG, 0x11 | 0x40 }, // memory write *reg to reg
    {"CMP", F_REG_REG, 0x12 | 0x40 }, // compare

    //single address [local jump, direct push)
    {"JMP", F_IMM, 0x00 | 0x80 }, // jump
    {"JPE", F_IMM, 0x01 | 0x80 }, // jump equal
    {"JNE", F_IMM, 0x02 | 0x80 }, // jump not equal
    {"JPZ", F_IMM, 0x03 | 0x80 }, // jump zero
    {"JNZ", F_IMM, 0x04 | 0x80 }, // jump not zero
    {"JPC", F_IMM, 0x05 | 0x80 }, // jump carry
    {"JNC", F_IMM, 0x06 | 0x80 }, // jump not carry
    {"JPM", F_IMM, 0x07 | 0x80 }, // jump more
    {"JPL", F_IMM, 0x08 | 0x80 }, // jump less
    {"PSI", F_IMM, 0x09 | 0x80 }, // push immediate

    //no arguments
    {"NOP", F_NONE, 0x00 | 0xC0 }, // no operation
    {"RET", F_NONE, 0x01 | 0xC0 } // return from subroutine
}

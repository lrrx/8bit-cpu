from enum import Enum

class CTRL(Enum):
    RF_WE = 0
    RAM_WE = 1
    IO_WE = 2
    RF_INP_USE_MEM = 3
    RF_INP_USE_IO = 4
    RF_FORCE_WRITE_PC = 5
    SET_IO_REG_PAGE = 6
    REG_A_IS_REG_RES = 7
    WHICH_FLAG_EXPECTED = 8
    FLAG_EXPECTED_VALUE = 9
    ENABLE_FLAG_EXPECTATION = 10
    SET_X_TO_IMM3_FROM_REG_RES = 11
    ALU_PASS_Y_INSTEAD = 12
    ALU_ENABLE_PASSING = 13
    ALU_WE = 14
    INCREMENT_PC_AND_FINISH = 15

class OP(Enum):
    ADD     = 0x0
    SUB     = 0x1
    MUL     = 0x2
    DIV     = 0x3
    SHL     = 0x4
    SHR     = 0x5
    NEG     = 0x6
    PASS_X  = 0x7
    PASS_Y  = 0x8
    AND     = 0x9
    OR      = 0xA
    XOR     = 0xB
    NXOR    = 0xC
    NOT     = 0xD
    ADDC    = 0xE
    SUBC    = 0xF		

RI  = 2
DATA= 0
RRR = 1
RRI = 3
#include "instr.h"

union InstrWord {
    u8 word,
    struct {
        u8 IMOPb : 1,
        u8 IMOPa : 1,
        u8 cmd1 : 1,
        u8 cmd0 : 4
    };
};

struct Instr decode(u16 const in) {
    u8 type = (in >> 11) & 1;
    if(type == 1) {
        union RawImm const rawImm = {.in = in};    
        return (struct Instr){.op = rawImm.
    }

    return (struct Instr) {.op = 
}

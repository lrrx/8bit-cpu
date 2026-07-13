#include "decode.h"

struct Instr decode(u16 const in) {
    if(in >> 14 == 0) {
        //RawRA layout
        union RawRA const rawRA = {.bitsIn = in};
        return (struct Instr){.op = rawRA.op,
            .addr = rawRA.addr,
            .reg = rawRA. reg
        };
    }
    else {
        //Raw layout
        union Raw const raw = {.bitsIn = in};
        return (struct Instr){.op = raw.op | raw.mode << 8,
            .addr = raw.addr,
            .reg = raw.reg,
            .reg2 = raw.reg2
        };
    }
}

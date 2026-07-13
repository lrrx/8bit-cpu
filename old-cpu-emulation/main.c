#include "state.h"
#include "decode.h"
#include "execute.h"
#include "memory.h"
#include "ops.h"

#include "stdio.h"
#include "stdint.h"
#include "string.h"

uint8_t memory[MEMORY_SIZE];
struct State state;

#ifdef EMULATOR
int main() {
#else
i16 main() {
#endif
    u16 i = 0;

    uint16_t* const mem16 = (uint16_t*)memory; 
    
    while(true) {
        uint16_t const nextInstruction = mem16[state.ip.hl];
        printf("%x:\t %x\t\t", state.ip.hl, nextInstruction);
        state.ip.hl += 1; // if instruction is jump, ip will be overwritten at execute
        struct Instr instr = decode(nextInstruction);
        instr.op = IMR;
        instr.reg = 0;
        instr.addr = 0x94;

        printf("op: %02x, addr: %02x, reg: %02x, reg2 %02x\t\t", instr.op % 32, instr.addr, instr.reg, instr.reg2);
        printf("State: %p %02x %02x %02x %02x %04x %04x %02x", &state.a, state.a, state.b, state.c, state.d, state.ip.hl, state.sp.hl, state.f);
        printf("\n");

        execute(instr);
        i++;
        if(i > 100) break;
    }

    return 0;
}

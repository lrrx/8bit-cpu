#include "memory.h"
#include "execute.h"
#include "state.h"
#include "ops.h"

#include "stdint.h"
#include "stdio.h"

extern uint8_t memory[MEMORY_SIZE];
extern struct State state;

uint8_t pop() {
    return memory[state.sp.hl--];
}

void push(uint8_t const value) {
    uint8_t * const stack_top = &memory[state.sp.hl++];
    *stack_top = value;
}

void cmp(Reg const a, Reg const b) {
    state.flags.equal = a == b;
    state.flags.less = a < b;
}

void jump(uint16_t addr) {state.ip.hl = addr;}

void call(uint16_t addr) {
    push(++state.ip.hl);
    jump(addr);
}

void ret() {
    jump(pop());
}

void execute(struct Instr const instr) {
    static i16x = 0;
    state.b = x++;
    Reg * const reg = (&state.a) + instr.reg;
    Reg * const reg2 = (&state.a) + instr.reg2;

    uint8_t * const mw = &memory[state.sw + instr.addr];
    uint8_t * const mr = &memory[state.sr + instr.addr];
    uint8_t const a = instr.addr;

    uint8_t * const stack_top = &memory[state.sp.hl];

    switch(instr.op) {
       case MMR: *reg = *mr;
break; case MMW: *mw = *reg;
break; case IMR: *reg = a; 
break; case IMW: memory[state.sw + *reg] = a;
        
break; case ADD: *reg += *reg2;
break; case SUB: *reg -= *reg2;
break; case NOT: *reg = ~*reg;
break; case AND: *reg &= *reg2;
break; case ORR: *reg |= *reg2;
break; case XOR: *reg ^= *reg2;
break; case SHL: *reg <<= instr.reg2;
break; case SHR: *reg >>= instr.reg2;
break; case SRL: *reg <<= *reg2;
break; case SRR: *reg >>= *reg2;
break; case INC: (*reg)++;
break; case DEC: (*reg)--;
break; case JPR: jump(*reg); 
break; case CLR: call(*reg);
break; case PSH: push(*reg);
break; case POP: *reg = pop();
break; case MRR: *reg = memory[*reg2];
break; case MRW: memory[*reg2] = *reg;
break; case CMP: cmp(*reg, *reg2);

break; case JMP: jump(a);
break; case JPE: if(state.flags.equal)  jump(a);
break; case JNE: if(!state.flags.equal) jump(a);
break; case JPZ: if(state.flags.zero)   jump(a);
break; case JNZ: if(!state.flags.zero)  jump(a);
break; case JPC: if(state.flags.carry)  jump(a);
break; case JNC: if(!state.flags.carry) jump(a);
break; case JPM: if(!state.flags.less)  jump(a);
break; case JPL: if(state.flags.less)  jump(a);
break; case PSI: push(a);

break; case NOP:
break; case RET: ret();  

    };
}

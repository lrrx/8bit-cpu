#!/usr/bin/env python3
from cpu_enums import *
from dataclasses import dataclass
import array

@dataclass
class Instr:
    name : str
    op_in: int
    format: int
    steps: [CTRL]

def get_bits(instr: Instr, step: int) -> int:
    bits : int = 0

    for c in instr.steps[step]:
        bits |= 1 << c.value

    return bits

def get_mem_word_loc(instr: Instr) -> int:
    o_id = int(instr.op_in)
    assert(0 <= o_id < 16)
    steps_per_instr = 2
    instr_per_format = 16
    return (instr.format * instr_per_format + o_id) * steps_per_instr

def create_mem_image(instructions : [Instr]):
    words = array.array('H', [0] * 128)
    for i in instructions:
        loc = get_mem_word_loc(i)
        words[loc] = get_bits(i, 0)
        words[loc + 1] = get_bits(i, 1)


    lines = []
    for i in range(0, len(words), 16):
        words_on_line = words[i:i+16]
        line = f'{i:02x}: ' + ' '.join(f'{word:04x}' for word in words_on_line)
        lines.append(line)

    out_str = 'v3.0 hex words addressed\n'
    out_str += '\n'.join(lines)

    return out_str

def generate_ALU_instruction(name : str, op: OP, is_RRI: bool) -> Instr:
    steps = [[CTRL.ALU_WE],
             [CTRL.INCREMENT_PC_AND_FINISH, CTRL.RF_WE]]

    format = RRI if is_RRI else RRR

    return Instr(name, op.value, format, steps)

def print_logic_table(instructions: [Instr]) -> None:
    print(' ' * 35, end='')
    for i in instructions:
         print(i.name.ljust(8), end='')
    #print()
    #print(' ' * 35, end='') 
    #for i in instructions:
    #    code = (i.format << 4) + i.op_in
    #    print(f'{code:06b}'.ljust(8), end='')
    print()

    for c in CTRL:
        print(str(c).ljust(35), end='')
        for i in instructions:
            s = ''
            if(c in i.steps[0]): s+='0'
            else: s+='.'
            if(c in i.steps[1]): s+='1'
            else: s+='.'
            print(s.ljust(8), end='')
        print()

def get_all_instructions():
    instructions = []
    #RI instructions

    #TODO: use 2-bit encoding of 4 input sources
    # we have 3 RF input sources as of now, but adding
    # immediate as input source would allow us to finish
    # RI instructions (and perhaps others) after 1 cycle 

    instructions += [
    Instr('LI' , 0xf, RI, [ #could be culled (see TODO), we don't really need to go through ALU
        [CTRL.ALU_WE, CTRL.ALU_ENABLE_PASSING, CTRL.ALU_PASS_Y_INSTEAD],
        [CTRL.INCREMENT_PC_AND_FINISH, CTRL.RF_WE],
    ]),
    Instr('IN' , 0xe, RI,[  #could be culled (see TODO), we don't really need to go through ALU
        [CTRL.ALU_WE, CTRL.ALU_ENABLE_PASSING],
        [CTRL.INCREMENT_PC_AND_FINISH, CTRL.RF_INP_USE_IO, CTRL.RF_WE],

    ]),Instr('OUT', 0xd, RI,[ #could be culled (see TODO), we don't really need to go through ALU
        [CTRL.INCREMENT_PC_AND_FINISH, CTRL.REG_A_IS_REG_RES, CTRL.IO_WE, CTRL.ALU_ENABLE_PASSING, CTRL.ALU_PASS_Y_INSTEAD],
        [],

    ]),Instr('JNC' ,0xc, RI,[ #could be culled (see TODO), we don't really need to go through ALU
        [CTRL.ALU_WE, CTRL.ALU_ENABLE_PASSING, CTRL.ALU_PASS_Y_INSTEAD],
        [CTRL.INCREMENT_PC_AND_FINISH, CTRL.ENABLE_FLAG_EXPECTATION, CTRL.WHICH_FLAG_EXPECTED, CTRL.RF_WE, CTRL.ALU_ENABLE_PASSING, CTRL.ALU_PASS_Y_INSTEAD],

    ]),Instr('JNZ' ,0xb, RI,[ #could be culled (see TODO), we don't really need to go through ALU
        [CTRL.ALU_WE, CTRL.ALU_ENABLE_PASSING, CTRL.ALU_PASS_Y_INSTEAD],
        [CTRL.INCREMENT_PC_AND_FINISH, CTRL.ENABLE_FLAG_EXPECTATION, CTRL.RF_WE],

    ]),Instr('JC' , 0xa, RI,[ #could be culled (see TODO), we don't really need to go through ALU
        [CTRL.ALU_WE, CTRL.ALU_ENABLE_PASSING, CTRL.ALU_PASS_Y_INSTEAD],
        [CTRL.INCREMENT_PC_AND_FINISH, CTRL.ENABLE_FLAG_EXPECTATION, CTRL.FLAG_EXPECTED_VALUE, CTRL.WHICH_FLAG_EXPECTED, CTRL.RF_WE]

    ]),Instr('JZ' , 0x9, RI,[ #could be culled (see TODO), we don't really need to go through ALU
        [CTRL.ALU_WE, CTRL.ALU_ENABLE_PASSING, CTRL.ALU_PASS_Y_INSTEAD],
        [CTRL.INCREMENT_PC_AND_FINISH, CTRL.ENABLE_FLAG_EXPECTATION, CTRL.FLAG_EXPECTED_VALUE, CTRL.RF_WE],

    ]),Instr('JL' , 0x8, RI,[ #could be culled (see TODO), we don't really need to go through ALU
        [CTRL.ALU_WE, CTRL.SET_X_TO_IMM3_FROM_REG_RES, CTRL.SET_IO_REG_PAGE, CTRL.ALU_ENABLE_PASSING, CTRL.ALU_PASS_Y_INSTEAD],
        [CTRL.INCREMENT_PC_AND_FINISH, CTRL.RF_WE, CTRL.RF_FORCE_WRITE_PC], #DONE: need to add logic for setting target register to pc on this step

    ])
    ]

    #DATA instructions
    instructions += [
    Instr('MOV', 0x0, DATA,[
        [CTRL.ALU_WE, CTRL.ALU_ENABLE_PASSING],
        [CTRL.INCREMENT_PC_AND_FINISH, CTRL.RF_WE], #could be culled (see TODO), we don't really need to go through ALU

    ]),Instr('LD' , 0x4, DATA,[
        [CTRL.ALU_WE, CTRL.ALU_ENABLE_PASSING], #could be culled (see TODO), we don't really need to go through ALU
        [CTRL.INCREMENT_PC_AND_FINISH, CTRL.RF_WE, CTRL.RF_INP_USE_MEM],

    ]),Instr('ST' , 0x8, DATA,[
        [CTRL.ALU_WE, CTRL.ALU_ENABLE_PASSING], #could be culled (see TODO), we don't really need to go through ALU
        [CTRL.INCREMENT_PC_AND_FINISH, CTRL.REG_A_IS_REG_RES, CTRL.RAM_WE],

    ]),Instr('JLR', 0xc, DATA,[
        [CTRL.ALU_WE, CTRL.ALU_ENABLE_PASSING], #could be culled (see TODO), we don't really need to go through ALU
        [CTRL.INCREMENT_PC_AND_FINISH, CTRL.REG_A_IS_REG_RES, CTRL.SET_IO_REG_PAGE, CTRL.IO_WE, CTRL.RF_WE, CTRL.RF_FORCE_WRITE_PC], #DONE: need to add logic for setting target register to pc on this step

    ]),Instr('INR', 0xd, DATA ,[
        [CTRL.INCREMENT_PC_AND_FINISH, CTRL.RF_INP_USE_IO, CTRL.RF_WE, CTRL.ALU_ENABLE_PASSING],
        [],

    ])
    ]

    #generate ALU instructions
    for op in OP:
        instructions.append(generate_ALU_instruction(str(op._name_), op, False))
        instructions.append(generate_ALU_instruction(str(op._name_) + 'I', op, True))

    return instructions

def run_main():
    instructions = get_all_instructions()
    print_logic_table(instructions)
    image_str = create_mem_image(instructions)
    print(image_str)
    with open("control_bit_memory.txt", "w") as f:
        f.write(image_str)


if __name__ == "__main__":
    run_main()
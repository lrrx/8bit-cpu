#!/usr/bin/env python3
from sys import argv
from isa import get_all_instructions
from cpu_enums import *

def run_main():
    isa = get_all_instructions()

    with open(argv[1], "r") as f:
        lines = f.readlines()
    lines = [line.split() for line in lines] #split by whitespaces
    lines = [line for line in lines if len(line) > 0] #filter empty lines

    instr_list = []
    code = []

    instr_count = 0
    labels = []

    #grab label positions
    for x, line in enumerate(lines):
        index = line[0].find(':')
        if index > 0:
            label = line[0][:index]
            labels.append((label, instr_count))

    for x,line in enumerate(lines):
        print(instr_count, line)
        instr = next((i for i in isa if i.name == line[0]), None)
        if instr == None:
            print(f"error on line{x}?:{line}")
            continue

        instr_count += 1

        arg0_str = line[1]

        if instr.format == RI and instr.name == "JL":
            imm11 = None
            
            try:
                imm11 = int(arg0_str, 0)
            except ValueError:
                label = next((l for l in labels if l[0] == arg0_str), None)
                imm11 = label[1]

            b = f"{instr.op_in:04b}1{imm11:011b}"
            code.append(b)
            continue

        assert(arg0_str[0] == 'r' and len(arg0_str) == 2)
        arg0 = int(arg0_str[1])
        assert(arg0 < 8)

        if instr.format == RI:
            imm8 = None

            try:
                imm8 = int(line[2], 0)
            except ValueError:
                if instr.name[0] == 'J':
                    label = next((l for l in labels if l[0] == line[2]), None)
                    if imm8 == None:
                        imm8 = label[1]
                
            b = f"{instr.op_in:04b}1{arg0:03b}{imm8:08b}"
            code.append(b)
            continue

        arg1_str = line[2]
        assert(arg1_str[0] == 'r')
        arg1 = int(arg1_str[1])
        assert(arg1 < 8)

        arg2 = None
        if instr.format == DATA:
            arg2 = 0
        else:
            arg2_str = line[3]
            if instr.format == RRR:
                assert(arg2_str[0] == 'r' and len(arg2_str) == 2)
                arg2_str = arg2_str[1]
            if instr.format == RRI:
                assert(len(arg2_str) == 1)
            arg2 = int(arg2_str[0])

        assert(arg2 < 8)

        b = f"{instr.op_in:04b}0{arg0:03b}{instr.format:02b}{arg1:03b}{arg2:03b}"

        code.append(b)
    
    print("\n".join(code))
    with open("code.bin", "wb") as outfile:
        for line in code:
            if len(line) != 16 or any(c not in "01" for c in line):
                continue  # skip invalid lines
            value = int(line, 2)
            outfile.write(value.to_bytes(2, byteorder="big"))  # or "little" if needed


if __name__ == "__main__":
    run_main()
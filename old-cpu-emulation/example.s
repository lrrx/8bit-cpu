
ADD a, b
IMR a, 123
IMR a, 0x7b
JMP -75 ; encode by adding 128, execute by removing 128 again
JMP @label ; resolve before encoding


@label:


; add db statement, add times / fill statement for assembler

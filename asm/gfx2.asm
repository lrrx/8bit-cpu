JL init
key_a:
    SUBI r3 r3 4
    JL loop_draw
key_s:
    ADDI r4 r4 4
    JL loop_draw
key_d:
    ADDI r3 r3 4
    JL loop_draw
key_w:
    SUBI r4 r4 4
    JL loop_draw

init:
    LI r3 0b10 --load a constant to R1
    LI r4 0b01 --load a constant to R2
    LI r5 0
loop_draw:
    OUT r4 1
    OUT r3 0
loop_key:
    IN r7 6
    ADDI r7 r7 0
    JZ pc loop_draw
    ANDI r7 r7 6
    ADDI pc r7 1
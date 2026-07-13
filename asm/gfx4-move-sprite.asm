JL init
key_a:
    SUBI r3 r3 4
    JL loop_draw_start
key_s:
    ADDI r4 r4 4
    JL loop_draw_start
key_d:
    ADDI r3 r3 4
    JL loop_draw_start
key_w:
    SUBI r4 r4 4
    JL loop_draw_start

init:
    LI r3 0b10 --load a constant to R1
    LI r4 0b01 --load a constant to R2
    LI r2 0 --RAM array pointer

    LI r5 0b01111110
    ST r2 r5
    ADDI r2 r2 1

    LI r5 0b10000001
    ST r2 r5
    ADDI r2 r2 1

    LI r5 0b10100101
    ST r2 r5
    ADDI r2 r2 1

    LI r5 0b10000001
    ST r2 r5
    ADDI r2 r2 1

    LI r5 0b01111110
    ST r2 r5
    ADDI r2 r2 1

    LI r5 0b00011000
    ST r2 r5
    ADDI r2 r2 1

    LI r5 0b01111110
    ST r2 r5
    ADDI r2 r2 1

    LI r5 0b00100100
    ST r2 r5
    ADDI r2 r2 1

loop_draw_start:
    LI r2 255
loop_draw_y:
    ADDI r2 r2 1
    LD r5 r2
    LI r6 8
loop_draw_x:
    ADDI r6 r6 0
    JZ pc loop_draw_y
    SUBI r6 r6 1
    SHLI r5 r5 1
    JNC pc loop_draw_x
    SHLI r3 r6 2
    ORI r3 r3 3
    SHLI r4 r2 2
    ORI r3 r3 1
    OUT r4 1
    OUT r3 0
    SHRI r3 r2 3
    --JL loop_key
    JZ pc loop_draw_x
loop_key:
    IN r7 7
    ADDI r7 r7 0
    JZ pc loop_draw_start
    ANDI r7 r7 6
    ADDI pc r7 1
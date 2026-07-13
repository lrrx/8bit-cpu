JL loop_draw_start
key_a:
    LI r3 250
    JL loop_draw_start
key_s:
    LI r4 5
    JL loop_draw_start
key_d:
    LI r3 5
    JL loop_draw_start
key_w:
    LI r4 250
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

    JL loop_key

loop_draw_start:
    --update x pos
    LI r6 10
    LD r5 r6
    ADD r5 r5 r3
    ST r6 r5

    --update y pos
    LI r6 11
    LD r5 r6
    ADD r5 r5 r4
    ST r6 r5

    --clear screen
    LI r2 1
    OUT r2 2
    LI r2 0
    OUT r2 2

    --init sprite ptr
    LI r2 255
loop_draw_y:
    ADDI r2 r2 1
    SHRI r3 r2 3
    JNZ pc loop_key
    LD r5 r2
    LI r6 8
loop_draw_x:
    ADDI r6 r6 0
    JZ pc loop_draw_y
    SUBI r6 r6 1
    SHLI r5 r5 1
    JNC pc loop_draw_x
    
    LI r7 10
    LD r7 r7
    ADD r3 r6 r7
    --ADDI r3 r6 2
    SHLI r3 r3 2
    ORI r3 r3 3

    LI r7 11
    LD r7 r7
    ADD r4 r2 r7
    SHLI r4 r4 2
    ORI r3 r3 1

    OUT r4 1
    OUT r3 0
    JL loop_draw_x
loop_key:
    LI r3 0
    LI r4 0
    IN r7 6
    ADDI r7 r7 0
    JZ pc loop_key
    ANDI r7 r7 6
    ADDI pc r7 1
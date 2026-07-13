Init:
    LI      r1      0b10     #load a constant to R1
    LI      r2      0x01     #load a constant to R2
Loop:
    OUT     r2      1
    OUT     r1      0
    ADDI    r1      r1      4
    JNC     r0      Loop
    ADDI    r2      r2      4
    JL      Loop
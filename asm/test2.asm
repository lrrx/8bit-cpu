LI      r1      0x4     #load a constant to R1
LI      r2      0
SUBI    r1  r1  1
JNZ     r0      1       #loop to start if not yet 0
LI      r7      0xff
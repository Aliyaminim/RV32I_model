.global start

start:
    andi x13,x18,-2036
    ori x27,x31,1798
    lui x25,0x1c5
    addi x25,x25,1039
    lh x27,-275(x25)
    lui x10,0x10b
    addi x10,x10,-29
    lhu x16,-1867(x10)
    addi x6,x12,-1431
    ebreak

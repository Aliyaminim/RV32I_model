.global start

start:
    add x10,x29,x26
    lui x13,0x17b
    addi x13,x13,1113
    lh x16,1019(x13)
    lui x28,0x100
    addi x28,x28,-172
    sw x26,1596(x28)
    lui x27,0x1d8
    addi x27,x27,-1986
    sw x21,1522(x27)
    srl x19,x1,x18
    ebreak


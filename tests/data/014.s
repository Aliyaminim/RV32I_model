.global start

start:
    lui x14,0x1a4
    addi x14,x14,1033
    lh x9,1419(x14)
    lui x16,0x10e
    addi x16,x16,-266
    lw x12,1202(x16)
    addi x18,x22,1789
    lui x13,0x197
    addi x13,x13,-754
    lb x28,1842(x13)
    lui x15,0x16d
    addi x15,x15,1455
    lbu x13,-1943(x15)
    ebreak


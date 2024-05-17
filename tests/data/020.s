.global start

start:
    sub x7,x7,x17
    addi x31,x14,-575
    lui x31,0x1a2
    addi x31,x31,874
    sh x12,-838(x31)
    addi x13,x31,819
    andi x11,x25,1381
    ebreak


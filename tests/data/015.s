.global start

start:
    xori x26,x27,1449
    xori x18,x7,-1885
    ori x7,x24,29
    andi x30,x27,-384
    lui x22,0x18d
    addi x22,x22,40
    sw x11,1104(x22)
    ebreak


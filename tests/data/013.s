.global start

start:
    andi x31,x22,-470
    lui x25,0x1db
    addi x25,x25,1432
    lbu x24,-252(x25)
    lui x6,0x190
    addi x6,x6,450
    lhu x26,1002(x6)
    andi x24,x19,1014
    lui x26,0x1e8
    addi x26,x26,2009
    lbu x31,-305(x26)
    ebreak


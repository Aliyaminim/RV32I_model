.global start

start:
    ori x1,x28,1812
    bne x19,x30,4
    jal x0,8

    .LBB0_1:
    bne x15,x9,12
    jal x0,10

    .LBB0_2:
    bltu x18,x15,14
    jal x0,18

    .LBB0_3:
    andi x28,x26,-1408

    ebreak

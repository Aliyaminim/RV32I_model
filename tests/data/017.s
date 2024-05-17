.global start

start:
    lui x23,0x1c5
    addi x23,x23,-846
    sw x10,1646(x23)
    ori x26,x14,-1583
    andi x27,x26,-1163
    xori x29,x20,-579
    xori x12,x25,-886
    ebreak



.global _start

_start: srl	x30,x0,x17
        add	x4,x23,x10
        sra	x11,x15,x5
        sra	x15,x20,x14
        sll	x26,x4,x26
        ebreak

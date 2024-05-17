.global _start

_start: add	x5,x4,x22
        and	x20,x0,x19
        sltu x25,x21,x23
        lui	x22,0x3e56d
        add	x26,x20,x16
        ebreak

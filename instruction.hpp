#pragma once

#include <cstdio>

namespace rv32i_model {

class inst_t final {
    int opcode;
    int rd;
    int rs1;
    int rs2;
    int funct3;
    int funct7;

    friend class Processor;
public:
    inst_t(int op, int rd_val, int rs1_val, int rs2_val, int func3_val, int func7_val)
        : opcode(op), rd(rd_val), rs1(rs1_val), rs2(rs2_val), funct3(func3_val), funct7(func7_val) {}
};

}
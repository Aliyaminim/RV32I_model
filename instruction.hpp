#pragma once

#include <cstdio>

namespace rv32i_model {

class inst_d final {
    unsigned int opc;
    unsigned int rd;
    unsigned int rs1;
    unsigned int rs2;
    unsigned int funct3;
    unsigned int funct7;

    friend class Processor;
public:
    inst_d(unsigned int op, unsigned int rd_val, unsigned int rs1_val, unsigned int rs2_val, unsigned int func3_val, unsigned int func7_val)
        : opc(op), rd(rd_val), rs1(rs1_val), rs2(rs2_val), funct3(func3_val), funct7(func7_val) {}

    void dump() {
        std::cout << std::hex << opc << std::endl;
        std::cout << std::hex << rd << std::endl;
        std::cout << std::hex << rs1 << std::endl;
        std::cout << std::hex << rs2 << std::endl;
        std::cout << std::hex << funct3 << std::endl;
        std::cout << std::hex << funct7 << std::endl;
    }

};

}
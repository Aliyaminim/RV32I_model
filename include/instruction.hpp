#pragma once

#include <cstdio>

namespace rv32i_model {

class inst_d final {
    uint32_t opc;
    uint32_t rd;
    uint32_t rs1;
    uint32_t rs2;
    uint32_t funct3;
    uint32_t funct7;

    friend class Processor;
public:
    inst_d(uint32_t op, uint32_t rd_val, uint32_t rs1_val, uint32_t rs2_val, uint32_t func3_val, uint32_t func7_val)
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
#pragma once

#include <cstdio>

namespace rv32i_model {

class inst_d final {
    int opc;
    int rd;
    int rs1;
    int rs2;
    int funct3;
    int funct7;

    friend class Executer;
public:
    inst_d(int op, int rd_val, int rs1_val, int rs2_val, int func3_val, int func7_val)
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
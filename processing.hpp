#pragma once

#include <unordered_map>
#include "instruction.hpp"

namespace rv32i_model {

enum class Opcode : unsigned int {
    OP_IMM = 0x13,
    OP = 0x33,
    JAL = 0x6F,
    JALR = 0x67,
    BRANCH = 0x63,
    LOAD = 0x3,
    STORE = 0x23,
    SYSTEM = 0x73,
    FENCE = 0xF
};

enum class Funct3 : unsigned int {
    VAR_1 = 0x0,
    VAR_2 = 0x1,
    VAR_3 = 0x2,
    VAR_4 = 0x3,
    VAR_5 = 0x4,
    VAR_6 = 0x5,
    VAR_7 = 0x6,
    VAR_8 = 0x7
};

enum class Funct7 : unsigned int {
    VAR_1 = 0x0,
    VAR_2 = 0x20
};

typedef void (*process_dinst)(unsigned int funct7, unsigned int rs2, \
                            unsigned int rs1, unsigned int funct3, unsigned int rd);

class Executer final {
    static std::unordered_map<Opcode,process_dinst> ProcessMap;
public:
    Executer() {}

    void execute(inst_d& dec_inst) {
        auto[opc, rd, rs1, rs2, funct3, funct7] = dec_inst;
        ProcessMap[static_cast<Opcode>(opc)](funct7, rs2, rs1, funct3, rd);
    }

};

void process_op_imm(unsigned int funct7, unsigned int rs2, \
                            unsigned int rs1, unsigned int funct3, unsigned int rd) {
    switch (static_cast<Funct3>(funct3)) {
        case Funct3::VAR_1:
            //addi
            break;
    }
}


void process_op(unsigned int funct7, unsigned int rs2, \
                            unsigned int rs1, unsigned int funct3, unsigned int rd) {
    switch (static_cast<Funct3>(funct3)) {
        case Funct3::VAR_1:
            //add or sub
            break;
    }
}

void process_jal(unsigned int funct7, unsigned int rs2, \
                            unsigned int rs1, unsigned int funct3, unsigned int rd) {

}

void process_jalr(unsigned int funct7, unsigned int rs2, \
                            unsigned int rs1, unsigned int funct3, unsigned int rd) {

}


void process_branch(unsigned int funct7, unsigned int rs2, \
                            unsigned int rs1, unsigned int funct3, unsigned int rd) {

}


void process_load(unsigned int funct7, unsigned int rs2, \
                            unsigned int rs1, unsigned int funct3, unsigned int rd) {

}


void process_store(unsigned int funct7, unsigned int rs2, \
                            unsigned int rs1, unsigned int funct3, unsigned int rd) {

}

void process_system(unsigned int funct7, unsigned int rs2, \
                            unsigned int rs1, unsigned int funct3, unsigned int rd) {

}

void process_fence(unsigned int funct7, unsigned int rs2, \
                            unsigned int rs1, unsigned int funct3, unsigned int rd) {

}

std::unordered_map<Opcode,process_dinst> Executer::ProcessMap = {
    {Opcode::OP_IMM, &process_op_imm},
    {Opcode::OP, &process_op},
    {Opcode::JAL, &process_jal},
    {Opcode::JALR, &process_jalr},
    {Opcode::BRANCH, &process_branch},
    {Opcode::LOAD, &process_load},
    {Opcode::STORE, &process_store},
    {Opcode::SYSTEM, &process_system},
    {Opcode::FENCE, &process_fence}
};

}
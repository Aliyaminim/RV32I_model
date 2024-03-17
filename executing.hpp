#pragma once

#include "proc.hpp"
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

void Processor::execute(inst_d dec_inst) {
    auto[opc, rd, rs1, rs2, funct3, funct7] = dec_inst;
    switch (static_cast<Opcode>(opc)) {
        case Opcode::OP_IMM:
            break;
        case Opcode::OP:
            execute_op(funct7, rs2, rs1, funct3, rd);
            break;
        case Opcode::JAL:
            break;
        default:
            break;
    }
}

void Processor::execute_op_imm(unsigned int funct7, unsigned int rs2, \
                            unsigned int rs1, unsigned int funct3, unsigned int rd) {
    switch (static_cast<Funct3>(funct3)) {
        case Funct3::VAR_1:
            //addi
            break;
    }
}


void Processor::execute_op(unsigned int funct7, unsigned int rs2, \
                            unsigned int rs1, unsigned int funct3, unsigned int rd) {
    switch (static_cast<Funct3>(funct3)) {
        case Funct3::VAR_1:
            switch (static_cast<Funct7>(funct7)) {
                case Funct7::VAR_1: //add
                    regfile.write(rd, regfile.read(rs1) + regfile.read(rs2));
                    break;
                case Funct7::VAR_2: //sub
                    regfile.write(rd, regfile.read(rs1) - regfile.read(rs2));
                    break;
                default:
                    //throw;
                    break;
            }
            break;
        default:
            //throw;
            break;
    }
}

void Processor::execute_jal(unsigned int funct7, unsigned int rs2, \
                            unsigned int rs1, unsigned int funct3, unsigned int rd) {

}

void Processor::execute_jalr(unsigned int funct7, unsigned int rs2, \
                            unsigned int rs1, unsigned int funct3, unsigned int rd) {

}


void Processor::execute_branch(unsigned int funct7, unsigned int rs2, \
                            unsigned int rs1, unsigned int funct3, unsigned int rd) {

}


void Processor::execute_load(unsigned int funct7, unsigned int rs2, \
                            unsigned int rs1, unsigned int funct3, unsigned int rd) {

}


void Processor::execute_store(unsigned int funct7, unsigned int rs2, \
                            unsigned int rs1, unsigned int funct3, unsigned int rd) {

}

void Processor::execute_system(unsigned int funct7, unsigned int rs2, \
                            unsigned int rs1, unsigned int funct3, unsigned int rd) {

}

void Processor::execute_fence(unsigned int funct7, unsigned int rs2, \
                            unsigned int rs1, unsigned int funct3, unsigned int rd) {

}

}
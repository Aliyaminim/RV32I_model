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
            execute_op_imm(funct7, rs2, rs1, funct3, rd);
            break;
        case Opcode::OP:
            execute_op(funct7, rs2, rs1, funct3, rd);
            break;
        case Opcode::JAL:
            execute_jal(funct7, rs2, rs1, funct3, rd);
            break;
        case Opcode::JALR:
            execute_jalr(funct7, rs2, rs1, funct3, rd);
            break;
        case Opcode::BRANCH:
            execute_branch(funct7, rs2, rs1, funct3, rd);
            break;
        case Opcode::LOAD:
            execute_load(funct7, rs2, rs1, funct3, rd);
            break;
        case Opcode::STORE:
            execute_store(funct7, rs2, rs1, funct3, rd);
            break;
        case Opcode::SYSTEM:
            execute_system(funct7, rs2, rs1, funct3, rd);
            break;
        case Opcode::FENCE:
            execute_fence(funct7, rs2, rs1, funct3, rd);
            break;
        default:
            break;
    }
}

void Processor::execute_op_imm(unsigned int funct7, unsigned int rs2, \
                            unsigned int rs1, unsigned int funct3, unsigned int rd) {
    unsigned int imm;
    switch (static_cast<Funct3>(funct3)) {
        case Funct3::VAR_1: //addi
            std::cout << "ADDI" << std::endl;
            imm = (funct7 << 5) + rs2;
            regfile.write(rd, imm + regfile.read(rs1));
            break;
        default:
            throw std::runtime_error ("Unknown instruction");
            break;
    }
}


void Processor::execute_op(unsigned int funct7, unsigned int rs2, \
                            unsigned int rs1, unsigned int funct3, unsigned int rd) {
    switch (static_cast<Funct3>(funct3)) {
        case Funct3::VAR_1:
            switch (static_cast<Funct7>(funct7)) {
                case Funct7::VAR_1: //add
                    std::cout << "ADD" << std::endl;
                    regfile.write(rd, regfile.read(rs1) + regfile.read(rs2));
                    break;
                case Funct7::VAR_2: //sub
                    std::cout << "SUB" << std::endl;
                    regfile.write(rd, regfile.read(rs1) - regfile.read(rs2));
                    break;
                default:
                    throw std::runtime_error ("Unknown instruction");
                    break;
            }
            break;
        default:
            throw std::runtime_error ("Unknown instruction");
            break;
    }
}

void Processor::execute_jal(unsigned int funct7, unsigned int rs2, \
                            unsigned int rs1, unsigned int funct3, unsigned int rd) {
    std::cout << "JAL" << std::endl;
    //regfile.write(rd, pc + 1);

}

void Processor::execute_jalr(unsigned int funct7, unsigned int rs2, \
                            unsigned int rs1, unsigned int funct3, unsigned int rd) {
    std::cout << "JALR" << std::endl;
    int32_t imm = ((int)(memory[PC-1] & 0xFFF00000)) >> 20;
    regfile.write(rd, PC + 1);
    int32_t address = regfile.read(rs1) + imm;
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
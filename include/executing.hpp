#pragma once

#include "processor.hpp"
#include "instruction.hpp"

namespace rv32i_model {

enum class Opcode : uint32_t {
    OP_IMM = 0x13,
    LUI = 0x37,
    AUIPC = 17,
    OP = 0x33,
    JAL = 0x6F,
    JALR = 0x67,
    BRANCH = 0x63,
    LOAD = 0x3,
    STORE = 0x23,
    SYSTEM = 0x73,
    FENCE = 0xF
};

enum class Funct3 : uint32_t {
    VAR_0 = 0x0,
    VAR_1 = 0x1,
    VAR_2 = 0x2,
    VAR_3 = 0x3,
    VAR_4 = 0x4,
    VAR_5 = 0x5,
    VAR_6 = 0x6,
    VAR_7 = 0x7
};

enum class Funct7 : uint32_t {
    VAR_0 = 0x0,
    VAR_1 = 0x20
};

void Processor::execute(inst_d dec_inst) {
    auto[opc, rd, rs1, rs2, funct3, funct7] = dec_inst;
    switch (static_cast<Opcode>(opc)) {
        case Opcode::OP_IMM:
            execute_op_imm(funct7, rs2, rs1, funct3, rd);
            break;
        case Opcode::LUI:
            //execute_lui(dec_inst);
        case Opcode::AUIPC:
            //execute_auipc(dec_inst);
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

void Processor::execute_op_imm(uint32_t funct7, uint32_t rs2, \
                            uint32_t rs1, uint32_t funct3, uint32_t rd) {
    uint32_t imm_tmp = 0u;
    imm_tmp += (funct7 << 25) + (rs2 << 20);
    int32_t imm = (int32_t) imm_tmp;
    imm = imm >> 20;

    switch (static_cast<Funct3>(funct3)) {
        case Funct3::VAR_0: //addi
            std::cout << "ADDI" << std::endl;
            regfile.write(rd, imm + regfile.read(rs1));
            break;
        case Funct3::VAR_2: //slti
            std::cout << "SLTI" << std::endl;
            regfile.write(rd, ((int32_t)regfile.read(rs1) < (int32_t)imm));
        case Funct3::VAR_3: //sltiu
            std::cout << "SLTIU" << std::endl;
            regfile.write(rd, ((uint32_t)regfile.read(rs1) < (uint32_t)imm));
        default:
            throw std::runtime_error ("Unknown instruction");
            break;
    }
}


void Processor::execute_op(uint32_t funct7, uint32_t rs2, \
                            uint32_t rs1, uint32_t funct3, uint32_t rd) {
    switch (static_cast<Funct3>(funct3)) {
        case Funct3::VAR_1:
            switch (static_cast<Funct7>(funct7)) {
                case Funct7::VAR_0: //add
                    std::cout << "ADD" << std::endl;
                    regfile.write(rd, regfile.read(rs1) + regfile.read(rs2));
                    break;
                case Funct7::VAR_1: //sub
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

void Processor::execute_jal(uint32_t funct7, uint32_t rs2, \
                            uint32_t rs1, uint32_t funct3, uint32_t rd) {
    std::cout << "JAL" << std::endl;

}

void Processor::execute_jalr(uint32_t funct7, uint32_t rs2, \
                            uint32_t rs1, uint32_t funct3, uint32_t rd) {
    std::cout << "JALR" << std::endl;
    #ifdef DEBUG
    int32_t imm = ((int)(memory[PC-1] & 0xFFF00000)) >> 20;
    regfile.write(rd, PC + 1);
    int32_t address = regfile.read(rs1) + imm;
    #endif
}


void Processor::execute_branch(uint32_t funct7, uint32_t rs2, \
                            uint32_t rs1, uint32_t funct3, uint32_t rd) {

}


void Processor::execute_load(uint32_t funct7, uint32_t rs2, \
                            uint32_t rs1, uint32_t funct3, uint32_t rd) {

}


void Processor::execute_store(uint32_t funct7, uint32_t rs2, \
                            uint32_t rs1, uint32_t funct3, uint32_t rd) {

}

void Processor::execute_system(uint32_t funct7, uint32_t rs2, \
                            uint32_t rs1, uint32_t funct3, uint32_t rd) {

}

void Processor::execute_fence(uint32_t funct7, uint32_t rs2, \
                            uint32_t rs1, uint32_t funct3, uint32_t rd) {

}

}
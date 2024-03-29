#pragma once

#include <memory>
#include "processor.hpp"
#include "instruction.hpp"

namespace rv32i_model {

enum class Opcode : uint32_t {
    OP_IMM = 0x13,
    LUI = 0x37,
    AUIPC = 0x17,
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

void Processor::execute(uint32_t& inst) {
    uint32_t opcode = inst & 0x7F;
    std::unique_ptr<instD> dec_inst;
    //здесь проверили на наличие в instruction cache

    switch (static_cast<Opcode>(opcode)) {
        case Opcode::OP_IMM:
            dec_inst = std::make_unique<instD_I>(inst);
            execute_op_imm(dec_inst.get());
            break;
        case Opcode::LUI:
            dec_inst = std::make_unique<instD_U>(inst);
            execute_lui(dec_inst.get());
            break;
        case Opcode::AUIPC:
            dec_inst = std::make_unique<instD_U>(inst);
            execute_auipc(dec_inst.get());
            break;
        case Opcode::OP:
            dec_inst = std::make_unique<instD_R>(inst);
            execute_op(dec_inst.get());
            break;
        case Opcode::JAL:
            dec_inst = std::make_unique<instD_J>(inst);
            execute_jal(dec_inst.get());
            break;
        case Opcode::JALR:
            dec_inst = std::make_unique<instD_I>(inst);
            execute_jalr(dec_inst.get());
            break;
        case Opcode::BRANCH:
            dec_inst = std::make_unique<instD_B>(inst);
            execute_branch(dec_inst.get());
            break;
        case Opcode::LOAD:
            //execute_load(funct7, rs2, rs1, funct3, rd);
            break;
        case Opcode::STORE:
            //execute_store(funct7, rs2, rs1, funct3, rd);
            break;
        case Opcode::SYSTEM:
            //execute_system(funct7, rs2, rs1, funct3, rd);
            break;
        case Opcode::FENCE:
            //execute_fence(funct7, rs2, rs1, funct3, rd);
            break;
        default:
            break;
    }
}

void Processor::execute_op_imm(instD* dec_inst) {
    auto[opc, rd, rs1, funct3, imm] = *dynamic_cast<instD_I*>(dec_inst);

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

void Processor::execute_lui(instD* dec_inst) {
    auto[opc, rd, imm] = *dynamic_cast<instD_U*>(dec_inst);
    regfile.write(rd, imm);
}

void Processor::execute_auipc(instD* dec_inst) {
    auto[opc, rd, imm] = *dynamic_cast<instD_U*>(dec_inst);
    imm += (int32_t)PC;
    regfile.write(rd, imm);
}

void Processor::execute_op(instD* dec_inst) {
    auto[opc, rd, rs1, rs2, funct3, funct7] = *dynamic_cast<instD_R*>(dec_inst);

    switch (static_cast<Funct3>(funct3)) {
        case Funct3::VAR_0:
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

void Processor::execute_jal(instD* dec_inst) {
    std::cout << "JAL" << std::endl;
    auto[opc, rd, imm] = *dynamic_cast<instD_J*>(dec_inst);
    regfile.write(rd, PC + 1);
    PC += imm;
}

void Processor::execute_jalr(instD* dec_inst) {
    std::cout << "JALR" << std::endl;
    auto[opc, rd, rs1, funct3, imm] = *dynamic_cast<instD_I*>(dec_inst);

    int32_t new_address = regfile.read(rs1) + imm;
    regfile.write(rd, PC + 1);
    PC = new_address;
}


void Processor::execute_branch(instD* dec_inst) {
    std::cout << "BRANCH" << std::endl;
    auto[opc, rs1, rs2, funct3, imm] = *dynamic_cast<instD_B*>(dec_inst);
    switch(static_cast<Funct3>(funct3)) {
        case Funct3::VAR_0:
            if (regfile.read(rs1) == regfile.read(rs2))
                PC += imm;
            break;
        case Funct3::VAR_1:
            if (regfile.read(rs1) != regfile.read(rs2))
                PC += imm;
            break;
        case Funct3::VAR_4:
            if (regfile.read(rs1) < regfile.read(rs2))
                PC += imm;
            break;
        case Funct3::VAR_5:
            if (regfile.read(rs1) >= regfile.read(rs2))
                PC += imm;
            break;
        case Funct3::VAR_6:
            if ((uint32_t)regfile.read(rs1) < (uint32_t)regfile.read(rs2))
                PC += imm;
            break;
        case Funct3::VAR_7:
            if ((uint32_t)regfile.read(rs1) >= (uint32_t)regfile.read(rs2))
                PC += imm;
            break;
    }

}


void Processor::execute_load(instD* dec_inst) {

}


void Processor::execute_store(instD* dec_inst) {

}

void Processor::execute_system(instD* dec_inst) {

}

void Processor::execute_fence(instD* dec_inst) {

}

}
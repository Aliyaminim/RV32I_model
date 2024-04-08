#pragma once

#include <memory>
#include <unistd.h>
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

//returns 1 if jump or branch occurred, otherwise 0
int Processor::execute(uint32_t& inst) {
    uint32_t opcode = inst & 0x7F;
    std::unique_ptr<instD> dec_inst;
    int ret_jb = 0;

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
            ret_jb = 1;
            break;
        case Opcode::JALR:
            dec_inst = std::make_unique<instD_I>(inst);
            execute_jalr(dec_inst.get());
            ret_jb = 1;
            break;
        case Opcode::BRANCH:
            dec_inst = std::make_unique<instD_B>(inst);
            ret_jb = execute_branch(dec_inst.get());
            break;
        case Opcode::LOAD:
            dec_inst = std::make_unique<instD_I>(inst);
            execute_load(dec_inst.get());
            break;
        case Opcode::STORE:
            dec_inst = std::make_unique<instD_S>(inst);
            execute_store(dec_inst.get());
            break;
        case Opcode::SYSTEM:
            dec_inst = std::make_unique<instD_I>(inst);
            execute_system(dec_inst.get());
            break;
        case Opcode::FENCE:
            dec_inst = std::make_unique<instD_I>(inst);
            execute_fence(dec_inst.get());
            break;
        default:
            throw std::runtime_error ("Unknown instruction");
            break;
    }

    return ret_jb;
}

void Processor::execute_op_imm(instD* dec_inst) {
    auto[opc, rd, rs1, funct3, imm] = *dynamic_cast<instD_I*>(dec_inst);

    switch (static_cast<Funct3>(funct3)) {
        case Funct3::VAR_0: //addi or nop
            std::cout << "ADDI" << std::endl;
            if ((rs1 == 0) && (rd == 0) && (imm == 0))
                increase_PC(); //nop
            else
                regfile.write(rd, imm + regfile.read(rs1));
            break;
        case Funct3::VAR_2: //slti
            std::cout << "SLTI" << std::endl;
            regfile.write(rd, ((int32_t)regfile.read(rs1) < (int32_t)imm));
            break;
        case Funct3::VAR_3: //sltiu
            std::cout << "SLTIU" << std::endl;
            regfile.write(rd, ((uint32_t)regfile.read(rs1) < (uint32_t)imm));
            break;
        case Funct3::VAR_4: //xori
            std::cout << "XORI" << std::endl;
            regfile.write(rd, ((uint32_t)regfile.read(rs1) ^ (uint32_t)imm));
            break;
        case Funct3::VAR_6: //ori
            std::cout << "ORI" << std::endl;
            regfile.write(rd, ((uint32_t)regfile.read(rs1) | (uint32_t)imm));
            break;
        case Funct3::VAR_7: //andi
            std::cout << "ANDI" << std::endl;
            regfile.write(rd, ((uint32_t)regfile.read(rs1) & (uint32_t)imm));
            break;
        default:
            throw std::runtime_error ("Unknown integer register-immediate instruction: funct3 = " + funct3);
            break;
    }
}

void Processor::execute_lui(instD* dec_inst) {
    auto[opc, rd, imm] = *dynamic_cast<instD_U*>(dec_inst);
    regfile.write(rd, imm);
}

void Processor::execute_auipc(instD* dec_inst) {
    auto[opc, rd, imm] = *dynamic_cast<instD_U*>(dec_inst);
    imm += (int32_t)get_PC();
    regfile.write(rd, imm);
}

void Processor::execute_op(instD* dec_inst) {
    auto[opc, rd, rs1, rs2, funct3, funct7] = *dynamic_cast<instD_R*>(dec_inst);

    uint32_t tmp_res = 0;
    switch (static_cast<Funct3>(funct3)) {
        case Funct3::VAR_0:
            switch (static_cast<Funct7>(funct7)) {
                case Funct7::VAR_0: //ADD
                    std::cout << "ADD" << std::endl;
                    regfile.write(rd, regfile.read(rs1) + regfile.read(rs2));
                    break;
                case Funct7::VAR_1: //SUB
                    std::cout << "SUB" << std::endl;
                    regfile.write(rd, regfile.read(rs1) - regfile.read(rs2));
                    break;
                default:
                    throw std::runtime_error ("Unknown instruction");
                    break;
            }
            break;
        case Funct3::VAR_2: //slt
            std::cout << "SLT" << std::endl;
            tmp_res = ((int32_t)regfile.read(rs1) < (int32_t)regfile.read(rs2)) ? 1u : 0;
            regfile.write(rd, tmp_res);
            break;
        case Funct3::VAR_3: //sltu
            std::cout << "SLTU" << std::endl;
            if (rs1 != 0)
                tmp_res = ((uint32_t)regfile.read(rs1) < (uint32_t)regfile.read(rs2)) ? 1u : 0;
            else
                //rs1 == x0
                tmp_res = ((uint32_t)regfile.read(rs2) != (uint32_t)regfile.read(rs1)) ? 1u : 0;
            regfile.write(rd, tmp_res);
            break;
        case Funct3::VAR_7: //add
            std::cout << "AND" << std::endl;
            regfile.write(rd, ((uint32_t)regfile.read(rs1) & (uint32_t)regfile.read(rs2)));
            break;
        case Funct3::VAR_6: //or
            std::cout << "OR" << std::endl;
            regfile.write(rd, ((uint32_t)regfile.read(rs1) | (uint32_t)regfile.read(rs2)));
            break;
        case Funct3::VAR_4: //xor
            std::cout << "XOR" << std::endl;
            regfile.write(rd, ((uint32_t)regfile.read(rs1) ^ (uint32_t)regfile.read(rs2)));
            break;
        case Funct3::VAR_1: //sll
            std::cout << "SLL" << std::endl;
            tmp_res = regfile.read(rs1) << regfile.read(rs2);
            regfile.write(rd, tmp_res);
            break;
        case Funct3::VAR_5:
            switch (static_cast<Funct7>(funct7)) {
                case Funct7::VAR_0: //srl
                    std::cout << "SRL" << std::endl;
                    tmp_res = ((uint32_t)regfile.read(rs1)) >> regfile.read(rs2);
                    regfile.write(rd, tmp_res);
                    break;
                case Funct7::VAR_1: //sra
                    std::cout << "SRA" << std::endl;
                    tmp_res = ((int32_t)regfile.read(rs1)) >> regfile.read(rs2);
                    regfile.write(rd, tmp_res);
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
    std::size_t imm_2index = imm /sizeof(uint32_t);
    regfile.write(rd, get_PC() + 1);

    if ( !(imm & 0x3) ) {
        // The address is 4-byte aligned here
        increase_PC(imm_2index);
    } else
        throw std::runtime_error ("An instruction-address-misaligned exception");
}

void Processor::execute_jalr(instD* dec_inst) {
    std::cout << "JALR" << std::endl;
    auto[opc, rd, rs1, funct3, imm] = *dynamic_cast<instD_I*>(dec_inst);

    int32_t new_address = regfile.read(rs1) + imm;
    std::size_t new_address_2index = new_address /sizeof(uint32_t);

    if (rd != 0) //sometimes dest is not required
        regfile.write(rd, get_PC() + 1);

    if ( !(new_address & 0x3) ) {
        // The address is 4-byte aligned here
        set_PC(new_address_2index);
    } else
        throw std::runtime_error ("An instruction-address-misaligned exception");
}


int Processor::execute_branch(instD* dec_inst) {
    std::cout << "BRANCH" << std::endl;
    auto[opc, rs1, rs2, funct3, imm] = *dynamic_cast<instD_B*>(dec_inst);
    bool branch_flag = false;
    std::size_t imm_2index = imm /sizeof(uint32_t);

    switch(static_cast<Funct3>(funct3)) {
        case Funct3::VAR_0: //beq
            if (regfile.read(rs1) == regfile.read(rs2))
                branch_flag = true;
            break;
        case Funct3::VAR_1: //bne
            if (regfile.read(rs1) != regfile.read(rs2))
                branch_flag = true;
            break;
        case Funct3::VAR_4: //blt
            if (regfile.read(rs1) < regfile.read(rs2))
                branch_flag = true;
            break;
        case Funct3::VAR_5: //bgeu
            if (regfile.read(rs1) >= regfile.read(rs2))
                branch_flag = true;
            break;
        case Funct3::VAR_6: //bltu
            if ((uint32_t)regfile.read(rs1) < (uint32_t)regfile.read(rs2))
                branch_flag = true;
            break;
        case Funct3::VAR_7: //bge
            if ((uint32_t)regfile.read(rs1) >= (uint32_t)regfile.read(rs2))
                branch_flag = true;
            break;
        default:
            throw std::runtime_error ("Unknown instruction");
            break;
    }

    if (branch_flag) {
        if ( !(imm & 0x3) ) {
            // The address is 4-byte aligned here
            increase_PC(imm_2index);
        } else
            throw std::runtime_error ("An instruction-address-misaligned exception");
    }

    return branch_flag;
}


void Processor::execute_load(instD* dec_inst) {
    std::cout << "LOAD" << std::endl;
    auto[opc, rd, rs1, funct3, imm] = *dynamic_cast<instD_I*>(dec_inst);

    int32_t effective_address = regfile.read(rs1) + imm;
    uint32_t value_tmp = memory.read(effective_address); //>+0
    int32_t value = 0;

    switch(static_cast<Funct3>(funct3)) {
        case Funct3::VAR_0: //LB
            value_tmp = (value_tmp & 0xFF) + ((value_tmp & 0x80) ? 0xFFFFFF00 : 0x0);
            value = (int32_t)value_tmp;
            break;
        case Funct3::VAR_1: //LH
            value_tmp = (value_tmp & 0xFFFF) + ((value_tmp & 0x8000) ? 0xFFFF0000 : 0x0);
            value = (int32_t)value_tmp;
            break;
        case Funct3::VAR_2: //LW
            value = (int32_t)value_tmp;
            break;
        case Funct3::VAR_4: //LBU
            value_tmp = (value_tmp & 0xFF);
            value = (int32_t)value_tmp;
            break;
        case Funct3::VAR_5: //LHU
            value_tmp = (value_tmp & 0xFFFF);
            value = (int32_t)value_tmp;
            break;
        default:
            throw std::runtime_error ("Unknown instruction");
            break;
    }
    regfile.write(rd, value);
}


void Processor::execute_store(instD* dec_inst) {
    std::cout << "STORE" << std::endl;
    auto[opc, rs1, rs2, funct3, imm] = *dynamic_cast<instD_S*>(dec_inst);

    int32_t effective_address = regfile.read(rs1) + imm;
    int32_t value = regfile.read(rs2);

    switch(static_cast<Funct3>(funct3)) {
        case Funct3::VAR_0: //SB
            memory.write(effective_address, value & 0xFF);
            break;
        case Funct3::VAR_1: //SH
            memory.write(effective_address, value & 0xFFFF);
            break;
        case Funct3::VAR_2: //SW
            memory.write(effective_address, value);
            break;
        default:
            throw std::runtime_error ("Unknown instruction");
            break;
    }

}

void Processor::execute_system(instD* dec_inst) {
    std::cout << "SYSTEM" << std::endl;
    enum {
        syscall_READ = 63,
        syscall_WRITE = 64,
        syscall_EXIT = 93
    };

    auto syscall_type = regfile.read(17);

    int32_t fd = regfile.read(10);
    int32_t status = fd;
    int32_t buf_model = regfile.read(11);
    int32_t size = regfile.read(12);
    void* buf = static_cast<char*>(memory.begin()) + buf_model;

    switch (syscall_type) {
        case syscall_READ:
            read(fd, buf, size);
            break;
        case syscall_WRITE:
            write(fd, buf, size);
            break;
        case syscall_EXIT:
            exit(status);
            break;
        default:
            throw std::runtime_error ("Unknown system call");
            break;
    }

}

void Processor::execute_fence(instD* dec_inst) {
    std::cout << "FENCE" << std::endl;
    throw std::runtime_error ("Unknown instruction");
}

}
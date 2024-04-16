#pragma once

#include <memory>
#include <unistd.h>
#include <stdexcept>
#include "processor.hpp"
#include "instruction.hpp"
#include "tools.hpp"

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

//returns exit status of syscalls, otherwise 0
int Processor::execute(uint32_t& inst) {
    uint32_t opcode = inst & 0x7F;
    std::unique_ptr<instD> dec_inst;
    int ret_v = 0;

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
            dec_inst = std::make_unique<instD_I>(inst);
            execute_load(dec_inst.get());
            break;
        case Opcode::STORE:
            dec_inst = std::make_unique<instD_S>(inst);
            execute_store(dec_inst.get());
            break;
        case Opcode::SYSTEM:
            dec_inst = std::make_unique<instD_I>(inst);
            ret_v = execute_system(dec_inst.get());
            break;
        case Opcode::FENCE:
            dec_inst = std::make_unique<instD_I>(inst);
            execute_fence(dec_inst.get());
            break;
        default:
            throw std::runtime_error ("Unknown instruction: PC = 0x" + num2hex(PC) + " inst = 0x" + num2hex(inst));
            break;
    }

    return ret_v;
}

void Processor::execute_op_imm(instD* dec_inst) {
    auto[opc, rd, rs1, funct3, imm] = *dynamic_cast<instD_I*>(dec_inst);

    switch (static_cast<Funct3>(funct3)) {
        case Funct3::VAR_0: //addi or nop
            logstream << "ADDI" << std::endl;
            if ((rs1 == 0) && (rd == 0) && (imm == 0))
                increase_PC(); //nop
            else
                regfile.write(rd, imm + regfile.read(rs1));
            break;
        case Funct3::VAR_2: //slti
            logstream << "SLTI" << std::endl;
            regfile.write(rd, ((int32_t)regfile.read(rs1) < (int32_t)imm));
            break;
        case Funct3::VAR_3: //sltiu
            logstream << "SLTIU" << std::endl;
            regfile.write(rd, ((uint32_t)regfile.read(rs1) < (uint32_t)imm));
            break;
        case Funct3::VAR_4: //xori
            logstream << "XORI" << std::endl;
            regfile.write(rd, ((uint32_t)regfile.read(rs1) ^ (uint32_t)imm));
            break;
        case Funct3::VAR_6: //ori
            logstream << "ORI" << std::endl;
            regfile.write(rd, ((uint32_t)regfile.read(rs1) | (uint32_t)imm));
            break;
        case Funct3::VAR_7: //andi
            logstream << "ANDI" << std::endl;
            regfile.write(rd, ((uint32_t)regfile.read(rs1) & (uint32_t)imm));
            break;
        default:
            throw std::runtime_error ("Unknown integer register-immediate instruction: PC = 0x" + num2hex(PC));
            break;
    }
}

void Processor::execute_lui(instD* dec_inst) {
    logstream << "LUI" << std::endl;
    auto[opc, rd, imm] = *dynamic_cast<instD_U*>(dec_inst);
    regfile.write(rd, imm);
}

void Processor::execute_auipc(instD* dec_inst) {
    logstream << "AUIPC" << std::endl;
    auto[opc, rd, imm] = *dynamic_cast<instD_U*>(dec_inst);
    imm += get_PC();
    regfile.write(rd, imm);
}

void Processor::execute_op(instD* dec_inst) {
    auto[opc, rd, rs1, rs2, funct3, funct7] = *dynamic_cast<instD_R*>(dec_inst);

    uint32_t tmp_res = 0;
    switch (static_cast<Funct3>(funct3)) {
        case Funct3::VAR_0:
            switch (static_cast<Funct7>(funct7)) {
                case Funct7::VAR_0: //ADD
                    logstream << "ADD" << std::endl;
                    regfile.write(rd, regfile.read(rs1) + regfile.read(rs2));
                    break;
                case Funct7::VAR_1: //SUB
                    logstream << "SUB" << std::endl;
                    regfile.write(rd, regfile.read(rs1) - regfile.read(rs2));
                    break;
                default:
                    throw std::runtime_error ("Unknown integer register-register instruction: PC = 0x" + num2hex(PC));
                    break;
            }
            break;
        case Funct3::VAR_2: //slt
            logstream << "SLT" << std::endl;
            tmp_res = ((int32_t)regfile.read(rs1) < (int32_t)regfile.read(rs2)) ? 1u : 0;
            regfile.write(rd, tmp_res);
            break;
        case Funct3::VAR_3: //sltu
            logstream << "SLTU" << std::endl;
            if (rs1 != 0)
                tmp_res = ((uint32_t)regfile.read(rs1) < (uint32_t)regfile.read(rs2)) ? 1u : 0;
            else
                //rs1 == x0
                tmp_res = ((uint32_t)regfile.read(rs2) != (uint32_t)regfile.read(rs1)) ? 1u : 0;
            regfile.write(rd, tmp_res);
            break;
        case Funct3::VAR_7: //add
            logstream << "AND" << std::endl;
            regfile.write(rd, ((uint32_t)regfile.read(rs1) & (uint32_t)regfile.read(rs2)));
            break;
        case Funct3::VAR_6: //or
            logstream << "OR" << std::endl;
            regfile.write(rd, ((uint32_t)regfile.read(rs1) | (uint32_t)regfile.read(rs2)));
            break;
        case Funct3::VAR_4: //xor
            logstream << "XOR" << std::endl;
            regfile.write(rd, ((uint32_t)regfile.read(rs1) ^ (uint32_t)regfile.read(rs2)));
            break;
        case Funct3::VAR_1: //sll
            logstream << "SLL" << std::endl;
            tmp_res = regfile.read(rs1) << regfile.read(rs2);
            regfile.write(rd, tmp_res);
            break;
        case Funct3::VAR_5:
            switch (static_cast<Funct7>(funct7)) {
                case Funct7::VAR_0: //srl
                    logstream << "SRL" << std::endl;
                    tmp_res = ((uint32_t)regfile.read(rs1)) >> regfile.read(rs2);
                    regfile.write(rd, tmp_res);
                    break;
                case Funct7::VAR_1: //sra
                    logstream << "SRA" << std::endl;
                    tmp_res = ((int32_t)regfile.read(rs1)) >> regfile.read(rs2);
                    regfile.write(rd, tmp_res);
                    break;
            }
            break;
        default:
            throw std::runtime_error ("Unknown integer register-register instruction: PC = 0x" + num2hex(PC));
            break;
    }
}

void Processor::execute_jal(instD* dec_inst) {
    logstream << "JAL" << std::endl;
    auto[opc, rd, imm] = *dynamic_cast<instD_J*>(dec_inst);

    if (rd != 0) //sometimes dest is not required
        regfile.write(rd, get_PC() + 4);

    if ( !(imm & 0x3) ) {
        // The address is 4-byte aligned here
        increase_PC(imm);
        jump_flag = true;
    } else
        throw std::runtime_error ("An instruction-address-misaligned exception");
}

void Processor::execute_jalr(instD* dec_inst) {
    logstream << "JALR" << std::endl;
    auto[opc, rd, rs1, funct3, imm] = *dynamic_cast<instD_I*>(dec_inst);

    uint32_t new_address = regfile.read(rs1) + imm;

    if (rd != 0) //sometimes dest is not required
        regfile.write(rd, get_PC() + 4);

    if ( !(new_address & 0x3) ) {
        // The address is 4-byte aligned here
        set_PC(new_address);
        jump_flag = true;
    } else
        throw std::runtime_error ("An instruction-address-misaligned exception");
}


void Processor::execute_branch(instD* dec_inst) {
    auto[opc, rs1, rs2, funct3, imm] = *dynamic_cast<instD_B*>(dec_inst);
    bool jump_flag = false;

    switch(static_cast<Funct3>(funct3)) {
        case Funct3::VAR_0: //beq
            logstream << "BEQ" << std::endl;
            if (regfile.read(rs1) == regfile.read(rs2))
                jump_flag = true;
            break;
        case Funct3::VAR_1: //bne
            logstream << "BNE" << std::endl;
            if (regfile.read(rs1) != regfile.read(rs2))
                jump_flag = true;
            break;
        case Funct3::VAR_4: //blt
            logstream << "BLT" << std::endl;
            if (regfile.read(rs1) < regfile.read(rs2))
                jump_flag = true;
            break;
        case Funct3::VAR_5: //bgeu
            logstream << "BGEU" << std::endl;
            if (regfile.read(rs1) >= regfile.read(rs2))
                jump_flag = true;
            break;
        case Funct3::VAR_6: //bltu
            logstream << "BLTU" << std::endl;
            if ((uint32_t)regfile.read(rs1) < (uint32_t)regfile.read(rs2))
                jump_flag = true;
            break;
        case Funct3::VAR_7: //bge
            logstream << "BGE" << std::endl;
            if ((uint32_t)regfile.read(rs1) >= (uint32_t)regfile.read(rs2))
                jump_flag = true;
            break;
        default:
            throw std::runtime_error ("Unknown branch instruction: PC = 0x" + num2hex(PC));
            break;
    }

    if (jump_flag) {
        if ( !(imm & 0x3) ) {
            // The address is 4-byte aligned here
            increase_PC(imm);
        } else
            throw std::runtime_error ("An instruction-address-misaligned exception");
    }
}


void Processor::execute_load(instD* dec_inst) {
    auto[opc, rd, rs1, funct3, imm] = *dynamic_cast<instD_I*>(dec_inst);

    uint32_t effective_address = regfile.read(rs1) + imm;
    uint32_t value_tmp;
    uint8_t value_lb;
    uint16_t value_lh;
    uint32_t value_lw;
    int32_t value = 0;

    switch(static_cast<Funct3>(funct3)) {
        case Funct3::VAR_0: //LB
            logstream << "LB" << std::endl;
            read(effective_address, 1, reinterpret_cast<char*>(&value_lb));
            value_tmp = value_lb;
            value_tmp = (value_tmp & 0xFF) + ((value_tmp & 0x80) ? 0xFFFFFF00 : 0x0);
            value = (int32_t)value_tmp;
            break;
        case Funct3::VAR_1: //LH
            logstream << "LH" << std::endl;
            read(effective_address, 2, reinterpret_cast<char*>(&value_lh));
            value_tmp = value_lh;
            value_tmp = (value_tmp & 0xFFFF) + ((value_tmp & 0x8000) ? 0xFFFF0000 : 0x0);
            value = (int32_t)value_tmp;
            break;
        case Funct3::VAR_2: //LW
            logstream << "LW" << std::endl;
            read(effective_address, 4, reinterpret_cast<char*>(&value_lw));
            value_tmp = value_lw;
            value = (int32_t)value_tmp;
            break;
        case Funct3::VAR_4: //LBU
            logstream << "LBU" << std::endl;
            read(effective_address, 1, reinterpret_cast<char*>(&value_lb));
            value_tmp = value_lb;
            value_tmp = (value_tmp & 0xFF);
            value = (int32_t)value_tmp;
            break;
        case Funct3::VAR_5: //LHU
            logstream << "LHU" << std::endl;
            read(effective_address, 2, reinterpret_cast<char*>(&value_lh));
            value_tmp = value_lh;
            value_tmp = (value_tmp & 0xFFFF);
            value = (int32_t)value_tmp;
            break;
        default:
            throw std::runtime_error ("Unknown load instruction: PC = 0x" + num2hex(PC));
            break;
    }
    regfile.write(rd, value);
}


void Processor::execute_store(instD* dec_inst) {
    auto[opc, rs1, rs2, funct3, imm] = *dynamic_cast<instD_S*>(dec_inst);

    uint32_t effective_address = regfile.read(rs1) + imm;
    uint32_t value = regfile.read(rs2);

    switch(static_cast<Funct3>(funct3)) {
        case Funct3::VAR_0: //SB
            logstream << "SB" << std::endl;
            value = (value & 0xFF);
            write(effective_address, 1, reinterpret_cast<char*>(&value));
            break;
        case Funct3::VAR_1: //SH
            logstream << "SH" << std::endl;
            value = (value & 0xFFFF);
            write(effective_address, 2, reinterpret_cast<char*>(&value));
            break;
        case Funct3::VAR_2: //SW
            logstream << "SW" << std::endl;
            write(effective_address, value);
            break;
        default:
            throw std::runtime_error ("Unknown store instruction: PC = 0x" + num2hex(PC));
            break;
    }

}

int Processor::execute_system(instD* dec_inst) {
    auto[opc, rd, rs1, funct3, imm] = *dynamic_cast<instD_I*>(dec_inst);

    enum {
        syscall_READ = 63,
        syscall_WRITE = 64,
        syscall_EXIT = 93
    };

    if (imm == 0) { //ecall
        auto syscall_type = regfile.read(17);

        logstream << "ECALL " << std::dec << syscall_type << std::endl;

        int32_t fd = regfile.read(10);
        int32_t status = fd;
        int32_t buf_model = regfile.read(11);
        int32_t size = regfile.read(12);
        char* buf = static_cast<char*>(memory.begin_ptr()) + buf_model;

        switch (syscall_type) {
            case syscall_READ:
                status = ::read(fd, buf, size);
                return status;
                break;
            case syscall_WRITE:
                status = ::write(fd, buf, size);
                return status;
                break;
            case syscall_EXIT:
                stop_process_flag = true;
                return status;
                break;
            default:
                throw std::runtime_error ("Unknown system call: PC = 0x" + num2hex(PC));
                break;
        }
    } else if (imm == 1) { //ebreak
        logstream << "EBREAK" << std::endl;
        stop_process_flag = true;
    } else
        throw std::runtime_error ("Unknown system instruction: PC = 0x" + num2hex(PC));

    return 0;
}

void Processor::execute_fence(instD* dec_inst) { //? nop
    logstream << "FENCE" << std::endl;
    throw std::runtime_error ("Unknown fence instruction: PC = 0x" + num2hex(PC));
}

}
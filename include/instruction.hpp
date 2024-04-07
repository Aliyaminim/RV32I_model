#pragma once

#include <cstdio>

namespace rv32i_model {

class instD {
public:
    virtual ~instD() = default;
};

class instD_R final : public instD {
    uint32_t opc;
    uint32_t rd;
    uint32_t rs1;
    uint32_t rs2;
    uint32_t funct3;
    uint32_t funct7;

    friend class Processor;
public:
    explicit instD_R(uint32_t& inst) {
        opc = inst & 0x7F;
        rd = (inst >> 7) & 0x1F;
        rs1 = (inst >> 15) & 0x1F;
        rs2 = (inst >> 20) & 0x1F;
        funct3 = (inst >> 12) & 0x7;
        funct7 = (inst >> 25) & 0x7F;
    }

    ~instD_R() = default;
};

class instD_I final : public instD {
    uint32_t opc;
    uint32_t rd;
    uint32_t rs1;
    uint32_t funct3;
    int32_t imm;

    friend class Processor;
public:
    explicit instD_I(uint32_t& inst) {
        opc = inst & 0x7F;
        rd = (inst >> 7) & 0x1F;
        rs1 = (inst >> 15) & 0x1F;
        funct3 = (inst >> 12) & 0x7;
        imm = (int32_t)(((int32_t) inst) >> 20);
    }

    ~instD_I() = default;
};

class instD_S final : public instD {
    uint32_t opc;
    uint32_t rs1;
    uint32_t rs2;
    uint32_t funct3;
    int32_t imm;

    friend class Processor;
public:
    explicit instD_S(uint32_t& inst) {
        opc = inst & 0x7F;
        rs1 = (inst >> 15) & 0x1F;
        rs2 = (inst >> 20) & 0x1F;
        funct3 = (inst >> 12) & 0x7;
        uint32_t imm_tmp = ((inst >> 7) & 0x1F) + ((((int32_t)inst) >> 25) << 5);
        imm = (int32_t)imm_tmp;
    }
    ~instD_S() = default;
};

class instD_B final : public instD {
    uint32_t opc;
    uint32_t rs1;
    uint32_t rs2;
    uint32_t funct3;
    int32_t imm;

    friend class Processor;
public:
    explicit instD_B(uint32_t& inst) {
        opc = inst & 0x7F;
        rs1 = (inst >> 15) & 0x1F;
        rs2 = (inst >> 20) & 0x1F;
        funct3 = (inst >> 12) & 0x7;
        uint32_t imm_tmp = ((inst & 0x80000000) ? 0xFFFFE000 : 0x0);
        imm_tmp += (((inst >> 7) & 0x1) << 11) + (((inst >> 8) & 0xF) << 1);
        imm_tmp += (((inst >> 25) & 0x3F) << 5) + (((inst >> 31) & 0x1) << 12);
        imm = (int32_t)imm_tmp;
    }
    ~instD_B() = default;
};

class instD_U final : public instD {
    uint32_t opc;
    uint32_t rd;
    int32_t imm;

    friend class Processor;
public:
    explicit instD_U(uint32_t& inst) {
        opc = inst & 0x7F;
        rd = (inst >> 7) & 0x1F;
        imm = (int32_t)(inst & 0xFFFFF000);
    }
    ~instD_U() = default;
};

class instD_J final : public instD {
    uint32_t opc;
    uint32_t rd;
    int32_t imm;

    friend class Processor;
public:
    explicit instD_J(uint32_t& inst) {
        opc = inst & 0x7F;
        rd = (inst >> 7) & 0x1F;
        uint32_t imm_tmp = ((inst & 0x80000000) ? 0xFFE00000 : 0x0);
        imm_tmp += (((inst >> 12) & 0xFF) << 12)+ (((inst >> 20) & 0x1) << 11);
        imm_tmp += ((inst >> 21) & 0x3FF) << 1;
        imm_tmp += ((inst >> 31) & 0x1) << 20;
        imm = (int32_t)imm_tmp;
    }
    ~instD_J() = default;
};

}
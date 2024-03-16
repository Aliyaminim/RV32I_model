#pragma once

/* RV32I contains 40 unique instructions, though a simple implementation might

cover the ECALL/EBREAK instructions with a single SYSTEM hardware instruction that al-
ways traps and might be able to implement the FENCE instruction as a NOP, reducing base

instruction count to 38 total.*/

/* The RISC-V ISA keeps the source (rs1 and rs2) and destination (rd) registers at the same position
in all formats to simplify decoding.*/

//exceptions for strange instruction

#include <cstdint>
#include <array>
#include <vector>
#include <cassert>
#include "instruction.hpp"


namespace rv32i_model {

class Regfile final {
/*  reg[0] = 0 (always)
    reg[1] - return address for a call(что вернула функция)
    reg[5] - alternate link register
    reg[2] - stack pointer
*/
    std::array<int32_t,32> regs;
public:
    Regfile() { regs.fill(0); }
};

class Memory final {
    std::vector<int32_t> mem;
public:
    Memory(std::size_t memory_size) { mem.reserve(memory_size); }

    Memory& operator=(std::vector<int32_t>&& other_mem) {
        mem = other_mem;
        return *this;
    }

    int32_t& operator[](std::size_t i) & { return mem[i]; }

    std::size_t inst_size() { return mem.size(); };
};


class Processor final {
    Regfile regfile;
    Memory memory;
    std::size_t PC;
public:
    Processor(std::size_t memory_size) : regfile(), memory(memory_size), PC(-1) {}

    void load_input_to_memory(auto && data) {
        memory = std::move(data);
        PC = 0;
    }

    int32_t& fetch() {
        int32_t& inst = memory[PC];
        PC++;
        return inst;
    }

    inst_t decode(int32_t& inst) {
        //применение масок
        int opcode = inst & 0x7F;
        int rd = (inst >> 7) & 0x1F;
        int rs1 = (inst >> 15) & 0x1F;
        int rs2 = (inst >> 20) & 0x1F;
        int funct3 = (inst >> 12) & 0x7;
        int funct7 = (inst >> 25) & 0x7F;

        return inst_t(opcode, rd, rs1, rs2, funct3, funct7);
    }

    void execute() {}

    void process() {
        std::size_t inst_number = memory.inst_size();
        while(PC >= 0 && PC < inst_number) {
            int32_t& inst = fetch();
            auto[opc, rd, rs1, rs2, funct3, funct7] = decode(inst);
            std::cout << inst << " ";
        }
        std::cout << std::endl;
    }

};
}
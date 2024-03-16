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
};


class Processor final {
    Regfile regfile;
    Memory memory;
    int32_t& pc; //hmm...
public:
    Processor(std::size_t memory_size) : regfile(), memory(memory_size) {}

    void load_input_to_memory(auto && data) {
        memory = std::move(data);
        pc = memory[0];
    }

};
}
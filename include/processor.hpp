#pragma once

/* RV32I contains 40 unique instructions, though a simple implementation might
cover the ECALL/EBREAK instructions with a single SYSTEM hardware instruction that
always traps and might be able to implement the FENCE instruction as a NOP, reducing base
instruction count to 38 total.*/

#include <cstdint>
#include <array>
#include <vector>
#include <cassert>
#include "regfile.hpp"
#include "memory.hpp"
#include "instruction.hpp"


namespace rv32i_model {

class Processor final {
    Regfile regfile;
    Memory memory;
    std::size_t PC;
public:
    Processor(std::size_t memory_size) : regfile(), memory(memory_size), PC(-1) {}

    void load_input_to_memory(auto && data) {
        memory = std::move(data);
        set_PC(0);
    }

private:
    uint32_t& fetch() { return memory[PC]; }

    int execute(uint32_t& inst);

public:
    void process() {
        std::size_t inst_number = memory.inst_size();

        while(PC >= 0 && PC < inst_number) {
            uint32_t& inst = fetch();
            if (!execute(inst))
                increase_PC();
        }
    }

    //for testing
    void write_to_reg(std::size_t reg, int32_t value) { regfile.write(reg, value); }
    int32_t read_reg(std::size_t reg) const { return regfile.read(reg); }

    void write_to_mem(int32_t address, int32_t value) { memory.write(address, value); }
    uint32_t read_mem(int32_t address) const { return memory.read(address); }

private:
    typedef void (execute_inst)(instD* dec_inst);

    execute_inst execute_op_imm;
    execute_inst execute_lui;
    execute_inst execute_auipc;
    execute_inst execute_op;
    execute_inst execute_jal;
    execute_inst execute_jalr;
    execute_inst execute_load;
    execute_inst execute_store;
    execute_inst execute_system;
    execute_inst execute_fence;
    int execute_branch(instD* dec_inst);

    void increase_PC(std::size_t value = 1) { PC += value; }
    std::size_t get_PC() const noexcept{ return PC; }
    void set_PC(std::size_t value) { PC = value; }
};

}
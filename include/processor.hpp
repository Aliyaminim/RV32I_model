#pragma once

/* RV32I contains 40 unique instructions, though a simple implementation might
cover the ECALL/EBREAK instructions with a single SYSTEM hardware instruction that
always traps and might be able to implement the FENCE instruction as a NOP, reducing base
instruction count to 38 total.*/

#include <cstdint>
#include <array>
#include <vector>
#include <cassert>
#include <fstream>
#include <iostream>
#include "regfile.hpp"
#include "memory.hpp"
#include "instruction.hpp"


namespace rv32i_model {

class Processor final {
    Regfile regfile;
    Memory memory;
    uint32_t PC;
    std::ostream& logstream;
public:
    Processor(std::size_t memory_size, std::ostream& _logstream_ = std::cout) : regfile(), memory(memory_size),
        PC(0), logstream(_logstream_) {}

    template <typename It>
    void load_input_to_memory(It input_start, It input_fin, uint32_t start_addr = 0) {
        memory.fill(input_start, input_fin);
        set_PC(start_addr);
    }

private:
    uint32_t& fetch() { return read_mem(PC); }

    int execute(uint32_t& inst);

public:
    bool stop_process_flag = false;
    bool jump_flag = false;

    int process() {
        int exec = 0;
        for (;;) {
            if(PC >= memory.size() * sizeof(uint32_t))
                break;

            uint32_t& inst = fetch();
            exec = execute(inst);

            if (!jump_flag)
                increase_PC();
            else
                jump_flag = false;

            if (stop_process_flag)
                break;
        }

        return exec;
    }

    //for testing
    void write_to_reg(std::size_t reg, int32_t value) { regfile.write(reg, value); }
    int32_t read_reg(std::size_t reg) const { return regfile.read(reg); }

    void write_to_mem(int32_t address, int32_t value) { memory.write(address, value); }
    uint32_t& read_mem(int32_t address) { return memory.read(address); }

    void dump_memory() const { memory.dump(logstream); }

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
    execute_inst execute_fence;
    execute_inst execute_branch;
    int execute_system(instD* dec_inst);

    void increase_PC(std::size_t value = sizeof(uint32_t)) { PC += value; }
    uint32_t get_PC() const noexcept{ return PC; }
    void set_PC(uint32_t value) { PC = value; }
};

}
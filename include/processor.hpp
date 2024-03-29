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
        PC = 0;
    }

private:
    uint32_t& fetch() {
        return memory[PC];
    }

    void execute(uint32_t& inst);

public:
    void process() {
        std::size_t inst_number = memory.inst_size();

        while(PC >= 0 && PC < inst_number) {
            uint32_t& inst = fetch();
            execute(inst);
            //regfile.dump();
            //write_back
            PC++; //?
        }
    }

private:
    typedef void (execute_inst)(instD* dec_inst);

    execute_inst execute_op_imm;
    execute_inst execute_lui;
    execute_inst execute_auipc;
    execute_inst execute_op;
    execute_inst execute_jal;
    execute_inst execute_jalr;
    execute_inst execute_branch;
    execute_inst execute_load;
    execute_inst execute_store;
    execute_inst execute_system;
    execute_inst execute_fence;
};

}
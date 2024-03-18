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
    int32_t& fetch() {
        int32_t& inst = memory[PC];
        PC++;
        return inst;
    }

    inst_d decode(int32_t& inst) {
        //применение масок
        unsigned int opcode = inst & 0x7F;
        unsigned int rd = (inst >> 7) & 0x1F;
        unsigned int rs1 = (inst >> 15) & 0x1F;
        unsigned int rs2 = (inst >> 20) & 0x1F;
        unsigned int funct3 = (inst >> 12) & 0x7;
        unsigned int funct7 = (inst >> 25) & 0x7F;

        return inst_d(opcode, rd, rs1, rs2, funct3, funct7);
    }

    void execute(inst_d dec_inst);

    typedef void (execute_dinst)(unsigned int funct7, unsigned int rs2, \
                            unsigned int rs1, unsigned int funct3, unsigned int rd);

    execute_dinst execute_op_imm;
    execute_dinst execute_op;
    execute_dinst execute_jal;
    execute_dinst execute_jalr;
    execute_dinst execute_branch;
    execute_dinst execute_load;
    execute_dinst execute_store;
    execute_dinst execute_system;
    execute_dinst execute_fence;

public:
    void process() {
        std::size_t inst_number = memory.inst_size();
        while(PC >= 0 && PC < inst_number) {
            int32_t& inst = fetch();
            inst_d dec_inst = decode(inst);
            execute(dec_inst);
            //regfile.dump();
            //write_back

        }
    }

};

}
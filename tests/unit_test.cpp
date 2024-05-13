#include <gtest/gtest.h>
#include "rv32i_model.hpp"
#include <vector>

using namespace rv32i_model;

TEST(Arithmetic_inst, ADD) {
    Processor model(1024);

    model.write_to_reg(3, 15);
    model.write_to_reg(2, 80);

    std::vector<uint32_t> input{0x3100B3};
    std::size_t num_instr = input.size();
    model.load_input_to_memory(input.begin(), input.end());
    model.process(num_instr);

    int32_t answer = (int32_t)model.read_reg(1);
    EXPECT_EQ(answer, 95);
}

TEST(Arithmetic_inst, SUB) {
    Processor model(1024);
    model.write_to_reg(2, 15);
    model.write_to_reg(3, -50);

    std::vector<uint32_t> input{0x403100B3};
    std::size_t num_instr = input.size();
    model.load_input_to_memory(input.begin(), input.end());
    model.process(num_instr);

    int32_t answer = (int32_t)model.read_reg(1);
    EXPECT_EQ(answer, 65);
}

TEST(Arithmetic_inst, ADDI) {
    Processor model(1024);
    model.write_to_reg(2, 15);

    std::vector<uint32_t> input{0x310093};
    std::size_t num_instr = input.size();
    model.load_input_to_memory(input.begin(), input.end());
    model.process(num_instr);

    EXPECT_EQ(model.read_reg(1), 18);
}

TEST(Arithmetic_inst, SLTI) {
    Processor model(1024);
    model.write_to_reg(2, 15);

    std::vector<uint32_t> input{0x1412093};
    std::size_t num_instr = input.size();
    model.load_input_to_memory(input.begin(), input.end());
    model.process(num_instr);

    EXPECT_EQ(model.read_reg(1), 1);
}

TEST(LUI_inst, LUI) {
    Processor model(1024);

    std::vector<uint32_t> input{0x2137};
    std::size_t num_instr = input.size();
    model.load_input_to_memory(input.begin(), input.end());
    model.process(num_instr);

    EXPECT_EQ(model.read_reg(2), 8192);

}

TEST(AUIPC_inst, AUIPC) {
    Processor model(1024);
    model.write_to_reg(2, 4);

    std::vector<uint32_t> input{0x3100B3, 0x2117, 0x403100B3};
    std::size_t num_instr = input.size();
    model.load_input_to_memory(input.begin(), input.end());
    model.process(num_instr);

    EXPECT_EQ(model.read_reg(2), 8196);
}

TEST(Jump_inst, JAL) {
    Processor model(1024);
    model.write_to_reg(3, 20);
    model.write_to_reg(2, 10);

    std::vector<uint32_t> input{0x8002EF, 0x3100B3, 0x403100B3};
    std::size_t num_instr = input.size();
    model.load_input_to_memory(input.begin(), input.end());
    model.process(num_instr);

    EXPECT_EQ(model.read_reg(1), -10);
}

TEST(Jump_inst, JALR) {
    Processor model(1024);
    model.write_to_reg(3, 20);
    model.write_to_reg(2, 10);
    model.write_to_reg(7, 4);

    std::vector<uint32_t> input{0x4382E7, 0x3100B3, 0x403100B3};
    std::size_t num_instr = input.size();
    model.load_input_to_memory(input.begin(), input.end());
    model.process(num_instr);

    EXPECT_EQ(model.read_reg(1), -10);
}



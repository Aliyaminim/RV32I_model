#include <gtest/gtest.h>
#include "rv32i_model.hpp"
#include <vector>

using namespace rv32i_model;

TEST(Arithmetic_inst, ADD) {
    Processor model(1024);

    model.write_to_reg(3, 15);
    model.write_to_reg(2, 80);

    std::vector<uint32_t> input{0x3100B3};
    model.load_input_to_memory(input.begin(), input.end());
    model.process();

    int32_t answer = (int32_t)model.read_reg(1);
    EXPECT_EQ(answer, 95);
}

TEST(Arithmetic_inst, SUB) {
    Processor model(1024);
    model.write_to_reg(2, 15);
    model.write_to_reg(3, -50);

    std::vector<uint32_t> input{0x403100B3};
    model.load_input_to_memory(input.begin(), input.end());
    model.process();

    int32_t answer = (int32_t)model.read_reg(1);
    EXPECT_EQ(answer, 65);
}

TEST(Arithmetic_inst, ADDI) {
    Processor model(1024);
    model.write_to_reg(2, 15);

    std::vector<uint32_t> input{0x310093};
    model.load_input_to_memory(input.begin(), input.end());
    model.process();

    EXPECT_EQ(model.read_reg(1), 18);
}

TEST(Arithmetic_inst, SLTI) {
    Processor model(1024);
    model.write_to_reg(2, 15);

    std::vector<uint32_t> input{0x1412093};
    model.load_input_to_memory(input.begin(), input.end());
    model.process();

    EXPECT_EQ(model.read_reg(1), 1);
}

TEST(LUI_inst, LUI) {
    Processor model(1024);

    std::vector<uint32_t> input{0x2137};
    model.load_input_to_memory(input.begin(), input.end());
    model.process();

    EXPECT_EQ(model.read_reg(2), 8192);

}

TEST(AUIPC_inst, AUIPC) {
    Processor model(1024);
    model.write_to_reg(2, 4);

    std::vector<uint32_t> input{0x3100B3, 0x2117, 0x403100B3};
    model.load_input_to_memory(input.begin(), input.end());
    model.process();

    EXPECT_EQ(model.read_reg(2), 8193);
}

TEST(Jump_inst, JAL) {
    Processor model(1024);
    model.write_to_reg(3, 20);
    model.write_to_reg(2, 10);

    std::vector<uint32_t> input{0x8002EF, 0x3100B3, 0x403100B3};
    model.load_input_to_memory(input.begin(), input.end());
    model.process();

    EXPECT_EQ(model.read_reg(1), -10);
}

TEST(Jump_inst, JALR) {
    Processor model(1024);
    model.write_to_reg(3, 20);
    model.write_to_reg(2, 10);
    model.write_to_reg(7, 4);

    std::vector<uint32_t> input{0x4382E7, 0x3100B3, 0x403100B3};
    model.load_input_to_memory(input.begin(), input.end());
    model.process();

    EXPECT_EQ(model.read_reg(1), -10);
}

//00010 00001 111  00010 1100011
TEST(Branch_inst, BEQ) {
    Processor model(1024);
    model.write_to_reg(1, 10);
    model.write_to_reg(2, 10);

    std::vector<uint32_t> input{0x208463, 0x6464, 0x197};
    model.load_input_to_memory(input.begin(), input.end());
    model.process();

    EXPECT_EQ(model.read_reg(3), 2);
}

TEST(Branch_inst, BNE) {
    Processor model(1024);
    model.write_to_reg(1, 10);
    model.write_to_reg(2, 9);

    std::vector<uint32_t> input{0x209463, 0x6454, 0x197};
    model.load_input_to_memory(input.begin(), input.end());
    model.process();

    EXPECT_EQ(model.read_reg(3), 2);
}

TEST(Branch_inst, BLT) {
    Processor model(1024);
    model.write_to_reg(1, -5);
    model.write_to_reg(2, -1);

    std::vector<uint32_t> input{0x20C463, 0x6454, 0x197};
    model.load_input_to_memory(input.begin(), input.end());
    model.process();

    EXPECT_EQ(model.read_reg(3), 2);
}

TEST(Branch_inst, BGE) {
    Processor model(1024);
    model.write_to_reg(1, -3834);
    model.write_to_reg(2, -38238);

    std::vector<uint32_t> input{0x20D463, 0x6454, 0x197};
    model.load_input_to_memory(input.begin(), input.end());
    model.process();

    EXPECT_EQ(model.read_reg(3), 2);
}

TEST(Branch_inst, BLTU) {
    Processor model(1024);
    model.write_to_reg(1, -1);
    model.write_to_reg(2, -2);

    std::vector<uint32_t> input{0x20E463, 0x6454, 0x197};
    model.load_input_to_memory(input.begin(), input.end());
    try {
        model.process();
        EXPECT_TRUE(false);
    }
    catch(std::runtime_error& err){
        EXPECT_TRUE(true);
    }
}

TEST(Branch_inst, BGEU){
    Processor model(1024);
    model.write_to_reg(1, -3834);
    model.write_to_reg(2, -38238);

    std::vector<uint32_t> input{0x20F463, 0x6454, 0x197};
    model.load_input_to_memory(input.begin(), input.end());
    model.process();

    EXPECT_EQ(model.read_reg(3), 2);
}

TEST(Load_inst, load_32) {
    Processor model(4096);
    model.write_to_mem(3, 12345678);

    std::vector<uint32_t> input{0b00000000001100010010000100000011};
    model.load_input_to_memory(input.begin(), input.end());
    model.process();

    EXPECT_EQ(model.read_reg(2), 12345678);
}

TEST(Load_inst, load_16) {
    Processor model(4096);
    model.write_to_mem(3, 12345678);

    std::vector<uint32_t> input{0b00000000001100010001000100000011};
    model.load_input_to_memory(input.begin(), input.end());
    model.process();

    EXPECT_EQ(model.read_reg(2), 24910);
}

TEST(Load_inst, load_8) {
    Processor model(4096);
    model.write_to_mem(3, 12345678);

    std::vector<uint32_t> input{0b00000000001100010000000100000011};
    model.load_input_to_memory(input.begin(), input.end());
    model.process();

    EXPECT_EQ(model.read_reg(2), 78);
}

TEST(Store_inst, store_32) {
    Processor model(4096);
    model.write_to_reg(1, 3);
    model.write_to_reg(2, 100);

    std::vector<uint32_t> input{0b00000000001000001010001110100011};
    model.load_input_to_memory(input.begin(), input.end());
    model.process();

    EXPECT_EQ(model.read_mem(10), 100);
}

TEST(Store_inst, store_16) {
    Processor model(4096);
    model.write_to_reg(1, 3);
    model.write_to_reg(2, 165535);

    std::vector<uint32_t> input{0b00000000001000001001001110100011};
    model.load_input_to_memory(input.begin(), input.end());
    model.process();

    EXPECT_EQ(model.read_mem(10), 34463);
}


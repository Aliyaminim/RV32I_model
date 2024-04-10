#include <iostream>
#include <fstream>
#include <vector>
#include <endian.h>
#include <filesystem>
#include <string>
#include "rv32i_model.hpp"

std::vector<uint32_t> get_memory_image(const char * filename) {
    if (!std::filesystem::is_regular_file(filename))
        throw std::runtime_error (std::string("ERROR:\nCan't open file ") + std::string(filename));

    std::ifstream binary_input{filename, std::ios::binary};
    if (!binary_input.is_open())
        throw std::runtime_error (std::string("ERROR:\nCan't open file ") + std::string(filename));

    std::vector<uint32_t> memory_image;
    uint32_t n;
    while(binary_input.read((char*)&n, sizeof(n))) {
        memory_image.push_back(le32toh(n));
        n = 0;
    }

    if (binary_input.gcount() > 0)
        memory_image.push_back(le32toh(n));

    return memory_image;
}


int main(int argc, char **argv) {
    if (argc != 2) {
        std::cerr << "ERROR:\nPlease enter: ./main <file>" << std::endl;
        return 1;
    }

    try {
        std::size_t mem_size = (1ull << 32) - 1; //RV32I provides a 32-bit address space
        std::ofstream logstream("logfile.out");
        rv32i_model::Processor model_rv32i(mem_size, logstream);

        std::vector<uint32_t> memory_image = get_memory_image(argv[1]);
        model_rv32i.load_input_to_memory(memory_image.begin(), memory_image.end());

        #ifdef DEBUG
        model_rv32i.dump_memory();
        #endif

        model_rv32i.process();

    }
    catch(const std::runtime_error& err) {
        std::cerr << err.what() << std::endl;
        return 1;
    }

    return 0;
}
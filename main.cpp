#include <iostream>
#include "proc.hpp"

#include <vector>

auto read_input(std::istream& is) {
    std::vector<int32_t> input;
    int32_t inst;

    while(is >> std::hex >> inst, is.good())
        input.push_back(inst);

    return input;
}

int main() {
    auto in = read_input(std::cin);

    rv32i_model::Processor pr(64);
    pr.load_input_to_memory(in);

    pr.process();

    return 0;
}
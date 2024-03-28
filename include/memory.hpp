#pragma once

#include <vector>

namespace rv32i_model {

class Memory final {
    std::vector<uint32_t> mem;
public:
    Memory(std::size_t memory_size) { mem.reserve(memory_size); }

    Memory& operator=(std::vector<uint32_t>&& other_mem) {
        mem = other_mem;
        return *this;
    }

    uint32_t& operator[](std::size_t i) & { return mem[i]; }

    std::size_t inst_size() { return mem.size(); };
};

}
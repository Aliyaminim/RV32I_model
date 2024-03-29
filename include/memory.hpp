#pragma once

#include <vector>
#include <exception>

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

    void write(int32_t effective_address, int32_t value) {
        if ((effective_address < 0) || (effective_address >= (int32_t)mem.capacity()))
            throw std::runtime_error("Cannot write to memory, invalid address");

        mem[effective_address] = value;
    }

    uint32_t read(int32_t effective_address) const {
        if ((effective_address < 0) || (effective_address >= (int32_t)mem.size()))
            throw std::runtime_error("Cannot read from memory, invalid address");

        return mem[effective_address];
    }

};

}
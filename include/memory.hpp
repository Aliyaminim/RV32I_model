#pragma once

#include <vector>
#include <exception>

namespace rv32i_model {

class Memory final {
    std::vector<uint32_t> mem;
public:
    Memory(std::size_t memory_size /*in bytes*/) { mem.reserve(memory_size / sizeof(uint32_t) + 1); }

    uint32_t& operator[](std::size_t i) & { return mem[i]; }
    void* begin_ptr() { return mem.data(); }

    void write(int32_t effective_address, int32_t value) {
        if ((effective_address < 0) || (effective_address >= (int32_t)mem.size() * sizeof(int32_t)))
            throw std::runtime_error("Cannot write to memory, invalid address");

        if ( !(effective_address & 0x3) ) {
            // The address is 4-byte aligned here
            std::size_t addr_2index = effective_address /sizeof(uint32_t);
            mem[addr_2index] = value;
        } else
            throw std::runtime_error ("An address-misaligned exception");
    }

    uint32_t read(int32_t effective_address) const {
        if ((effective_address < 0) || (effective_address >= (int32_t)mem.size() * sizeof(int32_t)))
            throw std::runtime_error("Cannot read from memory, invalid address");

        if ( !(effective_address & 0x3) ) {
            // The address is 4-byte aligned here
            std::size_t addr_2index = effective_address /sizeof(uint32_t);
            return mem[addr_2index];
        } else
            throw std::runtime_error ("An address-misaligned exception");
    }

    template <typename It>
    void fill(It input_start, It input_fin) {
        std::move(input_start, input_fin, std::back_inserter(mem));
    }

    void dump(std::ostream& os) const {
        uint32_t addr = 0;
        bool flag_null = false;
        for (auto && elem: mem) {
            if (elem != 0) {
                os << std::setw(8) << std::setfill('0') << addr << ": 0x" << std::hex << std::setw(8) << std::setfill('0') << elem << std::endl;
                flag_null = true;
            } else if (flag_null) {
                os << "..." << std::endl;
                flag_null = false;
            }
            addr += 4;
        }
    }

};

}
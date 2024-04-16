#pragma once

#include <vector>
#include <bitset>
#include <climits>
#include <stdexcept>

namespace rv32i_model {

class Memory final {
    std::vector<uint32_t> mem;
    std::size_t size_;
public:
    Memory(std::size_t memory_size /*in bytes*/) : size_(memory_size) {
        mem.resize(memory_size / sizeof(uint32_t) + 1);
    }

    uint32_t& operator[](std::size_t i) & { return mem[i]; }
    void* begin_ptr() { return mem.data(); }
    std::size_t size() const { return size_; }

    uint32_t& read(uint32_t effective_address) {
        if ((effective_address + sizeof(int32_t)) >= size_)
            throw std::out_of_range("Cannot read from memory, invalid address");

        if ( !(effective_address & 0x3) ) {
            // The address is 4-byte aligned here
            std::size_t addr_2index = effective_address / sizeof(uint32_t);
            #ifdef DEBUG
            std::cout << mem[addr_2index] << std::endl;
            #endif
            return mem[addr_2index];
        } else
            throw std::runtime_error ("An address-misaligned exception");
    }

    void read(uint32_t address, std::size_t size_, char * data) {
        if ((address + size_ * sizeof(char)) >= size_)
            throw std::out_of_range("Cannot read from memory, invalid address");

        char* elem_byte = static_cast<char*>(begin_ptr()) + address;

        for (std::size_t i = 0; i < size_; ++i)
            data[i] = elem_byte[i];
    }

    void write(uint32_t address, std::size_t size_, const char * data) {
        if ((address + size_ * sizeof(char)) >= size_)
            throw std::out_of_range("Cannot write to memory, invalid address");

        char* elem_byte = static_cast<char*>(begin_ptr()) + address;

        for (std::size_t i = 0; i < size_; ++i)
            elem_byte[i] = data[i];
    }

    template <typename It>
    void fill(It input_start, It input_fin) {
        std::move(input_start, input_fin, mem.begin());
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
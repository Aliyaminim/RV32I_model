#pragma once

#include <cstdint>
#include <array>
#include <stdexcept>

namespace rv32i_model {

class Regfile final {
/*  regs[0] = 0 (always)
    regs[1] - return address for a call
    regs[5] - alternate link register
    regs[2] - stack pointer
*/
    std::array<int32_t, 32> regs;
public:
    Regfile() : regs{} {}

    int32_t read(std::size_t i) const { return regs[i]; }

    void write(std::size_t i, int32_t value) {
        if (i != 0)
            regs[i] = value;
    }

    void dump() const {
        for (const int32_t& s : regs)
            std::cout << s << std::endl;
    }

};

}
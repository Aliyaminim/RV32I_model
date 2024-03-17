#pragma once

#include <cstdint>
#include <array>

namespace rv32i_model {

class Regfile final {
/*  reg[0] = 0 (always)
    reg[1] - return address for a call(что вернула функция)
    reg[5] - alternate link register
    reg[2] - stack pointer
*/
    std::array<int32_t,32> regs;
public:
    Regfile() { regs.fill(0); regs[0] = 10; }

    int32_t read(std::size_t i) const { return regs[i]; }

    void write(std::size_t i, int32_t value) {
        if (i == 0)
            throw std::runtime_error("Forbidden to change value x0");
        else
            regs[i] = value;
    }

    void dump() {
        for (auto s : regs)
            std::cout << s << std::endl;
    }

};

}
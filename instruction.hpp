#pragma once

#include <cstdio>

namespace rv32i {

class inst_t final {
    int32_t opcode;
    int32_t rs1, rs2, rd;
    int32_t funct;
}

}
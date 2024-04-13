#include <optional>
#include <cstring>
#include "RVM.h"
#include "rv32i_model.hpp"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

class SnippyRISCVSimulator final {
    std::optional<std::ofstream> LogFile;
    rv32i_model::Processor proc;

public:
    SnippyRISCVSimulator(std::size_t memory_size, const char *LogFilePath) : LogFile(LogFilePath),
                                                            proc(memory_size, LogFile.value()) {}

    explicit SnippyRISCVSimulator(std::size_t memory_size) : proc(memory_size) {}

    int executeInstr() { return proc.process_instr(); }

    int8_t readByte(uint32_t addr) { return proc.read_byte(addr); }
    void writeByte(uint32_t addr, const char * Data) { proc.write_byte(addr, Data); }

    uint64_t readPC() { return proc.get_PC(); }
    void setPC(uint64_t NewPC) { proc.set_PC(NewPC); }

    RVMRegT readXReg(RVMXReg Reg) { return proc.read_reg((std::size_t)Reg); }
    void setXReg(RVMXReg Reg, RVMRegT NewValue) {
        proc.write_to_reg((std::size_t)Reg, NewValue);
    }

    ~SnippyRISCVSimulator() = default;
};


struct RVMState {
    RVMConfig Config;
    SnippyRISCVSimulator* Model;
};

RVMState *rvm_modelCreate(const RVMConfig *config) {
    SnippyRISCVSimulator* model;
    if (strlen(config->LogFilePath) == 0)
        model = new SnippyRISCVSimulator(config->RamSize);
    else
        model = new SnippyRISCVSimulator(config->RamSize, config->LogFilePath);

    return new RVMState(*config, model);

}

void rvm_modelDestroy(RVMState *State) {
    if (State == NULL)
        throw std::runtime_error("There is no existing state to destroy model");
    State->Model->~SnippyRISCVSimulator();
}

const RVMConfig *rvm_getModelConfig(const RVMState *State) {
    if (State == NULL)
        throw std::runtime_error("There is no existing state to get ModelConfig");
    return &(State->Config);
}

int rvm_executeInstr(RVMState *State) {
    if (State == NULL)
        throw std::runtime_error("There is no existing state to execute instruction");
    return State->Model->executeInstr();
}

void rvm_readMem(const RVMState *State, uint64_t Addr, size_t Count,
                 char *Data) {
    if (State == NULL)
        throw std::runtime_error("There is no existing state to read from memory");
    for (auto Idx = 0; Idx < Count; ++Idx)
        Data[Idx] = State->Model->readByte(Addr + Idx);
}

void rvm_writeMem(RVMState *State, uint64_t Addr, size_t Count,
                  const char *Data) {
    if (State == NULL)
        throw std::runtime_error("There is no existing state to write to memory");
    for (auto Idx = 0; Idx < Count; ++Idx)
        State->Model->writeByte(Addr + Idx, Data + Idx);
}

uint64_t rvm_readPC(const RVMState *State) {
    if (State == NULL)
        throw std::runtime_error("There is no existing state to read PC");
    return State->Model->readPC();
}

void rvm_setPC(RVMState *State, uint64_t NewPC) {
    if (State == NULL)
        throw std::runtime_error("There is no existing state to set PC");
    State->Model->setPC(NewPC);
}

RVMRegT rvm_readXReg(const RVMState *State, RVMXReg Reg) {
    if (State == NULL)
        throw std::runtime_error("There is no existing state to read XReg");
    return State->Model->readXReg(Reg);
}

void rvm_setXReg(RVMState *State, RVMXReg Reg, RVMRegT Value) {
    if (State == NULL)
        throw std::runtime_error("There is no existing state to write to XReg");
    State->Model->setXReg(Reg, Value);
}

void rvm_logMessage(const char *Message) { std::cout << Message; }

#ifdef __cplusplus
}
#endif // __cplusplus
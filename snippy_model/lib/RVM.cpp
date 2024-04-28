#include <optional>
#include <cstring>
#include <stdexcept>
#include "RVM.h"
#include "rv32i_model.hpp"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

class SnippyRISCVSimulator final {
    std::optional<std::ofstream> LogFile;
    rv32i_model::Processor proc;

public:
    SnippyRISCVSimulator(uint32_t memory_start, std::size_t memory_size, const char *LogFilePath) : LogFile(LogFilePath),
                                                            proc(memory_start, memory_size, LogFile.value()) {}

    SnippyRISCVSimulator(uint32_t memory_start, std::size_t memory_size) : proc(memory_start, memory_size) {}

    int executeInstr() { return proc.process_instr(); }

    void write(uint32_t address, std::size_t size, const char * data) {
        proc.write(address, size, data);
    }

    void read(uint32_t address, std::size_t size, char * data) {
        proc.read(address, size, data);
    }

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
    try {
    SnippyRISCVSimulator* model;

    auto memory_start = ((config->RomStart < config->RamStart) ? config->RomStart : config->RamStart);
    auto RomEnd = config->RomStart + config->RomSize;
    auto RamEnd = config->RamStart + config->RamSize;
    auto memory_end = ((RomEnd > RamEnd) ? RomEnd : RamEnd);

    if (strlen(config->LogFilePath) == 0)
        model = new SnippyRISCVSimulator(memory_start, memory_end - memory_start);
    else
        model = new SnippyRISCVSimulator(memory_start, memory_end - memory_start, config->LogFilePath);

        #ifdef DEBUG
        std::cout << "config->RamSize " << config->RamSize << std::endl;
        std::cout << "config->RamStart " << config->RamStart << std::endl;
        std::cout << "config->RomSize " << config->RomSize << std::endl;
        std::cout << "config->RomStart " << config->RomStart << std::endl;
        #endif

    return new RVMState(*config, model);
    }
    catch(const std::exception& err) {
        std::cerr << err.what() << std::endl;
        std::terminate();
    }
    catch(...) {
        std::cerr << "Unknown error" << std::endl;
        std::terminate();
    }
}

RVMRegT rvm_readFReg(const RVMState *State, RVMFReg Reg) {
    try {
    throw std::runtime_error("rvm_readFReg Not implemented!");
    }
    catch(const std::exception& err) {
        std::cerr << err.what() << std::endl;
        std::terminate();
    }
    catch(...) {
        std::cerr << "Unknown error" << std::endl;
        std::terminate();
    }
}

void rvm_setFReg(RVMState *State, RVMFReg Reg, RVMRegT Value) {
    try {
    throw std::runtime_error("rvm_setFReg Not implemented!");
    }
    catch(const std::exception& err) {
        std::cerr << err.what() << std::endl;
        std::terminate();
    }
    catch(...) {
        std::cerr << "Unknown error" << std::endl;
        std::terminate();
    }
}

RVMRegT rvm_readCSRReg(const RVMState *State, unsigned Reg) {
    // throw std::runtime_error("rvm_readCSRReg Not implemented!");
    return 0;
}

void rvm_setCSRReg(RVMState *State, unsigned Reg, RVMRegT Value) {
    // throw std::runtime_error("rvm_setCSRReg Not implemented!");
    return;
}

int rvm_readVReg(const RVMState *State, RVMVReg Reg, char *Data,
                 size_t MaxSize) {
    try {
    throw std::runtime_error("rvm_readVReg Not implemented!");
    }
    catch(const std::exception& err) {
        std::cerr << err.what() << std::endl;
        std::terminate();
    }
    catch(...) {
        std::cerr << "Unknown error" << std::endl;
        std::terminate();
    }
}

int rvm_setVReg(RVMState *State, RVMVReg Reg, const char *Data,
                size_t DataSize) {
    try {
    throw std::runtime_error("rvm_setVReg Not implemented!");
    }
    catch(const std::exception& err) {
        std::cerr << err.what() << std::endl;
        std::terminate();
    }
    catch(...) {
        std::cerr << "Unknown error" << std::endl;
        std::terminate();
    }
}

int rvm_queryCallbackSupportPresent() {
    try {
    throw std::runtime_error("rvm_queryCallbackSupportPresent Not implemented!");
    }
    catch(const std::exception& err) {
        std::cerr << err.what() << std::endl;
        std::terminate();
    }
    catch(...) {
        std::cerr << "Unknown error" << std::endl;
        std::terminate();
    }
}

void rvm_modelDestroy(RVMState *State) {
    try {
    if (State == NULL)
        throw std::runtime_error("There is no existing state to destroy model");
    State->Model->~SnippyRISCVSimulator();
    }
    catch(const std::exception& err) {
        std::cerr << err.what() << std::endl;
        std::terminate();
    }
    catch(...) {
        std::cerr << "Unknown error" << std::endl;
        std::terminate();
    }
}

const RVMConfig *rvm_getModelConfig(const RVMState *State) {
    try {
    if (State == NULL)
        throw std::runtime_error("There is no existing state to get ModelConfig");
    return &(State->Config);
    }
    catch(const std::exception& err) {
        std::cerr << err.what() << std::endl;
        std::terminate();
    }
    catch(...) {
        std::cerr << "Unknown error" << std::endl;
        std::terminate();
    }
}

int rvm_executeInstr(RVMState *State) {
    try {
    if (State == NULL)
        throw std::runtime_error("There is no existing state to execute instruction");
    return State->Model->executeInstr();
    }
    catch(const std::exception& err) {
        std::cerr << err.what() << std::endl;
        std::terminate();
    }
    catch(...) {
        std::cerr << "Unknown error" << std::endl;
        std::terminate();
    }
}

void rvm_readMem(const RVMState *State, uint64_t Addr, size_t Count,
                 char *Data) {
    try {
    if (State == NULL)
        throw std::runtime_error("There is no existing state to read from memory");

    State->Model->read(Addr, Count, Data);
    }
    catch(const std::exception& err) {
        std::cerr << err.what() << std::endl;
        std::terminate();
    }
    catch(...) {
        std::cerr << "Unknown error" << std::endl;
        std::terminate();
    }
}

void rvm_writeMem(RVMState *State, uint64_t Addr, size_t Count,
                  const char *Data) {
    try {
    if (State == NULL)
        throw std::runtime_error("There is no existing state to write to memory");

    State->Model->write(Addr, Count, Data);
    }
    catch(const std::exception& err) {
        std::cerr << err.what() << std::endl;
        std::terminate();
    }
    catch(...) {
        std::cerr << "Unknown error" << std::endl;
        std::terminate();
    }
}

uint64_t rvm_readPC(const RVMState *State) {
    try {
    if (State == NULL)
        throw std::runtime_error("There is no existing state to read PC");
    return State->Model->readPC();
    }
    catch(const std::exception& err) {
        std::cerr << err.what() << std::endl;
        std::terminate();
    }
    catch(...) {
        std::cerr << "Unknown error" << std::endl;
        std::terminate();
    }
}

void rvm_setPC(RVMState *State, uint64_t NewPC) {
    try {
    if (State == NULL)
        throw std::runtime_error("There is no existing state to set PC");
    State->Model->setPC(NewPC);
    }
    catch(const std::exception& err) {
        std::cerr << err.what() << std::endl;
        std::terminate();
    }
    catch(...) {
        std::cerr << "Unknown error" << std::endl;
        std::terminate();
    }
}

RVMRegT rvm_readXReg(const RVMState *State, RVMXReg Reg) {
    try {
    if (State == NULL)
        throw std::runtime_error("There is no existing state to read XReg");
    return State->Model->readXReg(Reg);
    }
    catch(const std::exception& err) {
        std::cerr << err.what() << std::endl;
        std::terminate();
    }
    catch(...) {
        std::cerr << "Unknown error" << std::endl;
        std::terminate();
    }
}

void rvm_setXReg(RVMState *State, RVMXReg Reg, RVMRegT Value) {
    try {
    if (State == NULL)
        throw std::runtime_error("There is no existing state to write to XReg");
    State->Model->setXReg(Reg, Value);
    }
    catch(const std::exception& err) {
        std::cerr << err.what() << std::endl;
        std::terminate();
    }
    catch(...) {
        std::cerr << "Unknown error" << std::endl;
        std::terminate();
    }
}

void rvm_logMessage(const char *Message) {
    try {
    std::cout << Message << std::endl;
    }
    catch(const std::exception& err) {
        std::cerr << err.what() << std::endl;
        std::terminate();
    }
    catch(...) {
        std::cerr << "Unknown error" << std::endl;
        std::terminate();
    }
}

#ifdef __cplusplus
}
#endif // __cplusplus
#include "Disassembler.hpp"
#include <iostream>

Disassembler::Disassembler(){
    if(cs_open(CS_ARCH_X86, CS_MODE_64, &handle) != CS_ERR_OK){
        std::cerr << "[!] Failed to initialize capstone engine" << std::endl;
        valid = false;
    }
    else{
        valid = true;
    }
}

Disassembler::~Disassembler(){
    if(valid){
        cs_close(&handle);
    }
}

std::vector<Instruction> Disassembler::disassemble(std::span<const uint8_t> data, uint64_t startVAddr){
    std::vector<Instruction> results;
    if(!valid || data.empty()){
        return results;
    }

    cs_insn* instruction;
    size_t count = cs_disasm(handle, data.data(), data.size(), startVAddr, 0, &instruction);

    if(count > 0){
        results.reserve(count);
        for(size_t i = 0; i < count; i++){
            results.push_back({
                    instruction[i].address,
                    instruction[i].mnemonic,
                    instruction[i].op_str
            });
        }
        cs_free(instruction, count);
    }
    else{
        std::cerr << "[!] Capstone failed to disassemble instruction" << std::endl;
    }

    return results;
}

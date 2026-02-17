#pragma once

#include <vector>
#include <string>
#include <cstdint>
#include <span>
#include <capstone/capstone.h>

struct Instruction{
    uint64_t address;
    std::string mnemonic;
    std::string op_str;
};

class Disassembler{
public:
    Disassembler();
    ~Disassembler();

    Disassembler(const Disassembler&) = delete;
    Disassembler& operator = (const Disassembler&) = delete;

    /*
     * Disassemble raw bytes into instruction
     */

    [[nodiscard]]
    std::vector<Instruction> disassemble(std::span<const uint8_t> data, uint64_t startVAddr);

    [[nodiscard]]
    bool is_valid_instruction() const { return valid; }

private:
    csh handle;
    bool valid = false;
};

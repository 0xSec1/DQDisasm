#pragma once

#include <optional>
#include <string>
#include <cstdint>
#include <span>
#include "elfio/elfio.hpp"

class ElfHandler{
public:
    explicit ElfHandler(const std::string& filepath);
    ~ElfHandler() = default;

    ElfHandler(const ElfHandler&) = delete;                 //disable copy constructor
    ElfHandler& operator=(const ElfHandler&) = delete;      //disable assignment operator

    /**
     * Extract view of raw bytes at gives vitual addr for a length
     * on success return bytes, otherwise std::nullopt if fails
     */
    [[nodiscard]]
    std::optional<std::span<const uint8_t>> getData(uint64_t vAddr, size_t length);

    [[nodiscard]]
    bool isValid() const { return valid_elf; }

    [[nodiscard]]
    std::string getError() const { return errMsg; }

private:
    ELFIO::elfio reader;
    bool valid_elf = false;
    std::string errMsg;

    void validateElf();
};

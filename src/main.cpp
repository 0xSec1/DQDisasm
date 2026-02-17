#include "ElfHandler.hpp"
#include "Disassembler.hpp"

int main(int argc, char* argv[]){
    if(argc != 4){
        std::cerr << "Usage: " << argv[0] << " <elf_file> <virtual_addr> <length>\n";
        return 1;
    }

    std::string filename = argv[1];
    uint64_t vAddr = std::stoull(argv[2], nullptr, 16);
    size_t length = std::stoull(argv[3]);

    std::cout << "[*] Loading: " << filename << std::endl;
    ElfHandler elf(filename);

    if(!elf.isValid()){
        std::cerr << "[!] Error: " << elf.getError() << std::endl;
        return 1;
    }

    std::cout << "[*] Extracting " << length << " bytes from " << std::hex << "0x" << vAddr << std::endl;
    auto data = elf.getData(vAddr, length);

    if(!data){
        std::cerr << "[!] Extraction failed: " << elf.getError() << std::endl;
        return 1;
    }

    std::span<const uint8_t> raw_bytes = *data;

    Disassembler disasm;
    if(!disasm.is_valid_instruction()) return 1;

    std::cout << "[*] Disassembling " << length << " bytes at 0x" << std::hex << vAddr << "\n\n";
    auto instructions = disasm.disassemble(raw_bytes, vAddr);

    for(const auto& instr: instructions){
        std::cout << "0x" << std::hex << instr.address << ": " << instr.mnemonic << " " << instr.op_str << std::endl;
    }

    return 0;
}

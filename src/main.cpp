#include "ElfHandler.hpp"
#include "Disassembler.hpp"
#include <cstdint>
#include <fstream>
#include <iostream>
#include <ostream>

void printUsage(const char* program){
    std::cerr << "Usage: " << program << " <elf_file> <start_virtual_addr> (--length <val> | --end <end_virtual_address>) <output_file>\n";
}

int main(int argc, char* argv[]){
    if(argc > 6 || argc < 5){
        printUsage(argv[0]);
        return 1;
    }

    std::string filename = argv[1];
    uint64_t start_vAddr = 0;
    size_t length = 0;
    std::string flag = argv[3];
    std::string outfile = (argc == 6) ? argv[5] : "";

    //parse safely
    try{
        start_vAddr = std::stoull(argv[2], nullptr, 16);
        if(flag == "--length"){
            length = std::stoull(argv[4], nullptr, 10);
            if (length == 0){
                throw std::invalid_argument("Length can't be zero");
            }
        }
        else if(flag == "--end"){
            uint64_t end_vAddr = std::stoull(argv[4], nullptr, 16);
            if(end_vAddr <= start_vAddr){
                std::cerr << "[!] Error: --end address can't be less than start address\n";
                return 1;
            }
            length = end_vAddr - start_vAddr;
        }
        else{
            std::cerr << "[!] Error: Invalid flag, Use --length or --end\n";
            printUsage(argv[0]);
            return 1;
        }
    }
    catch(const std::exception& e){
        std::cerr << "[!] Error: Parsing numerical arguments " << e.what() << "\n";
        std::cerr << "    Note: start and end address takes HEX and length takes DECIMAL.\n";
        return 1;
    }

    std::cout << "[*] Loading: " << filename << std::endl;
    ElfHandler elf(filename);

    if(!elf.isValid()){
        std::cerr << "[!] Error: " << elf.getError() << std::endl;
        return 1;
    }

    std::cout << "[*] Extracting " << length << " bytes from " << std::hex << "0x" << start_vAddr << std::endl;
    auto data = elf.getData(start_vAddr, length);

    if(!data){
        std::cerr << "[!] Extraction failed: " << elf.getError() << std::endl;
        return 1;
    }

    std::span<const uint8_t> raw_bytes = *data;

    Disassembler disasm;
    if(!disasm.is_valid_instruction()) return 1;

    std::cout << "[*] Disassembling " << length << " bytes at 0x" << std::hex << start_vAddr << "\n";
    auto instructions = disasm.disassemble(raw_bytes, start_vAddr);

    if(instructions.empty()){
        std::cerr << "[!] No valid instructions decoded\n";
        return 1;
    }

    //save file
    std::ostream* outStream = &std::cout;
    std::ofstream fileStream;

    if(!outfile.empty()){
        fileStream.open(outfile);
        if(!fileStream.is_open()){
            std::cerr << "[!] Error: Could not open output file: " << outfile << "\n";
            return 1;
        }
        outStream = &fileStream;
        std::cout << "[*] Writing disassembly to " << outfile << "\n";
    }

    for(const auto& instr: instructions){
        *outStream << "0x" << std::hex << instr.address << ": " << instr.mnemonic << " " << instr.op_str << std::endl;
    }

    if(!outfile.empty()){
        std::cout << "[*] Done\n";
    }

    return 0;
}

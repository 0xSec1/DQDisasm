#include "ElfHandler.hpp"
#include <iomanip>
#include <iostream>

int main(int argc, char* argv[]){
    if(argc != 4){
        std::cerr << "Usage: " << argv[0] << " <elf_file> <virtual_addr> <length>\n";
        return 1;
    }

    std::string filename = argv[1];
    uint64_t vAddr = std::stoull(argv[2], nullptr, 16);
    size_t length = std::stoull(argv[3]);

    std::cout << "[*] Loading: " << filename << "..." << std::endl;
    ElfHandler elf(filename);

    if(!elf.isValid()){
        std::cerr << "[!] Error: " << elf.getError() << std::endl;
        return 1;
    }

    std::cout << "Extracting " << length << " bytes from " << std::hex << "0x" << vAddr << "..." << std::endl;
    auto data = elf.getData(vAddr, length);

    if(!data){
        std::cerr << "[!] Extraction failed: " << elf.getError() << std::endl;
        return 1;
    }

    //print dump
    std::cout << "[+] Data extacted" << std::endl;
    std::cout << std::hex << std::setfill('0');

    for(size_t i = 0; i < data->size(); i++){
        std::cout << std::setw(2) << (int)(*data)[i] << " ";
        if((i + 1) % 16 == 0){
            std::cout << "\n";
        }
    }
    std::cout << "\n";
    return 0;
}

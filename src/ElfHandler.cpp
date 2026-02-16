#include "ElfHandler.hpp"

ElfHandler::ElfHandler(const std::string& filepath){
    if(!reader.load(filepath)){
        errMsg = "Failed to load ELF: " + filepath;
        return;
    }
    validateElf();
}

void ElfHandler::validateElf(){
    if(reader.get_class() != ELFIO::ELFCLASS64){                                                //check x64 binary
        errMsg = "Unsupported: File is not a x64/x86 ELF";
        return;
    }

    if(reader.get_machine() != ELFIO::EM_X86_64){                                               //check x86_64 arch
        errMsg = "Unsupported: File is not x86_64";
        return;
    }
    valid_elf = true;
}

std::optional<std::span<const uint8_t>> ElfHandler::getData(uint64_t vAddr, size_t length){
    if(!valid_elf) return std::nullopt;

    for(const auto& segment: reader.segments){
        if(segment->get_type() != ELFIO::PT_LOAD) continue;                                     //only loadable segments

        uint64_t segmentStart = segment->get_virtual_address();
        uint64_t segmentSize = segment->get_file_size();

        if(vAddr >= segmentStart && vAddr < (segmentStart + segmentSize)){                      //if vAddr is within this segment
            uint64_t offset = vAddr - segmentStart;
            uint64_t availBytes = segmentSize - offset;

            if(length > availBytes){                                                            //check if length fit in segment
                errMsg = "Requested length exceeds segment bounds";
                return std::nullopt;
            }

            const char* rawPtr = segment->get_data();                                           //get ptr to data
            if(!rawPtr){
                errMsg = "Segment has no data";
                return std::nullopt;
            }

            return std::span<const uint8_t>(reinterpret_cast<const uint8_t*>( rawPtr + offset ), length);
        }
    }
    errMsg = "Virtual Address not found in any LOAD segment";
    return std::nullopt;
}

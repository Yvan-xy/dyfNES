//
// Created by root on 2020/5/1.
//

#ifndef DYFNES_ROMPARSER_H
#define DYFNES_ROMPARSER_H

#include <vector>
#include <string>
#include <cstdint>

namespace dyfNES {
    using Byte = std::uint8_t;
    using Address = std::uint16_t;

    class ROMParser {
    public:
        ROMParser();
        const std::vector<Byte>& getROM();
        const std::vector<Byte>& getVROM();
        bool loadRom(std::string path);

    private:
        std::vector<Byte> PRG_ROM;
        std::vector<Byte> CHR_ROM;
    };
}


#endif //DYFNES_ROMPARSER_H

//
// Created by root on 2020/5/1.
//

#include <log.h>
#include <fstream>
#include <romparser.h>

namespace dyfNES {

    dyfNES::ROMParser::ROMParser() {

    }


    const std::vector<Byte> &ROMParser::getROM() {
        return this->PRG_ROM;
    }

    const std::vector<Byte> &ROMParser::getVROM() {
        return this->CHR_ROM;
    }

    bool ROMParser::loadRom(std::string path) {
        std::ifstream romFile(path, std::ios_base::binary | std::ios_base::in);
        if (!romFile) {
            return false;
        }

        std::vector<Byte> header;
        LOG(Info) << "ROM path is " << path << std::endl;

        header.resize(16);
        if (!romFile.read(reinterpret_cast<char *>(&header[0]), 16)) {
            LOG(Error) << "Read NES Header Failed" << std::endl;
            return false;
        }

        if (std::string{&header[0], &header[4]} != "NES\x1A") {
            LOG(Error) << "File is not NES Format" << std::endl;
            return false;
        }

        int banks = header[4];
        LOG(Info) << "PRG-ROM Banks is " << banks << std::endl;

        int vbanks = header[5];
        LOG(Info) << "CHR-ROM Banks is " << vbanks << std::endl;

        int mapper = header[6];
        // TODO: flag 6

        if ((header[0xA] & 0x3) == 0x2 || (header[0xA] & 0x1)) {
            LOG(Error) << "PAL ROM not supported." << std::endl;
            return false;
        } else LOG(Info) << "ROM is NTSC compatible.\n";

        this->PRG_ROM.resize(0x4000 * banks);
        if (!romFile.read(reinterpret_cast<char *>(&PRG_ROM[0]), 0x4000 * banks)) {
            LOG(Error) << "Read PRG-ROM Failed" << std::endl;
            return false;
        }

        for (auto i : this->PRG_ROM) {
            std::cout << "0x" << std::hex << int(i) << " ";
        }
        std::cout<<std::endl;

        if (!vbanks)
            LOG(Info) << "Cartridge with CHR-RAM" << std::endl;

        this->CHR_ROM.resize(0x2000 * vbanks);
        if (!romFile.read(reinterpret_cast<char *>(&CHR_ROM[0]), 0x2000 * vbanks)) {
            LOG(Error) << "Read CHR-ROM Failed" << std::endl;
            return false;
        }

        return true;
    }
}

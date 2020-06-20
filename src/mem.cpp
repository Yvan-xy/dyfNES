//
// Created by root on 2020/5/2.
//

#include <log.h>
#include <mem.h>

namespace dyfNES {

    Mem::Mem() {
        this->data.resize(MEMORY_SIZE);
    }

    Mem::~Mem() {}

    /*
     *  @param std::vector<Byte> &PGR_ROM
     *
     *  Load PGR_ROM into CPU memory.
     *  If banks == 1 then load it into 0x8000 - 0xbffff
     *  and mirror it from 0xc000 to  0xffff
     */
    void Mem::loadPRGROM(std::vector<Byte> &PGR_ROM) {
        int banks = PGR_ROM.size() / 0x4000;
        for (int i = 0; i < PGR_ROM.size(); i++) {
            this->data[i + 0x8000] = PGR_ROM[i];
        }
        if (banks == 1) {
            for (int i = 0; i < PGR_ROM.size(); i++) {
                this->data[i + 0xc000] = PGR_ROM[i];
            }
        }

#ifdef CPU_LOG
//        for (int i = 0x8000; i < MEMORY_SIZE; i++) {
//            std::cout << "0x" << std::hex << int(this->data[i]) << " ";
//        }
//        std::cout << std::endl;
#endif

    }

    /**
     *
     * @param address
     * @return Byte
     *
    +---------+-------+-------+-----------------------+
    | addr    | size  | Flag  |         desc          |
    +---------+-------+-------+-----------------------+
    | $0000   | $800  |       | RAM                   |
    | $0800   | $800  | M     | RAM                   |
    | $1000   | $800  | M     | RAM                   |
    | $1800   | $800  | M     | RAM                   |
    | $2000   | 8     |       | Registers             |
    | $2008   | $1FF8 |  R    | Registers             |
    | $4000   | $20   |       | Registers             |
    | $4020   | $1FDF |       | Expansion ROM         |
    | $6000   | $2000 |       | SRAM                  |
    | $8000   | $4000 |       | PRG-ROM               |
    | $C000   | $4000 |       | PRG-ROM               |
    +---------+-------+-------+-----------------------+
     */
    Byte Mem::readByte(Address address) {
        // 0x0800 ~ 0x1fff is mirror of 0x0000 ~ 0x07ff
        // 0x2008 ~ 0x401f is mirror os 0x2000 ~ 0x2007
        Address addr = this->getRealAddress(address);

        if (addr < 0x2000) {
            return this->data[addr];
        } else {
//            LOG(Error) << "Not support read 0x" << std::hex << int(addr) << " yet!" << std::endl;
            return this->data[addr];
        }
        return 0;
    }

    Byte Mem::writeByte(Address address, Byte value) {
        Address addr = this->getRealAddress(address);
        if (addr < 0x2000) {
            this->data[addr] = value;
        } else {
//            LOG(Error) << "Not support write 0x" << std::hex << int(addr) << "yet!" << std::endl;
            this->data[addr] = value;
        }
        return 0;
    }

    Address Mem::getRealAddress(Address address) {
        if (address < 0x2000) {
            return (address & 0x7ff);
        } else if (address < 0x4000) {
            return (address & 0x2007);
        }
        return address;
    }

    Word Mem::readWord(Address address) {
        address = this->getRealAddress(address);
        return (this->data[address] | (this->data[address + 1] << 8));
    }

}

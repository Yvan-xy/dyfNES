//
// Created by root on 2020/5/2.
//

#ifndef DYFNES_MEM_H
#define DYFNES_MEM_H

#include <memory>
#include <vector>
#include <cstring>

namespace dyfNES {
    const int MEMORY_SIZE = 0x10000;

    using Byte = std::uint8_t;
    using Word = std::uint16_t;
    using Address = std::uint16_t;

    class Mem {
    public:
        Mem();

        ~Mem();

        void loadPRGROM(std::vector<Byte> &);

        Byte readByte(Address);

        Word readWord(Address);

        Byte writeByte(Address, Byte);

    private:
        std::vector<Byte> data;

        Address getRealAddress(Address);
    };
}


#endif //DYFNES_MEM_H

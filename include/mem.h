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
    using Address = std::uint16_t;

    class Mem {
    public:
        Mem();
        ~Mem();
        void loadPRGROM(std::vector<Byte> &);
    private:
        std::vector<Byte> data;
    };
}


#endif //DYFNES_MEM_H

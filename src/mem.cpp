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

        for (int i = 0x8000; i < MEMORY_SIZE; i++) {
            std::cout << "0x" << std::hex << int(this->data[i]) << " ";
        }
        std::cout << std::endl;
    }

}

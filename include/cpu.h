//
// Created by root on 2020/5/1.
//

#ifndef DYFNES_CPU_H
#define DYFNES_CPU_H

#include <log.h>
#include <mem.h>
#include <cstring>
#include <stdlib.h>
#include <opcode.h>
#include <romparser.h>

namespace dyfNES {
    enum Flag {
        C = 0, Z, I, D, B, _, V, N
    };
    enum IORegisters {
        PPUCTRL = 0x2000,
        PPUMASK,
        PPUSTATUS,
        OAMADDR,
        OAMDATA,
        PPUSCROL,
        PPUADDR,
        PPUDATA,
        OAMDMA = 0x4014,
        JOY1 = 0x4016,
        JOY2 = 0x4017,
    };

    class CPU {
    public:
        CPU(Mem &);

        void reset();

        Address readAddress(Address);

        void runOpcode(Byte opcode);

        uint16_t getOperand(OpcodeInfo &, Address &);

    private:
        /* Common register */
        Byte A{};
        Byte X{};
        Byte Y{};
        Byte PSW{};

        Byte SP{};    // Stack pointer
        Address PC{}; // Program counter

        Mem &mem;
    };
}


#endif //DYFNES_CPU_H

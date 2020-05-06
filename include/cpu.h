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

    enum InterruptType {
        InterruptTypeNone,  //
        InterruptTypeBreak, //
        InterruptTypeIRQs,  //
        InterruptTypeNMI,   //
        InterruptTypeReset  //
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

        // Set PSW
        inline void SetCarry(int flag) { PSW[C] = (flag != 0); };

        inline void SetDecimal(int flag) { PSW[D] = (flag != 0); };

        inline void SetInterrupt(int flag) { PSW[I] = (flag != 0); };

        inline void SetSign(int flag) { PSW[N] = (flag != 0); };

        inline void SetZero(int flag) { PSW[Z] = (flag != 0); };

        inline void SetOverflow(int flag) { PSW[V] = (flag != 0); };

        // Get PWS
        inline bool GetCarry() const { return PSW[C]; }

        inline bool GetDecimal() const { return PSW[D]; }

        inline bool GetInterrupt() const { return PSW[I]; }

        inline bool GetSign() const { return PSW[N]; }

        inline bool GetZero() const { return PSW[Z]; }

        inline bool GetOverflow() const { return PSW[V]; }

        // Fetch relative address
        inline Address RelativeAddress(Address base, Byte offset) { return base + int8_t(offset); };

        inline Address GetInterruptHandler(InterruptType);

        void pushStack(Byte);

        Byte popStack();

        void step();

        void Interrupt(InterruptType);

#ifdef CPU_LOG

        void info();

#endif

    private:
        /* Common register */
        Byte A{};
        Byte X{};
        Byte Y{};
        bool PSW[8];


        Byte SP{};    // Stack pointer
        Address PC{}; // Program counter

        Mem &mem;
    };
}


#endif //DYFNES_CPU_H

//
// Created by root on 2020/5/1.
//

#ifndef DYFNES_CPU_H
#define DYFNES_CPU_H

#include <cstring>
#include <stdlib.h>
#include <romparser.h>

namespace dyfNES {
    enum Flag {
        C = 0, Z, I, D, B, _, V, N
    };

    enum AddressingMode {
        Implied,        // No further operand needs to be specified.
        Immediate,      // Immediate addressing has an 8 bit constant operand.
        Accumulator,    // Just operate directly upon the accumulator A.

        Absolute,       // Absolute addressing mode has a 16 bit address operand (little endian).
        Absolute_X,     // Absolute address with X, has a 16 bit address operand (little endian).
        Absolute_Y,     // Absolute address with Y, has a 16 bit address operand (little endian).

        ZeroPage,       // Zero page addressing mode has only an 8 bit address operand.
        ZeroPage_X,     // Zero page with X, has only an 8 bit address operand.
        ZeroPage_Y,     // Zero page with Y, has only an 8 bit address operand.

        // Indirect addressing mode has a 16 bit address operand (little endian).
        // It will get another 16 bit address as the real target.
        // JMP ($FFFC)
        Indirect,

        // Indirect address with X, has an 8 bit address operand (little endian).
        // LDA ($3E, X)
        Indexed_Indirect_X,

        // Indirect address with Y, has an 8 bit address operand (little endian).
        // LDA ($4C), Y
        Indirect_Indexed_Y,
    };

    class CPU {
    public:
        CPU();

    private:
        /* Common register */
        Byte A;
        Byte X;
        Byte Y;
        Byte PSW;

        Byte SP;    // Stack pointer
        Address PC; // Program counter
    };
}


#endif //DYFNES_CPU_H

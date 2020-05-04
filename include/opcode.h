//DYFNES_OPCODE_H
//
// Created by root on 2020/5/3.
//

#ifndef DYFNES_OPCODE_H
#define DYFNES_OPCODE_H

#include <map>
#include <utility>

namespace dyfNES {
    using Byte = uint8_t;
    using Word = uint16_t;
    enum AddressingMode {
        Implied,        // No further operand needs to be specified.
        Immediate,      // Immediate addressing has an 8 bit constant operand.
        Accumulator,    // Just operate directly upon the accumulator A.

        Relative,       // Relative TODO: research

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
    enum TYPE {

        NON,

        BCC, BCS,
        BEQ,
        BMI, BNE, BPL, BVC, BVS,
        CLC, CLD, CLI, CLV,
        CMP, CPX, CPY,
        DEC, DEX, DEY,
        EOR,
        LDA, LDX, LDY,

        INX, INY, INC,

        ORA,

        SEC, SED, SEI,
        STA, STX, STY,
        TAX, TAY, TXA, TSX, TXS, TYA,

        BRK,

        ST,
        RTI,
        JSR,
        RTS,
        AND,
        JMP,

        DE,
        SBC,
        ADC,

        BIT,

        NOP,

        ASL, // 累加器寻址，算数左移
        LSR,

        PHA,
        PHP,
        PLA, PLP,
        ROL,
        ROR,
    };

    class OpcodeInfo {

    public:
        OpcodeInfo() = default;;

        OpcodeInfo(std::string name, TYPE type, AddressingMode mode, int opcodeLength, int cycles, int psw) :
                name(std::move(name)), mode(mode), type(type), opcodeLength(opcodeLength), cycles(cycles), psw(psw) {}

        std::string name;
        TYPE type;
        AddressingMode mode;

        int opcodeLength{};
        int cycles{};
        Byte psw{};
    };


    const std::map<Byte, OpcodeInfo> opcodeList = {

            /* (immidiate) ADC #oper */
            {0x69, {"ADC", ADC, Immediate,          2, 2, 195}},
            /* (zeropage) ADC oper */
            {0x65, {"ADC", ADC, ZeroPage,           2, 3, 195}},
            /* (zeropage,X) ADC oper,X */
            {0x75, {"ADC", ADC, ZeroPage_X,         2, 4, 195}},
            /* (absolute) ADC oper */
            {0x6D, {"ADC", ADC, Absolute,           3, 4, 195}},
            /* (absolute,X) ADC oper,X */
            {0x7D, {"ADC", ADC, Absolute_X,         3, 4, 195}},
            /* (absolute,Y) ADC oper,Y */
            {0x79, {"ADC", ADC, Absolute_Y,         3, 4, 195}},
            /* ((indirect,X)) ADC (oper,X) */
            {0x61, {"ADC", ADC, Indexed_Indirect_X, 2, 6, 195}},
            /* ((indirect),Y) ADC (oper),Y */
            {0x71, {"ADC", ADC, Indirect_Indexed_Y, 2, 5, 195}},
            /* (immidiate) AND #oper */
            {0x29, {"AND", AND, Immediate,          2, 2, 130}},
            /* (zeropage) AND oper */
            {0x25, {"AND", AND, ZeroPage,           2, 3, 130}},
            /* (zeropage,X) AND oper,X */
            {0x35, {"AND", AND, ZeroPage_X,         2, 4, 130}},
            /* (absolute) AND oper */
            {0x2D, {"AND", AND, Absolute,           3, 4, 130}},
            /* (absolute,X) AND oper,X */
            {0x3D, {"AND", AND, Absolute_X,         3, 4, 130}},
            /* (absolute,Y) AND oper,Y */
            {0x39, {"AND", AND, Absolute_Y,         3, 4, 130}},
            /* ((indirect,X)) AND (oper,X) */
            {0x21, {"AND", AND, Indexed_Indirect_X, 2, 6, 130}},
            /* ((indirect),Y) AND (oper),Y */
            {0x31, {"AND", AND, Indirect_Indexed_Y, 2, 5, 130}},
            /* (accumulator) ASL A */
            {0x0A, {"ASL", ASL, Accumulator,        1, 2, 131}},
            /* (zeropage) ASL oper */
            {0x06, {"ASL", ASL, ZeroPage,           2, 5, 131}},
            /* (zeropage,X) ASL oper,X */
            {0x16, {"ASL", ASL, ZeroPage_X,         2, 6, 131}},
            /* (absolute) ASL oper */
            {0x0E, {"ASL", ASL, Absolute,           3, 6, 131}},
            /* (absolute,X) ASL oper,X */
            {0x1E, {"ASL", ASL, Absolute_X,         3, 7, 131}},
            /* (relative) BCC oper */
            {0x90, {"BCC", BCC, Relative,           2, 2, 0}},
            /* (relative) BCS oper */
            {0xB0, {"BCS", BCS, Relative,           2, 2, 0}},
            /* (relative) BEQ oper */
            {0xF0, {"BEQ", BEQ, Relative,           2, 2, 0}},
            /* (zeropage) BIT oper */
            {0x24, {"BIT", BIT, ZeroPage,           2, 3, 2}},
            /* (absolute) BIT oper */
            {0x2C, {"BIT", BIT, Absolute,           3, 4, 2}},
            /* (relative) BMI oper */
            {0x30, {"BMI", BMI, Relative,           2, 2, 0}},
            /* (relative) BNE oper */
            {0xD0, {"BNE", BNE, Relative,           2, 2, 0}},
            /* (relative) BPL oper */
            {0x10, {"BPL", BPL, Relative,           2, 2, 0}},
            /* (implied) BRK */
            {0x00, {"BRK", BRK, Implied,            1, 7, 0}},
            /* (relative) BVC oper */
            {0x50, {"BVC", BVC, Relative,           2, 2, 0}},
            /* (relative) BVC oper */
            {0x70, {"BVS", BVS, Relative,           2, 2, 0}},
            /* (implied) CLC */
            {0x18, {"CLC", CLC, Implied,            1, 2, 0}},
            /* (implied) CLD */
            {0xD8, {"CLD", CLD, Implied,            1, 2, 0}},
            /* (implied) CLI */
            {0x58, {"CLI", CLI, Implied,            1, 2, 0}},
            /* (implied) CLV */
            {0xB8, {"CLV", CLV, Implied,            1, 2, 0}},
            /* (immidiate) CMP #oper */
            {0xC9, {"CMP", CMP, Immediate,          2, 2, 131}},
            /* (zeropage) CMP oper */
            {0xC5, {"CMP", CMP, ZeroPage,           2, 3, 131}},
            /* (zeropage,X) CMP oper,X */
            {0xD5, {"CMP", CMP, ZeroPage_X,         2, 4, 131}},
            /* (absolute) CMP oper */
            {0xCD, {"CMP", CMP, Absolute,           3, 4, 131}},
            /* (absolute,X) CMP oper,X */
            {0xDD, {"CMP", CMP, Absolute_X,         3, 4, 131}},
            /* (absolute,Y) CMP oper,Y */
            {0xD9, {"CMP", CMP, Absolute_Y,         3, 4, 131}},
            /* ((indirect,X)) CMP (oper,X) */
            {0xC1, {"CMP", CMP, Indexed_Indirect_X, 2, 6, 131}},
            /* ((indirect),Y) CMP (oper),Y */
            {0xD1, {"CMP", CMP, Indirect_Indexed_Y, 2, 5, 131}},
            /* (immidiate) CPX #oper */
            {0xE0, {"CPX", CPX, Immediate,          2, 2, 131}},
            /* (zeropage) CPX oper */
            {0xE4, {"CPX", CPX, ZeroPage,           2, 3, 131}},
            /* (absolute) CPX oper */
            {0xEC, {"CPX", CPX, Absolute,           3, 4, 131}},
            /* (immidiate) CPY #oper */
            {0xC0, {"CPY", CPY, Immediate,          2, 2, 131}},
            /* (zeropage) CPY oper */
            {0xC4, {"CPY", CPY, ZeroPage,           2, 3, 131}},
            /* (absolute) CPY oper */
            {0xCC, {"CPY", CPY, Absolute,           3, 4, 131}},
            /* (zeropage) DEC oper */
            {0xC6, {"DEC", DEC, ZeroPage,           2, 5, 130}},
            /* (zeropage,X) DEC oper,X */
            {0xD6, {"DEC", DEC, ZeroPage_X,         2, 6, 130}},
            /* (absolute) DEC oper */
            {0xCE, {"DEC", DEC, Absolute,           3, 3, 130}},
            /* (absolute,X) DEC oper,X */
            {0xDE, {"DEC", DEC, Absolute_X,         3, 7, 130}},
            /* (implied) DEX */
            {0xCA, {"DEX", DEX, Implied,            1, 2, 130}},
            /* (implied) DEY */
            {0x88, {"DEY", DEY, Implied,            1, 2, 130}},
            /* (immidiate) EOR #oper */
            {0x49, {"EOR", EOR, Immediate,          2, 2, 130}},
            /* (zeropage) EOR oper */
            {0x45, {"EOR", EOR, ZeroPage,           2, 3, 130}},
            /* (zeropage,X) EOR oper,X */
            {0x55, {"EOR", EOR, ZeroPage_X,         2, 4, 130}},
            /* (absolute) EOR oper */
            {0x4D, {"EOR", EOR, Absolute,           3, 4, 130}},
            /* (absolute,X) EOR oper,X */
            {0x5D, {"EOR", EOR, Absolute_X,         3, 4, 130}},
            /* (absolute,Y) EOR oper,Y */
            {0x59, {"EOR", EOR, Absolute_Y,         3, 4, 130}},
            /* ((indirect,X)) EOR (oper,X) */
            {0x41, {"EOR", EOR, Indexed_Indirect_X, 2, 6, 130}},
            /* ((indirect),Y) EOR (oper),Y */
            {0x51, {"EOR", EOR, Indirect_Indexed_Y, 2, 5, 130}},
            /* (zeropage) INC oper */
            {0xE6, {"INC", INC, ZeroPage,           2, 5, 130}},
            /* (zeropage,X) INC oper,X */
            {0xF6, {"INC", INC, ZeroPage_X,         2, 6, 130}},
            /* (absolute) INC oper */
            {0xEE, {"INC", INC, Absolute,           3, 6, 130}},
            /* (absolute,X) INC oper,X */
            {0xFE, {"INC", INC, Absolute_X,         3, 7, 130}},
            /* (implied) INX */
            {0xE8, {"INX", INX, Implied,            1, 2, 130}},
            /* (implied) INY */
            {0xC8, {"INY", INY, Implied,            1, 2, 130}},
            /* (absolute) JMP oper */
            {0x4C, {"JMP", JMP, Absolute,           3, 3, 0}},
            /* (indirect) JMP (oper) */
            {0x6C, {"JMP", JMP, Indirect,           3, 5, 0}},
            /* (absolute) JSR oper */
            {0x20, {"JSR", JSR, Absolute,           3, 6, 0}},
            /* (immidiate) LDA #oper */
            {0xA9, {"LDA", LDA, Immediate,          2, 2, 130}},
            /* (zeropage) LDA oper */
            {0xA5, {"LDA", LDA, ZeroPage,           2, 3, 130}},
            /* (zeropage,X) LDA oper,X */
            {0xB5, {"LDA", LDA, ZeroPage_X,         2, 4, 130}},
            /* (absolute) LDA oper */
            {0xAD, {"LDA", LDA, Absolute,           3, 4, 130}},
            /* (absolute,X) LDA oper,X */
            {0xBD, {"LDA", LDA, Absolute_X,         3, 4, 130}},
            /* (absolute,Y) LDA oper,Y */
            {0xB9, {"LDA", LDA, Absolute_Y,         3, 4, 130}},
            /* ((indirect,X)) LDA (oper,X) */
            {0xA1, {"LDA", LDA, Indexed_Indirect_X, 2, 6, 130}},
            /* ((indirect),Y) LDA (oper),Y */
            {0xB1, {"LDA", LDA, Indirect_Indexed_Y, 2, 5, 130}},
            /* (immidiate) LDX #oper */
            {0xA2, {"LDX", LDX, Immediate,          2, 2, 130}},
            /* (zeropage) LDX oper */
            {0xA6, {"LDX", LDX, ZeroPage,           2, 3, 130}},
            /* (zeropage,Y) LDX oper,Y */
            {0xB6, {"LDX", LDX, ZeroPage_Y,         2, 4, 130}},
            /* (absolute) LDX oper */
            {0xAE, {"LDX", LDX, Absolute,           3, 4, 130}},
            /* (absolute,Y) LDX oper,Y */
            {0xBE, {"LDX", LDX, Absolute_Y,         3, 4, 130}},
            /* (immidiate) LDY #oper */
            {0xA0, {"LDY", LDY, Immediate,          2, 2, 130}},
            /* (zeropage) LDY oper */
            {0xA4, {"LDY", LDY, ZeroPage,           2, 3, 130}},
            /* (zeropage,X) LDY oper,X */
            {0xB4, {"LDY", LDY, ZeroPage_X,         2, 4, 130}},
            /* (absolute) LDY oper */
            {0xAC, {"LDY", LDY, Absolute,           3, 4, 130}},
            /* (absolute,X) LDY oper,X */
            {0xBC, {"LDY", LDY, Absolute_X,         3, 4, 130}},
            /* (accumulator) LSR A */
            {0x4A, {"LSR", LSR, Accumulator,        1, 2, 3}},
            /* (zeropage) LSR oper */
            {0x46, {"LSR", LSR, ZeroPage,           2, 5, 3}},
            /* (zeropage,X) LSR oper,X */
            {0x56, {"LSR", LSR, ZeroPage_X,         2, 6, 3}},
            /* (absolute) LSR oper */
            {0x4E, {"LSR", LSR, Absolute,           3, 6, 3}},
            /* (absolute,X) LSR oper,X */
            {0x5E, {"LSR", LSR, Absolute_X,         3, 7, 3}},
            /* (implied) NOP */
            {0xEA, {"NOP", NOP, Implied,            1, 2, 0}},
            /* (immidiate) ORA #oper */
            {0x09, {"ORA", ORA, Immediate,          2, 2, 130}},
            /* (zeropage) ORA oper */
            {0x05, {"ORA", ORA, ZeroPage,           2, 3, 130}},
            /* (zeropage,X) ORA oper,X */
            {0x15, {"ORA", ORA, ZeroPage_X,         2, 4, 130}},
            /* (absolute) ORA oper */
            {0x0D, {"ORA", ORA, Absolute,           3, 4, 130}},
            /* (absolute,X) ORA oper,X */
            {0x1D, {"ORA", ORA, Absolute_X,         3, 4, 130}},
            /* (absolute,Y) ORA oper,Y */
            {0x19, {"ORA", ORA, Absolute_Y,         3, 4, 130}},
            /* ((indirect,X)) ORA (oper,X) */
            {0x01, {"ORA", ORA, Indexed_Indirect_X, 2, 6, 130}},
            /* ((indirect),Y) ORA (oper),Y */
            {0x11, {"ORA", ORA, Indirect_Indexed_Y, 2, 5, 130}},
            /* (implied) PHA */
            {0x48, {"PHA", PHA, Implied,            1, 3, 0}},
            /* (implied) PHP */
            {0x08, {"PHP", PHP, Implied,            1, 3, 0}},
            /* (implied) PLA */
            {0x68, {"PLA", PLA, Implied,            1, 4, 130}},
            /* (implied) PHP */
            {0x28, {"PLP", PLP, Implied,            1, 4, 0}},
            /* (accumulator) ROL A */
            {0x2A, {"ROL", ROL, Accumulator,        1, 2, 131}},
            /* (zeropage) ROL oper */
            {0x26, {"ROL", ROL, ZeroPage,           2, 5, 131}},
            /* (zeropage,X) ROL oper,X */
            {0x36, {"ROL", ROL, ZeroPage_X,         2, 6, 131}},
            /* (absolute) ROL oper */
            {0x2E, {"ROL", ROL, Absolute,           3, 6, 131}},
            /* (absolute,X) ROL oper,X */
            {0x3E, {"ROL", ROL, Absolute_X,         3, 7, 131}},
            /* (accumulator) ROR A */
            {0x6A, {"ROR", ROR, Accumulator,        1, 2, 131}},
            /* (zeropage) ROR oper */
            {0x66, {"ROR", ROR, ZeroPage,           2, 5, 131}},
            /* (zeropage,X) ROR oper,X */
            {0x76, {"ROR", ROR, ZeroPage_X,         2, 6, 131}},
            /* (absolute) ROR oper */
            {0x6E, {"ROR", ROR, Absolute,           3, 6, 131}},
            /* (absolute,X) ROR oper,X */
            {0x7E, {"ROR", ROR, Absolute_X,         3, 7, 131}},
            /* (implied) RTI */
            {0x40, {"RTI", RTI, Implied,            1, 6, 0}},
            /* (implied) RTS */
            {0x60, {"RTS", RTS, Implied,            1, 6, 0}},
            /* (immidiate) SBC #oper */
            {0xE9, {"SBC", SBC, Immediate,          2, 2, 195}},
            /* (zeropage) SBC oper */
            {0xE5, {"SBC", SBC, ZeroPage,           2, 3, 195}},
            /* (zeropage,X) SBC oper,X */
            {0xF5, {"SBC", SBC, ZeroPage_X,         2, 4, 195}},
            /* (absolute) SBC oper */
            {0xED, {"SBC", SBC, Absolute,           3, 4, 195}},
            /* (absolute,X) SBC oper,X */
            {0xFD, {"SBC", SBC, Absolute_X,         3, 4, 195}},
            /* (absolute,Y) SBC oper,Y */
            {0xF9, {"SBC", SBC, Absolute_Y,         3, 4, 195}},
            /* ((indirect,X)) SBC (oper,X) */
            {0xE1, {"SBC", SBC, Indexed_Indirect_X, 2, 6, 195}},
            /* ((indirect),Y) SBC (oper),Y */
            {0xF1, {"SBC", SBC, Indirect_Indexed_Y, 2, 5, 195}},
            /* (implied) SEC */
            {0x38, {"SEC", SEC, Implied,            1, 2, 0}},
            /* (implied) SED */
            {0xF8, {"SED", SED, Implied,            1, 2, 0}},
            /* (implied) SEI */
            {0x78, {"SEI", SEI, Implied,            1, 2, 0}},
            /* (zeropage) STA oper */
            {0x85, {"STA", STA, ZeroPage,           2, 3, 0}},
            /* (zeropage,X) STA oper,X */
            {0x95, {"STA", STA, ZeroPage_X,         2, 4, 0}},
            /* (absolute) STA oper */
            {0x8D, {"STA", STA, Absolute,           3, 4, 0}},
            /* (absolute,X) STA oper,X */
            {0x9D, {"STA", STA, Absolute_X,         3, 5, 0}},
            /* (absolute,Y) STA oper,Y */
            {0x99, {"STA", STA, Absolute_Y,         3, 5, 0}},
            /* ((indirect,X)) STA (oper,X) */
            {0x81, {"STA", STA, Indexed_Indirect_X, 2, 6, 0}},
            /* ((indirect),Y) STA (oper),Y */
            {0x91, {"STA", STA, Indirect_Indexed_Y, 2, 6, 0}},
            /* (zeropage) STX oper */
            {0x86, {"STX", STX, ZeroPage,           2, 3, 0}},
            /* (zeropage,Y) STX oper,Y */
            {0x96, {"STX", STX, ZeroPage_Y,         2, 4, 0}},
            /* (absolute) STX oper */
            {0x8E, {"STX", STX, Absolute,           3, 4, 0}},
            /* (zeropage) STY oper */
            {0x84, {"STY", STY, ZeroPage,           2, 3, 0}},
            /* (zeropage,X) STY oper,X */
            {0x94, {"STY", STY, ZeroPage_X,         2, 4, 0}},
            /* (absolute) STY oper */
            {0x8C, {"STY", STY, Absolute,           3, 4, 0}},
            /* (implied) TAX */
            {0xAA, {"TAX", TAX, Implied,            1, 2, 130}},
            /* (implied) TAY */
            {0xA8, {"TAY", TAY, Implied,            1, 2, 130}},
            /* (implied) TSX */
            {0xBA, {"TSX", TSX, Implied,            1, 2, 130}},
            /* (implied) TXA */
            {0x8A, {"TXA", TXA, Implied,            1, 2, 130}},
            /* (implied) TXS */
            {0x9A, {"TXS", TXS, Implied,            1, 2, 130}},
            /* (implied) TYA */
            {0x98, {"TYA", TYA, Implied,            1, 2, 130}}
    };


}

#endif //DYFNES_OPCODE_H

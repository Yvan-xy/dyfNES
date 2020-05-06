//
// Created by root on 2020/5/1.
//

#include <cpu.h>
#include <string.h>
#include <assert.h>

namespace dyfNES {

    CPU::CPU(Mem &_mem) : mem(_mem) {
        this->A = 0;
        this->X = this->Y = this->PC = 0;
        PSW[I] = true;
        PSW[C] = PSW[Z] = PSW[D] = PSW[B] = PSW[V] = PSW[N] = false;
        this->SP = 0xfD;
    }

    void CPU::reset() {
        this->PC = GetInterruptHandler(InterruptTypeReset);
    }

    Address CPU::readAddress(Address address) {
        Address addr = this->mem.readByte(address) | this->mem.readByte(address + 1) << 8;
        return addr;
    }

    void CPU::pushStack(Byte value) {
        this->mem.writeByte(0x100 | SP, value);
        --SP;
    }

    Byte CPU::popStack() {
        return this->mem.readByte(0x100 | ++SP);
    }

    void CPU::runOpcode(Byte opcode) {
        OpcodeInfo opcodeInfo;
        Word operand;
        Address address = 0;
        auto iter = opcodeList.find(opcode);
        if (iter != opcodeList.end())
            opcodeInfo = iter->second;

        TYPE type = opcodeInfo.type;
        AddressingMode mode = opcodeInfo.mode;
        operand = this->getOperand(opcodeInfo, address);


        switch (type) {
            case ADC: {     // ADC -> A + M + C
                operand &= this->A;
                Word sum = this->A + operand + this->PSW[C];
                SetCarry(sum & 0x100);

                //SIGNED overflow, would only happen if the sign of sum is
                //different from BOTH the operands
                SetOverflow((this->A ^ sum) & (operand ^ sum) & 0x80);
                this->A = static_cast<Byte>(sum);
                break;
            }
            case AND: {
                this->A |= operand;
                SetZero(!A);
                SetSign(A & 0x80);
                break;
            }
            case ASL: {
                if (mode == Accumulator) {
                    this->PSW[C] = operand & 0x80;
                    this->A <<= 1;
                    SetZero(!A);
                    SetSign(A & 0x80);
                } else {
                    this->PSW[C] = operand & 0x80;
                    operand = operand << 1;
                    this->mem.writeByte(address, operand);
                    SetZero(!operand);
                    SetSign(operand & 0x80);
                }
                break;
            }
            case BCC: {
                if (!GetCarry()) {
                    PC = RelativeAddress(PC, operand);
                }
                break;
            }
            case BCS: {
                if (GetCarry()) {
                    PC = RelativeAddress(PC, operand);
                }
                break;
            }
            case BEQ: {
                if (GetZero()) {
                    PC = RelativeAddress(PC, operand);
                }
                break;
            }
            case BIT: {
                SetZero(!(A & operand));
                SetOverflow(operand & 0x40);
                SetSign(operand & 0x80);
                break;
            }
            case BMI: {
                if (GetSign()) {
                    PC = RelativeAddress(PC, operand);
                }
                break;
            }
            case BNE: {
                if (!GetZero()) {
                    PC = RelativeAddress(PC, operand);
                }
                break;
            }
            case BPL: {
                if (!GetSign()) {
                    PC = RelativeAddress(PC, operand);
                }
                break;
            }
            case BRK: {
                Interrupt(InterruptTypeBreak);
                break;
            }
            case BVC: {
                if (!GetOverflow()) {
                    PC = RelativeAddress(PC, operand);
                }
                break;
            }
            case BVS: {
                if (GetOverflow()) {
                    PC = RelativeAddress(PC, operand);
                }
                break;
            }
            case CLC: {
                SetCarry(0);
                break;
            }
            case CLD: {
                SetDecimal(0);
                break;
            }
            case CLI: {
                SetInterrupt(0);
                break;
            }
            case CLV: {
                SetOverflow(0);
                break;
            }
            case CMP: {
                Word diff = A - operand;
                SetCarry(!(diff & 0x100));
                SetZero(!diff);
                SetSign(diff & 0x80);
                break;
            }
            case CPX: {
                Word diff = X - operand;
                SetCarry(!(diff & 0x100));
                SetZero(!diff);
                SetSign(diff & 0x80);
                break;
            }
            case CPY: {
                Word diff = Y - operand;
                SetCarry(!(diff & 0x100));
                SetZero(!diff);
                SetSign(diff & 0x80);
                break;
            }
            case DEC: {
                operand -= 1;
                SetZero(!operand);
                SetSign(operand & 0x80);
                this->mem.writeByte(address, operand);
                break;
            }
            case DEX: {
                --X;
                SetZero(!X);
                SetSign(X & 0x80);
                break;
            }
            case DEY: {
                --Y;
                SetZero(!Y);
                SetSign(Y & 0x80);
                break;
            }
            case EOR: {
                A ^= operand;
                SetZero(!A);
                SetSign(A & 0x80);
                break;
            }
            case INC: {
                ++operand;
                SetZero(!operand);
                SetSign(operand & 0x80);
                this->mem.writeByte(address, operand);
                break;
            }
            case INX: {
                ++X;
                SetZero(!X);
                SetSign(X & 0x80);
                break;
            }
            case INY: {
                ++Y;
                SetZero(!Y);
                SetSign(Y & 0x80);
                break;
            }
            case JMP: {
                PC = address;
                break;
            }
            case JSR: {
                pushStack(static_cast<Byte>((PC - 1) >> 8));
                pushStack(static_cast<Byte>(PC - 1));
                break;
            }
            case LDA: {
                A = operand;
                SetZero(!A);
                SetSign(A & 0x80);
                break;
            }
            case LDX: {
                X = operand;
                SetZero(!X);
                SetSign(X & 0x80);
                break;
            }
            case LDY: {
                Y = operand;
                SetZero(!Y);
                SetSign(Y & 0x80);
                break;
            }
            case LSR: {
                if (mode == Accumulator) {
                    PSW[C] = operand & 1;
                    A >>= 1;
                    SetZero(!A);
                    SetSign(A & 0x80);
                } else {
                    PSW[C] = operand & 1;
                    operand >>= 1;
                    SetZero(!operand);
                    SetSign(operand & 0x80);
                    this->mem.writeByte(address, operand);
                }
                break;
            }
            case NOP: {
                break;
            }
            case ORA: {
                A |= operand;
                SetZero(!A);
                SetSign(A & 0x80);
                break;
            }
            case PHA: {
                pushStack(A);
                break;
            }
            case PHP: {
                Byte flags = PSW[N] << 7 |
                             PSW[V] << 6 |
                             1 << 5 | //supposed to always be 1
                             1 << 4 | //PHP pushes with the B flag as 1, no matter what
                             PSW[D] << 3 |
                             PSW[I] << 2 |
                             PSW[Z] << 1 |
                             PSW[C];
                pushStack(flags);
                break;
            }
            case PLA: {
                A = popStack();
                SetZero(!A);
                SetSign(A & 0x80);
                break;
            }
            case PLP: {
                Byte flags = popStack();
                PSW[N] = flags & 0x80;
                PSW[V] = flags & 0x40;
                PSW[D] = flags & 0x8;
                PSW[I] = flags & 0x4;
                PSW[Z] = flags & 0x2;
                PSW[C] = flags & 0x1;
                break;
            }
            case ROL: {
                if (mode == Accumulator) {
                    auto carry = PSW[C];
                    PSW[C] = operand & 0x80;
                    A <<= 1;
                    A = A | carry;
                } else {
                    auto carry = PSW[C];
                    PSW[C] = operand & 0x80;
                    operand <<= 1;
                    operand = operand | carry;
                    this->mem.writeByte(address, operand);
                }
                break;
            }
            case ROR: {
                if (mode == Accumulator) {
                    auto carry = PSW[C];
                    PSW[C] = operand & 1;
                    A >>= 1;
                    A = A | (carry << 7);
                } else {
                    auto carry = PSW[C];
                    PSW[C] = operand & 1;
                    operand >>= 1;
                    operand = operand | (carry << 7);
                    this->mem.writeByte(address, operand);
                }
                break;
            }
            case RTI: {
                Byte flags = popStack();
                PSW[N] = flags & 0x80;
                PSW[V] = flags & 0x40;
                PSW[D] = flags & 0x8;
                PSW[I] = flags & 0x4;
                PSW[Z] = flags & 0x2;
                PSW[C] = flags & 0x1;

                PC = popStack();
                PC |= popStack() << 8;
                break;
            }
            case RTS: {
                PC = popStack();
                PC |= popStack() << 8;
                ++PC;
            }
            case SBC: {
                Word diff = A - operand - !PSW[C];
                PSW[C] = !(diff & 0x100);
                PSW[V] = (A ^ diff) & (~operand ^ diff) & 0x80;
                SetZero(!diff);
                SetSign(diff & 0x80);
                A = static_cast<Byte>(diff);
                break;
            }
            case SEC: {
                SetCarry(1);
                break;
            }
            case SED: {
                SetDecimal(1);
                break;
            }
            case SEI: {
                SetInterrupt(1);
            }
            case STA: {
                LOG(Info) << "Address in sta -> " << address << std::endl;
                this->mem.writeByte(address, A);
                break;
            }
            case STX: {
                this->mem.writeByte(address, X);
                break;
            }
            case STY: {
                this->mem.writeByte(address, Y);
                break;
            }
            case TAX: {
                X = A;
                SetZero(!X);
                SetSign(X & 0x80);
                break;
            }
            case TAY: {
                Y = A;
                SetZero(!Y);
                SetSign(Y & 0x80);
                break;
            }
            case TSX: {
                X = SP;
                SetZero(!X);
                SetSign(X & 0x80);
                break;
            }
            case TXA: {
                A = X;
                SetZero(!A);
                SetSign(A & 0x80);
            }
            case TXS: {
                SP = X;
                break;
            }
            case TYA: {
                A = Y;
                SetZero(!A);
                SetSign(A & 0x80);
                break;
            }
        }
#ifdef CPU_LOG
        int psw = PSW[N] << 7 |
                  PSW[V] << 6 |
                  1 << 5 |
                  PSW[D] << 3 |
                  PSW[I] << 2 |
                  PSW[Z] << 1 |
                  PSW[C];
        this->info();
        LOG(Info) << "\nName -> " << opcodeInfo.name << "\n"
                  << "opcode -> 0x" << std::hex << int(opcode) << "\n"
                  << "opcodeLength -> " << opcodeInfo.opcodeLength << "\n"
                  << "operand -> 0x" << std::hex << int(operand) << "\n"
                  << "address -> 0x" << std::hex << int(address) << "\n"
                  << "flags -> " << psw << "\n" << std::endl;
#endif
    }

    uint16_t CPU::getOperand(OpcodeInfo &opcodeInfo, Address &address) {
        AddressingMode mode = opcodeInfo.mode;
        Byte operand = 0;
        switch (mode) {
            case Implied: {
                break;
            }
            case Immediate: {
                address = this->PC;
                operand = this->mem.readByte(this->PC);
                this->PC++;
                break;
            }
            case Accumulator: {
                operand = this->A;
                break;
            }
            case Relative: {
                address = this->PC;
                operand = this->mem.readByte(this->PC);
                this->PC++;
                break;
            }
            case Absolute: {
                address = this->mem.readWord(this->PC);
                operand = this->mem.readByte(address);
                this->PC += 2;
                break;
            }
            case Absolute_X: {
                address = this->mem.readWord(this->PC) + this->X;
                operand = this->mem.readByte(address);
                this->PC += 2;
                break;
            }
            case Absolute_Y: {
                address = this->mem.readWord(this->PC) + this->Y;
                operand = this->mem.readByte(address);
                this->PC += 2;
                break;
            }
            case ZeroPage: {
                address = this->mem.readByte(this->PC);
                operand = this->mem.readByte(address);
                this->PC++;
                break;
            }
            case ZeroPage_X: {
                address = (this->mem.readByte(this->PC) + this->X) % 0x100;
                operand = this->mem.readByte(address);
                this->PC++;
                break;
            }
            case ZeroPage_Y: {
                address = (this->mem.readByte(this->PC) + this->Y) % 0x100;
                operand = this->mem.readByte(address);
                this->PC++;
                break;
            }
            case Indirect: {
                Address idx = this->mem.readWord(this->PC);
                if ((idx & 0xff) == 0xff && opcodeInfo.name == "JMP") {
                    address = this->mem.readByte(this->PC) | (this->mem.readByte(this->PC - 0xff) << 8);
                } else {
                    address = this->mem.readWord(idx);
                }
                operand = this->mem.readByte(address);
                this->PC += 2;
                break;
            }
            case Indexed_Indirect_X: {
                Address idx = this->mem.readByte(this->PC) + this->X;
                address = this->mem.readWord(idx % 0x100);
                operand = this->mem.readByte(address);
                this->PC++;
                break;
            }
            case Indirect_Indexed_Y: {
                Address idx = this->mem.readWord(this->PC);
                address = this->mem.readWord(idx) + this->Y;
                operand = this->mem.readByte(address);
                this->PC++;
                break;
            }
            default: {
                assert(!"Wrong Address Method!");
                break;
            }
        }
        return operand;
    }

    Address CPU::GetInterruptHandler(InterruptType interruptType) {
        const static std::map<int, uint16_t> handler = {
                {InterruptTypeNMI,   0xFFFA},
                {InterruptTypeReset, 0xFFFC},
                {InterruptTypeIRQs,  0xFFFE},
                {InterruptTypeBreak, 0xFFFE},
        };

        const Address addr = handler.at(interruptType);
        Address interruptHandlerAddr = this->readAddress(addr);
        return interruptHandlerAddr;
    }

    void CPU::step() {
        Byte opcode = this->mem.readByte(PC++);
        runOpcode(opcode);
    }


#ifdef CPU_LOG

    void CPU::info() {
        LOG(Info) << "---- CPU INFO ----\n"
                  << "PC -> 0x" << std::hex << int(PC) << "\n"
                  << "SP -> 0x" << std::hex << int(SP) << "\n"
                  << "A -> 0x" << std::hex << int(A) << "\n"
                  << "X -> 0x" << std::hex << int(X) << "\n"
                  << "Y -> 0x" << std::hex << int(Y) << "\n"
                  << "C -> " << std::hex << int(PSW[C]) << "\n"
                  << "Z -> " << std::hex << int(PSW[Z]) << "\n"
                  << "I -> " << std::hex << int(PSW[I]) << "\n"
                  << "D -> " << std::hex << int(PSW[D]) << "\n"
                  << "B -> " << std::hex << int(PSW[B]) << "\n"
                  << "V -> " << std::hex << int(PSW[V]) << "\n"
                  << "N -> " << std::hex << int(PSW[N]) << std::endl;
    }


#endif

    void CPU::Interrupt(InterruptType interruptType) {
        if (PSW[I] && interruptType != InterruptTypeBreak && interruptType != InterruptTypeNMI)
            return;

        if (interruptType == InterruptTypeBreak)
            this->PC++;     // //Add one if BRK, a bug(feature ^_^) of 6502

        pushStack(PC >> 8);
        pushStack(PC);

        Byte flag = PSW[N] << 7 |
                    PSW[V] << 6 |
                    1 << 5 | // unused bit, supposed to be always 1
                    (interruptType == InterruptTypeBreak) << 4 | // B flag set if BRK
                    PSW[D] << 3 |
                    PSW[I] << 2 |
                    PSW[Z] << 1 |
                    PSW[C];
        pushStack(flag);

        PSW[I] = true;
        this->PC = GetInterruptHandler(interruptType);
    }
}

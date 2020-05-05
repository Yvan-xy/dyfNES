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
        this->SP = 0;
    }

    void CPU::reset() {
        this->PC = GetInterruptHandler(InterruptTypeReset);
    }

    Address CPU::readAddress(Address address) {
        Address addr = this->mem.readByte(address) | this->mem.readByte(address + 1) << 8;
        return addr;
    }

    void CPU::runOpcode(Byte opcode) {
        OpcodeInfo opcodeInfo;
        Word operand;
        Address address = 0;
        auto iter = opcodeList.find(opcode);
        if (iter != opcodeList.end())
            opcodeInfo = iter->second;

#ifdef CPU_LOG
        LOG(Info) << "\nName -> " << opcodeInfo.name << "\n"
                  << "opcode -> " << std::hex << int(opcode) << "\n"
                  << "opcodeLength -> " << opcodeInfo.opcodeLength << "\n"
                  << "flags -> " << int(opcodeInfo.psw) << "\n";
#endif
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

            }
            case CLD: {
                SetDecimal(0);
                break;
            }
            case LDA: {
                this->A = operand;
                SetZero(!A);
                SetSign(A & 0x80);
                break;
            }
            case SEI: {
                SetInterrupt(1);
            }
        }

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
}

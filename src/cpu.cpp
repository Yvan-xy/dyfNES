//
// Created by root on 2020/5/1.
//

#include <cpu.h>
#include <assert.h>

namespace dyfNES {

    CPU::CPU(Mem &_mem) : mem(_mem) {
        this->A = 0;
        this->X = this->Y = this->PC = this->PSW = 0;
        this->SP = 0;
    }

    void CPU::reset() {
        this->PC = 0xfffc;
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
                  << "opcode -> " << opcode << "\n"
                  << "opcodeLength -> " << opcodeInfo.opcodeLength << "\n"
                  << "flags -> " << int(opcodeInfo.psw) << "\n";
#endif
        TYPE type = opcodeInfo.type;
        operand = this->getOperand(opcodeInfo, address);

        switch (type) {
            case ADC: {

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
                break;
            }
            case Accumulator: {
                operand = this->A;
                break;
            }
            case Relative: {
                address = this->PC;
                operand = this->mem.readByte(this->PC);
                break;
            }
            case Absolute: {
                address = this->mem.readWord(this->PC);
                operand = this->mem.readByte(address);
                break;
            }
            case Absolute_X: {
                address = this->mem.readWord(this->PC) + this->X;
                operand = this->mem.readByte(address);
                break;
            }
            case Absolute_Y: {
                address = this->mem.readWord(this->PC) + this->Y;
                operand = this->mem.readByte(address);
                break;
            }
            case ZeroPage: {
                address = this->mem.readByte(this->PC);
                operand = this->mem.readByte(address);
                break;
            }
            case ZeroPage_X: {
                address = (this->mem.readByte(this->PC) + this->X) % 0x100;
                operand = this->mem.readByte(address);
                break;
            }
            case ZeroPage_Y: {
                address = (this->mem.readByte(this->PC) + this->Y) % 0x100;
                operand = this->mem.readByte(address);
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
                break;
            }
            case Indexed_Indirect_X: {
                Address idx = this->mem.readByte(this->PC) + this->X;
                address = this->mem.readWord(idx % 0x100);
                operand = this->mem.readByte(address);
                break;
            }
            case Indirect_Indexed_Y: {
                Address idx = this->mem.readWord(this->PC);
                address = this->mem.readWord(idx) + this->Y;
                operand = this->mem.readByte(address);
                break;
            }
            default: {
                assert(!"Wrong Address Method!");
                break;
            }
        }
        return operand;
    }

}

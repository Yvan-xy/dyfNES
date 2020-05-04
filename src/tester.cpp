//
// Created by root on 2020/5/1.
//

#include <log.h>
#include <mem.h>
#include <tester.h>
#include <romparser.h>
#include <cpu.h>

namespace dyfNES {
    Tester::Tester() {}

    void Tester::logTester() {
        LOG(Info) << "log test" << "ohhh" << std::endl;
    }

    void Tester::romparserTester() {
        const char *path = "./Super_mario_brothers.nes";
        ROMParser rompaser;
        rompaser.loadRom(path);
    }

    void Tester::memTester() {
        Mem *mem = new Mem();

        const char *path = "./Super_mario_brothers.nes";
        ROMParser romParser;
        romParser.loadRom(path);

        std::vector<Byte> PRG_ROM = romParser.getROM();
        mem->loadPRGROM(PRG_ROM);
        Byte result = mem->readByte(0x8000);
        std::cout << "result is 0x" << std::hex << int(result) << std::endl;
    }

    void Tester::CPUTester() {
        Mem *mem = new Mem();

        const char *path = "./Super_mario_brothers.nes";
        ROMParser romParser;
        romParser.loadRom(path);

        std::vector<Byte> PRG_ROM = romParser.getROM();
        mem->loadPRGROM(PRG_ROM);
        CPU *cpu = new CPU(*mem);
        Address resetAddr = cpu->readAddress(0xfffc);
        std::cout << "Reset vector is 0x" << std::hex << int(resetAddr) << std::endl;

        cpu->runOpcode(0x78);
    }
}
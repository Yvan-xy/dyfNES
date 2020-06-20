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

        const std::string path = "./1.Branch_Basics.nes";

        ROMParser romParser;
        int result = romParser.loadRom(path);
        if (!result) {
            LOG(Error) << "Read ROM Status is " << result << std::endl;
        }
//        exit(0);

        std::vector<Byte> PRG_ROM = romParser.getROM();
        mem->loadPRGROM(PRG_ROM);
        CPU *cpu = new CPU(*mem);
        Address resetAddr = cpu->readAddress(0xfffc);
        std::cout << "Reset vector is 0x" << std::hex << int(resetAddr) << std::endl;
        cpu->reset();
        for (int i = 0; i < 40; i++)
            cpu->step();
    }
}
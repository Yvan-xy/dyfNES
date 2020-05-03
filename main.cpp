#include <log.h>
#include <tester.h>
#include <iostream>

int main() {
    std::ofstream logFile("dyfnes.log"), cpuTraceFile;
    dyfNES::TeeStream logTee(logFile, std::cout);

    if (logFile.is_open() && logFile.good())
        dyfNES::Log::get().setLogStream(logTee);
    else
        dyfNES::Log::get().setLogStream(std::cout);

    dyfNES::Log::get().setLevel(dyfNES::Info);

    dyfNES::Tester tester;
    tester.memTester();

    return 0;
}

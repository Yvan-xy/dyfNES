//
// Created by root on 2020/5/1.
//

#ifndef DYFNES_TESTER_H
#define DYFNES_TESTER_H

namespace dyfNES {
    class Tester {
    public:
        Tester();

        static void logTester();

        void romparserTester();

        void memTester();
    };
}

#endif //DYFNES_TESTER_H

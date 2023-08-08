#pragma once

#include "SearchServer.hpp"

namespace UnitTesting {
    class UnitTests {
    private:
        SearchServer ss;
    public:
        bool TestAddDocument();

        bool TestMinusWords();

        bool TestFindAll();

        bool TestFindTop();


        void RunAllTests();
    };
}



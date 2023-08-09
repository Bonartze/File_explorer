#pragma once

#include "SearchServer.hpp"

namespace UnitTesting {
    class UnitTests {
    private:
        SearchServer ss;
    public:
        UnitTests();

        bool TestAddDocument();

        bool TestMinusWords();

        bool TestFindAll();

        bool TestFindTop();


        void RunAllTests();
    };
}



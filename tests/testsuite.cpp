#include <tests/testsuite.hpp>
#include <tests/test.hpp>

int TestSuite::runSuite() const {

    return std::count_if(_tests.begin(), _tests.end(), 
        [](const Test& t) -> bool { return t.runTest(); }
    );
}
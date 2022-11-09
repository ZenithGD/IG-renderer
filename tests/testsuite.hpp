#pragma once

#include <memory>
#include <vector>

#include <tests/test.hpp>

/**
 * @brief Test suite class for basic testing
 * 
 */
class TestSuite {
public:

    TestSuite(std::initializer_list<Test> tests) : _tests(tests) {}

    /**
     * @brief Run this test suite and return how many tests succeeded
     * 
     * @return int 
     */
    int runSuite() const;

private:

    std::vector<Test> _tests;
};
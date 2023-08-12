#include <iostream>

#include <estd/AnsiEscape.hpp>
#include <fstream>
#include <functional>
#include <iostream>

#pragma once

class UnitTests {
private:
    uint32_t testNum = 0;
    uint32_t passedNum = 0;

public:
    bool verbose = true;
    void __internalTestBool(const char* file, int line, const char* func, bool val) {
        using std::cout;
        using std::endl;
        testNum++;
        if (val) {
            cout << "[" << estd::setTextColor(0, 255, 0) << "PASS" << estd::clearSettings << "] test number ";
            passedNum++;
        } else {
            cout << "[" << estd::setTextColor(255, 0, 0) << "FAIL" << estd::clearSettings << "] test number ";
        }
        cout << testNum << " (" << file << ":" << line << ")" << endl;
    }

    void __internalTestFunction(const char* file, int line, const char* func, std::function<bool()> test) {
        using std::cout;
        using std::endl;
        testNum++;
        bool testResult = false;
        try {
            testResult = test();
        } catch (std::exception& e) {
            if (verbose) { cout << e.what() << endl; }
        } catch (...) {};
        if (testResult) {
            cout << "[" << estd::setTextColor(0, 255, 0) << "PASS" << estd::clearSettings << "] test number ";
            passedNum++;
        } else {
            cout << "[" << estd::setTextColor(255, 0, 0) << "FAIL" << estd::clearSettings << "] test number ";
        }
        cout << testNum << " (" << file << ":" << line << ")" << endl;
    }

    inline void testBool(bool) {
        throw std::runtime_error("This should be expanded to a macro, unit test framework internal fail.");
    }
    inline void testBlock(std::function<bool()>) { testBool(true); }
    inline void testLambda(std::function<bool()>) { testBool(false); }

    std::string getStats() {
        return std::string() + "TEST RESULTS: " + std::to_string(passedNum) + "/" + std::to_string(testNum);
    }
};

#define testBool(...) __internalTestBool(__FILE__, __LINE__, __func__, __VA_ARGS__)
#define testBlock(...) __internalTestFunction(__FILE__, __LINE__, __func__, [&]() { __VA_ARGS__ })
#define testLambda(...) __internalTestFunction(__FILE__, __LINE__, __func__, __VA_ARGS__)
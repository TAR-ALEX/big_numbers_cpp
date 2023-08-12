#include <climits>
#include <estd/BigDecimal.h>
#include <estd/BigInteger.h>
#include <estd/UnitTest.h>
#include <iostream>
#include <vector>


using namespace std;
using namespace estd;

std::vector<std::string> readLinesFromFile(const std::string& filename) {
    std::vector<std::string> lines;
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Failed to open the file: " << filename << std::endl;
        return lines;
    }

    std::string line;
    while (std::getline(file, line)) { lines.push_back(line); }

    file.close();
    return lines;
}

int main() {
    UnitTests test;

    auto expectedOutput = readLinesFromFile("./expected.txt");

    // compare to primitive testing
    test.testBool(BigInt{0} == 0);
    test.testBool(BigInt{1} == 1);
    test.testBool(BigInt{-1} == -1);
    test.testBool(BigInt{ULLONG_MAX} == uint64_t(ULLONG_MAX));
    test.testBool((BigInt{ULLONG_MAX} + 1) != (uint64_t(ULLONG_MAX) + 1));
    test.testBool((BigInt{ULLONG_MAX} - 1) == (uint64_t(ULLONG_MAX) - 1));

    test.testBool(BigInt{"0"} == 0);
    test.testBool(BigInt{"-0"} == 0);
    test.testBool(BigInt{"1"} == 1);
    test.testBool(BigInt{"-1"} == -1);

    test.testBool(BigDec{0} == 0);
    test.testBool(BigDec{1} == 1);
    test.testBool(BigDec{-1} == -1);
    test.testBool(BigDec{0.0} == 0);//fails
    test.testBool(BigDec{1.0} == 1);
    test.testBool(BigDec{-1.0} == -1);
    test.testBool(BigDec{ULLONG_MAX} == uint64_t(ULLONG_MAX));
    test.testBool((BigDec{ULLONG_MAX} + 1) != (uint64_t(ULLONG_MAX) + 1));
    test.testBool((BigDec{ULLONG_MAX} - 1) == (uint64_t(ULLONG_MAX) - 1));

    test.testBool(BigDec{"0"} == 0);
    test.testBool(BigDec{"-0"} == 0);
    test.testBool(BigDec{"1"} == 1);
    test.testBool(BigDec{"-1"} == -1);
    test.testBool(BigDec{"0.0"} == 0);
    test.testBool(BigDec{"-0.0"} == 0);
    test.testBool(BigDec{"1.0"} == 1);
    test.testBool(BigDec{"-1.0"} == -1);

    //toString tests
    test.testBool(BigInt{0}.toString() == "0");
    test.testBool(BigInt{1}.toString()  == "1");
    test.testBool(BigInt{-1}.toString()  == "-1");
    test.testBool(BigInt{ULLONG_MAX}.toString()  == std::to_string(ULLONG_MAX));
    test.testBool((BigInt{ULLONG_MAX} + 1).toString() != std::to_string(uint64_t(ULLONG_MAX) + 1));
    test.testBool((BigInt{ULLONG_MAX} - 1).toString() == std::to_string(uint64_t(ULLONG_MAX) - 1));

    test.testBool(BigInt{"0"}.toString() == "0");
    test.testBool(BigInt{"-0"}.toString() == "0");
    test.testBool(BigInt{"1"}.toString() == "1");
    test.testBool(BigInt{"-1"}.toString() == "-1");

    test.testBool(BigDec{0}.toString() == "0");
    test.testBool(BigDec{1}.toString() == "1");
    test.testBool(BigDec{-1}.toString() == "-1");
    test.testBool(BigDec{0.0}.toString() == "0");//fails
    test.testBool(BigDec{1.0}.toString() == "1");
    test.testBool(BigDec{-1.0}.toString() == "-1");
    test.testBool(BigDec{ULLONG_MAX}.toString() == std::to_string(ULLONG_MAX));
    test.testBool((BigDec{ULLONG_MAX} + 1) != std::to_string(uint64_t(ULLONG_MAX) + 1));
    test.testBool((BigDec{ULLONG_MAX} - 1) == std::to_string(uint64_t(ULLONG_MAX) - 1));

    test.testBool(BigDec{"0"} == "0");
    test.testBool(BigDec{"-0"} == "0");
    test.testBool(BigDec{"1"} == "1");
    test.testBool(BigDec{"-1"} == "-1");
    test.testBool(BigDec{"0.0"} == "0");
    test.testBool(BigDec{"-0.0"} == "0");
    test.testBool(BigDec{"1.0"} == "1");
    test.testBool(BigDec{"-1.0"} == "-1");

    test.testBlock(
        try { BigInt{"-1.0"}; } catch (...) { return true; } return false;
    );
    test.testBlock(
        try { BigInt{"1.0"}; } catch (...) { return true; } return false;
    );

    test.testBlock({
        BigInt i = 0;
        BigInt k = nullptr;
        return k != i && k.toString() != i.toString();
    });

    test.testBlock({
        BigDec i = "0";
        BigDec k = nullptr;
        return k != i && k.toString() != i.toString();
    });

    test.testBlock(BigInt i = 2; BigInt k = i.power(9000) - "1" + 1000; return k.toString() == expectedOutput.at(0););

    test.testBool(BigInt{0} == BigInt{"0"});

    std::cout << test.getStats() << std::endl;

    return 0;
}

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
    while (std::getline(file, line)) {
        lines.push_back(line);
    }

    file.close();
    return lines;
}

int main() {
    UnitTests test;

    auto expectedOutput = readLinesFromFile("./expected.txt");

    test.testLambda([&]() {
        BigInt i = 0;
        BigInt k = nullptr;
        return k != i && k.toString() != i.toString();
    });

    test.testLambda([&]() {
        BigDec i = "0";
        BigDec k = nullptr;
        return k != i && k.toString() != i.toString();
    });

    test.testLambda([&]() {
        BigInt i = 2;
        BigInt k = i.power(9000) - "1" + 1000;
        return k.toString() == expectedOutput.at(0);
    });

    test.testBool(BigInt{0} == BigInt{"0"});

    std::cout << test.getStats() << std::endl;


    // BigInt i = 9;
    // cout << i << endl;
    // i <<= 1;
    // cout << (BigInt(0) == BigInt(9)) << endl;
    // cout << (BigInt(2) * BigInt(9)) << endl;
    return 0;
}

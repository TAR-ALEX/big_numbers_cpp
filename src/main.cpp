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

    system("python3 ./src/python-reference.py");

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
    test.testBool(BigDec{0.0} == 0); 
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
    test.testBool(BigInt{1}.toString() == "1");
    test.testBool(BigInt{-1}.toString() == "-1");
    test.testBool(BigInt{ULLONG_MAX}.toString() == std::to_string(ULLONG_MAX));
    test.testBool((BigInt{ULLONG_MAX} + 1).toString() != std::to_string(uint64_t(ULLONG_MAX) + 1));
    test.testBool((BigInt{ULLONG_MAX} - 1).toString() == std::to_string(uint64_t(ULLONG_MAX) - 1));

    test.testBool(BigInt{"0"}.toString() == "0");
    test.testBool(BigInt{"-0"}.toString() == "0");
    test.testBool(BigInt{"1"}.toString() == "1");
    test.testBool(BigInt{"-1"}.toString() == "-1");

    test.testBool(BigDec{0}.toString() == "0");
    test.testBool(BigDec{1}.toString() == "1");
    test.testBool(BigDec{-1}.toString() == "-1");
    test.testBool(BigDec{0.0}.toString() == "0"); 
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

    //case 0 python
    test.testBlock({
        BigInt i = 2;
        BigInt k = i.power(90000) - "1" + 1000;
        return k.toString() == expectedOutput.at(0);
    });

    //case 1 python
    test.testBlock({
        BigInt i = "123908478902347";
        BigInt k = i / "9000";
        return k.toString() == expectedOutput.at(1);
    });

    //case 2 python
    test.testBlock({
        BigInt i = BigInt{"1902834789798719023847"} * BigInt{"8123740981273492389479079234817"};
        return i.toString() == expectedOutput.at(2);
    });

    //case 3 python
    test.testBlock({
        BigDec i = BigDec{"8123740981273492389479079234817"} / BigDec{"1902834789798719023847"};
        return i.toString() == expectedOutput.at(3);
    });

    //case 4 python
    test.testBlock({
        BigDec i = BigDec{"981234689762398678687786784126786"} - BigDec{"2389746987126060789568967867823465783678787862354876"};
        return i.toString() == expectedOutput.at(4);
    });

    //case 5 python
    test.testBlock({
        BigDec i = BigDec{"92813468712365489269086001623000008792316"} + BigDec{"1287364786890689068934612364781258999"};
        return i.toString() == expectedOutput.at(5);
    });

    //case 6 python
    test.testBlock({
        BigInt i = BigInt{"92813468712365489269086001623000003464534656458792316"} >> 20;
        return i.toString() == expectedOutput.at(6);
    });

    //case 7 python
    test.testBlock({
        BigInt i = BigInt{"92813468712365489269086001623000003464534656458792316"} << 20;
        return i.toString() == expectedOutput.at(7);
    });

    //case 8 python
    test.testBlock({
        BigInt i = BigInt{"92813468712365489269086001623000003464534656458792316"} % 26435;
        return i.toString() == expectedOutput.at(8);
    });


    std::cout << test.getStats() << std::endl;

    return 0;
}

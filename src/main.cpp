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

    // Constructor tests
    test.testBlock({
        BigInt i = BigInt{"0"};
        return i.toString() == "0";
    });

    test.testBlock({
        BigDec d = BigDec{"0.0"};
        return d.toString() == "0";
    });

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

    //compare to toString()
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

    // verify errors thrown

    test.testBlock(
        try { BigInt{"-1.0"}; } catch (...) { return true; } return false;
    );
    test.testBlock(
        try { BigInt{"1.0"}; } catch (...) { return true; } return false;
    );

    // Simple operator testing, for near zero edge cases

    // Addition and Subtraction: simple edge cases

    test.testBlock({
        BigInt i = BigInt{"-1"} - BigInt{"-1"};
        BigInt k = "0";
        return k == i && k.toString() == i.toString();
    });

    test.testBlock({
        BigInt i = BigInt{"1"} - BigInt{"1"};
        BigInt k = "0";
        return k == i && k.toString() == i.toString();
    });

    test.testBlock({
        BigInt i = BigInt{"-1"} + BigInt{"1"};
        BigInt k = "0";
        return k == i && k.toString() == i.toString();
    });

    test.testBlock({
        BigInt i = BigInt{"1"} + BigInt{"-1"};
        BigInt k = "0";
        return k == i && k.toString() == i.toString();
    });

    test.testBlock({
        BigInt i = BigInt{"-2"} - BigInt{"-1"};
        BigInt k = "-1";
        return k == i && k.toString() == i.toString();
    });

    test.testBlock({
        BigInt i = BigInt{"2"} - BigInt{"1"};
        BigInt k = "1";
        return k == i && k.toString() == i.toString();
    });

    test.testBlock({
        BigInt i = BigInt{"-2"} + BigInt{"1"};
        BigInt k = "-1";
        return k == i && k.toString() == i.toString();
    });

    test.testBlock({
        BigInt i = BigInt{"2"} + BigInt{"-1"};
        BigInt k = "1";
        return k == i && k.toString() == i.toString();
    });

    // Multiplication: simple edge cases

    test.testBlock({
        BigInt i = BigInt{"2"} * BigInt{"0"};
        BigInt k = "0";
        return k == i && k.toString() == i.toString();
    });

    test.testBlock({
        BigInt i = BigInt{"-2"} * BigInt{"-0"};
        BigInt k = "0";
        return k == i && k.toString() == i.toString();
    });

    test.testBlock({
        BigInt i = BigInt{"2"} * BigInt{"-0"};
        BigInt k = "0";
        return k == i && k.toString() == i.toString();
    });

    test.testBlock({
        BigInt i = BigInt{"-2"} * BigInt{"0"};
        BigInt k = "0";
        return k == i && k.toString() == i.toString();
    });

    // Division: simple edge cases

    test.testBlock({
        BigInt i = BigInt{"0"} / BigInt{"2"};
        BigInt k = "0";
        return k == i && k.toString() == i.toString();
    });

    test.testBlock({
        BigInt i = BigInt{"0"} / BigInt{"-2"};
        BigInt k = "0";
        return k == i && k.toString() == i.toString();
    });

    // Modulo, remainder simple edge cases

    test.testBlock({
        BigInt i = BigInt{"1"} % BigInt{"2"};
        BigInt k = "1";
        return k == i && k.toString() == i.toString();
    });

    test.testBlock({
        BigInt i = BigInt{"1"} % BigInt{"-2"};
        BigInt k = "1";
        return k == i && k.toString() == i.toString();
    });

    test.testBlock({
        BigInt i = BigInt{"-1"} % BigInt{"2"};
        BigInt k = "1";
        return k == i && k.toString() == i.toString();
    });

    test.testBlock({
        BigInt i = BigInt{"-1"} % BigInt{"-2"};
        BigInt k = "1";
        return k == i && k.toString() == i.toString();
    });

    // Some more practical test cases

    // Arithmetic operations for BigInt

    // Addition
    test.testBlock({
        BigInt a = BigInt{"12345678901234567890"};
        BigInt b = BigInt{"98765432109876543210"};
        BigInt sum = a + b;
        return sum.toString() == "111111111011111111100";
    });

    // Subtraction
    test.testBlock({
        BigInt a = BigInt{"98765432109876543210"};
        BigInt b = BigInt{"12345678901234567890"};
        BigInt difference = a - b;
        return difference.toString() == "86419753208641975320";
    });

    // Multiplication
    test.testBlock({
        BigInt a = BigInt{"123456789"};
        BigInt b = BigInt{"987654321"};
        BigInt product = a * b;
        return product.toString() == "121932631112635269";
    });

    // Division
    test.testBlock({
        BigInt a = BigInt{"98765432109876543210"};
        BigInt b = BigInt{"12345678901234567890"};
        BigInt quotient = a / b;
        return quotient.toString() == "8";
    });

    // Arithmetic operations for BigDec

    // Addition
    test.testBlock({
        BigDec a = BigDec{"12345.6789"};
        BigDec b = BigDec{"98765.4321"};
        BigDec sum = a + b;
        return sum.toString() == "111111.111";
    });

    // Subtraction
    test.testBlock({
        BigDec a = BigDec{"98765.4321"};
        BigDec b = BigDec{"12345.6789"};
        BigDec difference = a - b;
        return difference.toString() == "86419.7532";
    });

    // Multiplication
    test.testBlock({
        BigDec a = BigDec{"123.456"};
        BigDec b = BigDec{"987.654"};
        BigDec product = a * b;
        return product.toString() == "121931.812224";
    });

    // Division
    test.testBlock({
        BigDec a = BigDec{"98765.4321"};
        BigDec b = BigDec{"12345.6789"};
        BigDec quotient = a / b;
        return quotient.toString() == "8"; // Assuming up to 8 decimal places of precision
    });

    // Comparison operations for BigInt

    test.testBlock({
        BigInt a = BigInt{"123456789"};
        BigInt b = BigInt{"987654321"};
        return a < b;
    });

    test.testBlock({
        BigInt a = BigInt{"987654321"};
        BigInt b = BigInt{"123456789"};
        return a > b;
    });

    test.testBlock({
        BigInt a = BigInt{"123456789"};
        BigInt b = BigInt{"123456789"};
        return a == b;
    });

    // Comparison operations for BigDec

    test.testBlock({
        BigDec a = BigDec{"123.456"};
        BigDec b = BigDec{"987.654"};
        return a < b;
    });

    test.testBlock({
        BigDec a = BigDec{"987.654"};
        BigDec b = BigDec{"123.456"};
        return a > b;
    });

    test.testBlock({
        BigDec a = BigDec{"123.456"};
        BigDec b = BigDec{"123.456"};
        return a == b;
    });

    // Edge Cases for BigInt

    test.testBlock({
        BigInt a = BigInt{"0"};
        BigInt b = BigInt{"0"};
        BigInt sum = a + b;
        return sum.toString() == "0";
    });

    test.testBlock({
        BigInt a = BigInt{"-12345678901234567890"};
        BigInt b = BigInt{"12345678901234567890"};
        BigInt sum = a + b;
        return sum.toString() == "0";
    });

    // Edge Cases for BigDec

    test.testBlock({
        BigDec a = BigDec{"-12345.6789"};
        BigDec b = BigDec{"12345.6789"};
        BigDec sum = a + b;
        return sum.toString() == "0";
    });

    test.testBlock({
        BigDec a = BigDec{"0.0001"};
        BigDec b = BigDec{"0.0009"};
        BigDec sum = a + b;
        return sum.toString() == "0.001"; // Assuming up to 3 decimal places of precision
    });

    // Power
    test.testBlock({
        BigInt base = BigInt{"2"};
        BigInt exp = BigInt{"10"};
        BigInt result = base.power(exp); // Using the correct method name
        return result.toString() == "1024";
    });

    // Modulo
    test.testBlock({
        BigInt a = BigInt{"12345678901234567890"};
        BigInt b = BigInt{"987654321"};
        BigInt mod = a % b;
        return mod.toString() == "339506163";
    });

    // Left Shift
    test.testBlock({
        BigInt a = BigInt{"12345678901234567890"};
        BigInt result = a << 3;                             // Left shifting by 3 positions
        return result.toString() == "98765431209876543120"; // Result based on simple binary shift semantics
    });

    // Right Shift
    test.testBlock({
        BigInt a = BigInt{"98765432109876543210"};
        BigInt result = a >> 4;                            // Right shifting by 4 positions
        return result.toString() == "6172839506867283950"; // Result based on simple binary shift semantics
    });

    // TODO:

    // // Binary AND
    // test.testBlock({
    //     BigInt a = BigInt{"170"};          // Equivalent to binary 10101010
    //     BigInt b = BigInt{"213"};          // Equivalent to binary 11010101
    //     BigInt result = a & b;             // Binary AND operation
    //     return result.toString() == "160"; // Equivalent to binary 10100000
    // });

    // // Binary OR
    // test.testBlock({
    //     BigInt a = BigInt{"170"};          // Equivalent to binary 10101010
    //     BigInt b = BigInt{"213"};          // Equivalent to binary 11010101
    //     BigInt result = a | b;             // Binary OR operation
    //     return result.toString() == "223"; // Equivalent to binary 11011111
    // });

    // Complex test cases that test really big numbers, verified against python to avoid long numeric constant strings

    //case 0 python
    test.testBlock({ //the slowest test case
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
        BigDec i = BigDec{"981234689762398678687786784126786"} -
                   BigDec{"2389746987126060789568967867823465783678787862354876"};
        return i.toString() == expectedOutput.at(4);
    });

    //case 5 python
    test.testBlock({
        BigDec i =
            BigDec{"92813468712365489269086001623000008792316"} + BigDec{"1287364786890689068934612364781258999"};
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

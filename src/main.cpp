#include <estd/BigInt.h>
#include <estd/BigDecimal.h>
#include <iostream>

using namespace std;
using namespace estd;

int main() {
    BigInt i = 2;
    BigInt k = i.power(900000) - "1" + 1000;
    cout << k << endl;
    return 0;
}

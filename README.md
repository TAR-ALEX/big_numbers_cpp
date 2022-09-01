# BigNumbers

A c++ / cpp implementation of Big Integers and Big Decimal (TODO: Big Decimal)

Sample usage:


```c++
#include <estd/BigInt.h>
#include <iostream>

using namespace std;
using namespace estd;

int main() {
    BigInt i = 2;
    BigInt k = i.power(900000) - "1" + 1000;
    cout << k << endl;
    return 0;
}
```

The makefile will build and run the main file. (modify the main file to try out the library)


To use this project with a dependency manager install the cpp-dependency-manager project from https://github.com/TAR-ALEX/Cpp-Dependency-Manager.git

and create a vendor.txt file and add the following entries:

```
git "https://github.com/TAR-ALEX/big_numbers_cpp.git" master "./include" "./vendor/include",

```

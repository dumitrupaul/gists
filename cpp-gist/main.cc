#include <cmath>
#include <cstdint>
#include <fstream>
#include <function.hh>
#include <iostream>

using byte = unsigned char;

int main() {
    std::vector<int32_t> memoryTest;
    for (int64_t i = 0; i < 1000000000000000;i++) {
        memoryTest.push_back(0);
        memoryTest[i] = 0;
        memoryTest[i]++;
        memoryTest[i] = memoryTest[i] << 1;
        std::cout << memoryTest[i] << std::endl;
    }

    return EXIT_SUCCESS;
}
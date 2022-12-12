#include <cmath>
#include <cstdint>
#include <fstream>
#include <iostream>

using byte = unsigned char;

int main() {
    std::fstream inputFile("/home/jamez-amd-ubuntu/gists/input/day9_ex.in", std::ios_base::in);
    if (!inputFile.is_open()) {
        std::cerr << "Couldn't open input file" << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
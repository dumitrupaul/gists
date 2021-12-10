#include <cmath>
#include <cstdint>
#include <fstream>
#include <function.hh>
#include <iostream>

using byte = unsigned char;

int main() {
    std::fstream inputFile("/home/jamez-amd-ubuntu/gists/input/day9_ex.in", std::ios_base::in);
    if (!inputFile.is_open()) {
        std::cerr << "Couldn't open input file" << std::endl;
        return EXIT_FAILURE;
    }

    std::string inputString;
    std::vector<std::vector<uint8_t>> digits;

    while (inputFile >> inputString) {
        std::vector<uint8_t> line;
        for (auto const& s : inputString) {
            line.push_back(s - '0');
        }
        digits.emplace_back(std::move(line));
    }

    std::cout << getBasins(digits) << std::endl;

    return EXIT_SUCCESS;
}
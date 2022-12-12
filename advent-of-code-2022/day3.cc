#include <algorithm>
#include <cmath>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>

uint32_t parseThreeRucksacks(std::string first, std::string second, std::string third) {
    std::unordered_set<char> countedCharacters;
    uint32_t totalPriority(0);
    for (auto const& f : first) {
        for (auto const& s : second) {
            if (countedCharacters.find(s) == countedCharacters.end() && f == s) {
                for (auto const& t : third) {
                    if (countedCharacters.find(s) == countedCharacters.end() && s == t) {
                        countedCharacters.insert(t);
                        if (islower(s)) {
                            totalPriority += s - 96;
                        } else {
                            totalPriority += s - 38;
                        }
                        // std::cout << s << "\t" << totalPriority << std::endl;
                    }
                }
            }
        }
    }
    return totalPriority;
}

int main() {
    std::fstream inputFile("../input/day3.in", std::ios_base::in);
    if (!inputFile.is_open()) {
        std::cerr << "Couldn't open input file" << std::endl;
        return EXIT_FAILURE;
    }

    std::array<std::string, 3> inputString;
    uint32_t totalPriority(0);
    while (inputFile >> inputString[0] >> inputString[1] >> inputString[2]) {
        totalPriority += parseThreeRucksacks(inputString[0], inputString[1], inputString[2]);
    }

    std::cout << totalPriority << std::endl;
    return EXIT_SUCCESS;
}
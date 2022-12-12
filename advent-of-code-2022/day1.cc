#include <algorithm>
#include <cmath>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

int main() {
    std::fstream inputFile("../input/day1.in", std::ios_base::in);
    if (!inputFile.is_open()) {
        std::cerr << "Couldn't open input file" << std::endl;
        return EXIT_FAILURE;
    }

    std::string inputString;
    std::vector<uint32_t> elfCalories;
    uint32_t calories(0);
    uint32_t maxCalories(0);
    while (std::getline(inputFile, inputString)) {
        if (inputString.empty()) {
            elfCalories.push_back(calories);
            if (maxCalories < calories) {
                maxCalories = calories;
            }
            calories = 0;
            continue;
        }
        calories += std::stoul(inputString);
    }
    elfCalories.push_back(calories);

    // part 1
    std::cout << "part 1: " << maxCalories << std::endl;

    // part 2
    std::sort(elfCalories.begin(), elfCalories.end(), std::greater<uint32_t>());
    std::cout << elfCalories[0] + elfCalories[1] + elfCalories[2] << std::endl;

    return EXIT_SUCCESS;
}
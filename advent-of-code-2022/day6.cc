#include <algorithm>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <cmath>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <regex>
#include <stack>
#include <string>
#include <string_view>
#include <unordered_set>
#include <vector>

bool containsDifferentChars(std::string const& input) {
    for (size_t i = 0; i < input.size(); ++i) {
        for (size_t j = i + 1; j < input.size(); ++j) {
            if (input[i] == input[j])
                return false;
        }
    }
    return true;
}

int main() {
    std::fstream inputFile("../input/day6.in", std::ios_base::in);
    if (!inputFile.is_open()) {
        std::cerr << "Couldn't open input file" << std::endl;
        return EXIT_FAILURE;
    }

    std::string inputString;
    while (std::getline(inputFile, inputString)) {

        for (size_t i = 0; i < inputString.size() - 14; i++) {
            if (containsDifferentChars(inputString.substr(i, 14))) {
                std::cout << i + 14 << std::endl;
                break;
            }
        }
    }

    return EXIT_SUCCESS;
}
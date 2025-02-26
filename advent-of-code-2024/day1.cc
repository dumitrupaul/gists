#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <fmt/chrono.h>
#include <fstream>
#include <future>
#include <iostream>
#include <list>
#include <map>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

constexpr const char getDayNumber(const char* path) {
    char file = '0';
    while (*(path + 1)) {
        if (*path == '.' && *(path + 1) == 'c') {
            file = *(path - 1);
            break;
        }
        ++path;
    }
    return file;
}

uint64_t getTotalDistance(std::vector<uint32_t> const& a, std::vector<uint32_t> const& b) {
    uint64_t sum{0};
    for(size_t i = 0; i < a.size();i++) {
        sum += std::abs(static_cast<int32_t>(a[i] - b[i]));
    }

    return sum;
}

uint64_t getSimilarityScore(std::vector<uint32_t> const& a, std::unordered_map<uint32_t, uint32_t> rightMap) {
    uint64_t sum{0};
    for(size_t i = 0; i < a.size();i++) {
        if(rightMap.contains(a[i])) {
            sum += a[i] * rightMap[a[i]];
        }
    }

    return sum;
}

int main() {
    static string const INPUT_FILE{"advent-of-code-2024/input/day" +
                                   std::string{getDayNumber(__FILE__)} + ".in"};
    std::fstream inputFile(INPUT_FILE.data(), std::ios_base::in);
    if (!inputFile.is_open()) {
        std::cerr << "Couldn't open input file" << std::endl;
        return EXIT_FAILURE;
    }

    string inputString;
    std::vector<uint32_t> leftSide, rightSide;
    std::unordered_map<uint32_t, uint32_t> rightMap;
    while (std::getline(inputFile, inputString)) {
        istringstream inputStream{inputString};
        uint32_t leftNumber{};
        uint32_t rightNumber{};
        inputStream >> leftNumber >> rightNumber;
        leftSide.push_back(leftNumber);
        rightSide.push_back(rightNumber);
        if (rightMap.contains(rightNumber)){
            rightMap[rightNumber]++;
        } else {
            rightMap[rightNumber] = 1;
        }
    }

    // std::sort(leftSide.begin(),leftSide.end());
    // std::sort(rightSide.begin(), rightSide.end());

    // std::cout << getTotalDistance(leftSide, rightSide) << std::endl;

    std::cout << getSimilarityScore(leftSide, rightMap) << std::endl;

    return EXIT_SUCCESS;
}
#include <algorithm>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <cmath>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <regex>
#include <stack>
#include <string>
#include <string_view>
#include <unordered_set>
#include <vector>

int main() {
    std::fstream inputFile("../input/day8.in", std::ios_base::in);
    if (!inputFile.is_open()) {
        std::cerr << "Couldn't open input file" << std::endl;
        return EXIT_FAILURE;
    }

    std::string inputString;
    std::vector<std::vector<uint32_t>> map;
    size_t k(0);
    while (std::getline(inputFile, inputString)) {
        map.push_back(std::vector<uint32_t>());

        for (auto const& c : inputString) {
            map[k].push_back(c - '0');
        }
        k++;
    }

    uint32_t maxScenicScore(0);
    for (uint32_t i = 1; i < map.size() - 1; ++i) {
        for (uint32_t j = 1; j < map.size() - 1; ++j) {
            uint32_t scenicScore(1);

            uint32_t numberOfTreesSeen(0);
            for (uint32_t i1 = i + 1; i1 < map.size(); i1++) {
                numberOfTreesSeen++;
                if (map[i1][j] >= map[i][j]) {
                    break;
                }
            }
            scenicScore *= numberOfTreesSeen;

            numberOfTreesSeen = 0;
            for (int32_t i1 = i - 1; i1 >= 0; i1--) {
                numberOfTreesSeen++;
                if (map[i1][j] >= map[i][j]) {
                    break;
                }
            }
            scenicScore *= numberOfTreesSeen;

            numberOfTreesSeen = 0;
            for (uint32_t j1 = j + 1; j1 < map.size(); j1++) {
                numberOfTreesSeen++;
                if (map[i][j1] >= map[i][j]) {
                    break;
                }
            }
            scenicScore *= numberOfTreesSeen;

            numberOfTreesSeen = 0;
            for (int32_t j1 = j - 1; j1 >= 0; j1--) {
                numberOfTreesSeen++;
                if (map[i][j1] >= map[i][j]) {
                    break;
                }
            }
            scenicScore *= numberOfTreesSeen;

            if (maxScenicScore < scenicScore) {
                maxScenicScore = scenicScore;
            }
        }
    }

    std::cout << maxScenicScore << std::endl;

    return EXIT_SUCCESS;
}
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <map>
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

struct PossibleGear {
    int32_t iPos{-1};
    int32_t jPos{-1};
    vector<uint32_t> neighbors;
};

bool hasAdjSymbol(uint32_t number,
                  vector<string> const& schematic,
                  vector<PossibleGear>& possibleGears,
                  int32_t line,
                  int32_t startPos,
                  size_t numberSize) {
    int32_t startingI = line == 0 ? line : line - 1;
    int32_t endingI = line == schematic.size() - 1 ? line : line + 1;
    int32_t startingJ = startPos == 0 ? startPos : startPos - 1;
    int32_t endingJ = startPos + numberSize >= schematic[line].size() - 1 ?
                          startPos + numberSize - 1 :
                          startPos + numberSize;
    for (int32_t i = startingI; i <= endingI; ++i) {
        for (int32_t j = startingJ; j <= endingJ; ++j) {
            if (schematic[i][j] >= 48 && schematic[i][j] <= 57) {
                continue;
            } else if (schematic[i][j] == '*') {
                auto foundPossibleGear =
                    find_if(begin(possibleGears), end(possibleGears),
                            [i, j](PossibleGear p) { return p.iPos == i && p.jPos == j; });
                if (foundPossibleGear != end(possibleGears)) {
                    foundPossibleGear->neighbors.push_back(number);
                } else {
                    possibleGears.emplace_back(PossibleGear{i, j, {number}});
                }
                return true;
            } else if (schematic[i][j] != '.') {
                return true;
            }
        }
    }
    return false;
}

int main() {
    static string INPUT_FILE{"advent-of-code-2023/input/day" + std::string{getDayNumber(__FILE__)} +
                             ".in"};
    std::fstream inputFile(INPUT_FILE.data(), std::ios_base::in);
    if (!inputFile.is_open()) {
        std::cerr << "Couldn't open input file" << std::endl;
        return EXIT_FAILURE;
    }

    string inputString;
    vector<string> schematic;
    while (std::getline(inputFile, inputString)) {
        schematic.push_back(inputString);
    }

    uint64_t sum{0};
    vector<PossibleGear> possibleGears;
    for (int32_t i = 0; i < schematic.size(); i++) {
        string number;
        for (int32_t j = 0; j < schematic[i].size(); j++) {
            if (schematic[i][j] >= 48 && schematic[i][j] <= 57) {
                number += schematic[i][j];
            } else if (!number.empty()) {
                // cout << "number: " << number << endl;
                uint32_t nr = stoul(number);
                if (hasAdjSymbol(nr, schematic, possibleGears, i, j - number.size(),
                                 number.size())) {
                    sum += nr;
                }
                number.clear();
            }
        }
        if (!number.empty()) {
            uint32_t nr = stoul(number);
            if (hasAdjSymbol(nr, schematic, possibleGears, i, schematic[i].size() - number.size(),
                             number.size())) {
                sum += nr;
            }
        }
    }

    uint64_t gearRatio{0};
    for (auto const& gear : possibleGears) {
        if (gear.neighbors.size() == 2) {
            gearRatio += gear.neighbors[0] * gear.neighbors[1];
        }
    }

    cout << gearRatio << endl;

    return EXIT_SUCCESS;
}
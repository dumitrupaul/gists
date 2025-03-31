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
#include <unordered_map>
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

enum class Direction { N, S, E, V, NE, SE, SV, NV };
struct DirectionValue {
    const int i;
    const int j;
    bool ok{true};
};
uint32_t countXMAS(vector<string> const& puzzle) {
    uint32_t count{0};
    static string const XMAS{"XMAS"};

    for (int i = 0; i < puzzle.size(); i++) {
        for (int j = 0; j < puzzle[i].size(); j++) {
            // if it's not X not worth continuing
            if (puzzle[i][j] != XMAS[0]) {
                continue;
            }
            unordered_map<Direction, DirectionValue> dirMap{
                {Direction::N, {-1, 0}},  {Direction::S, {1, 0}},   {Direction::E, {0, 1}},
                {Direction::V, {0, -1}},  {Direction::NE, {-1, 1}}, {Direction::SE, {1, 1}},
                {Direction::SV, {1, -1}}, {Direction::NV, {-1, -1}}};

            // Check all directions for XMAS word matches
            for (int k = 1; k < XMAS.size(); k++) {
                // Check the chosen letter in k position in all directions
                for (auto& [dirEnum, dirValues] : dirMap) {
                    // direction already invalidated
                    if (!dirValues.ok) {
                        continue;
                    }

                    // Adjust i and j based on the current position in k
                    int iCurr{i + dirValues.i * (k - 1)};
                    int jCurr{j + dirValues.j * (k - 1)};

                    // sanity checks
                    if (iCurr + dirValues.i < 0 || iCurr + dirValues.i >= puzzle.size() ||
                        jCurr + dirValues.j < 0 ||
                        jCurr + dirValues.j >= puzzle[iCurr + dirValues.i].size()) {
                        dirValues.ok = false;
                        continue;
                    }

                    if (puzzle[iCurr + dirValues.i][jCurr + dirValues.j] != XMAS[k]) {
                        dirValues.ok = false;
                        continue;
                    }
                }
            }

            // Check each direction to see if any matched the whole word and count each one
            for (auto& [dirEnum, dirValues] : dirMap) {
                if (dirValues.ok) {
                    count++;
                }
            }
        }
    }
    return count;
}

uint32_t countXMASpart2(vector<string> const& puzzle) {
    struct DirectionValueChar {
        const int i;
        const int j;
    };
    uint32_t count{0};
    for (int i = 0; i < puzzle.size(); i++) {
        for (int j = 0; j < puzzle[i].size(); j++) {
            // if it's not A not worth continuing
            if (puzzle[i][j] != 'A') {
                continue;
            }

            unordered_map<Direction, DirectionValueChar> dirMap{{Direction::NE, {-1, 1}},
                                                                {Direction::SE, {1, 1}},
                                                                {Direction::SV, {1, -1}},
                                                                {Direction::NV, {-1, -1}}};

            auto getOppositeChar = [](char c) -> char {
                if (c == 'M')
                    return 'S';
                if (c == 'S')
                    return 'M';
                return '\0';
            };
            auto getOppositeDirection = [](Direction dir) -> Direction {
                if (dir == Direction::NE)
                    return Direction::SV;
                if (dir == Direction::SV)
                    return Direction::NE;
                if (dir == Direction::SE)
                    return Direction::NV;
                if (dir == Direction::NV)
                    return Direction::SE;
                return Direction::SE;
            };
            auto isDirectionValid = [&puzzle, &dirMap, i, j](Direction dir) -> bool {
                if (i + dirMap.at(dir).i < 0 || i + dirMap.at(dir).i >= puzzle.size() ||
                    j + dirMap.at(dir).j < 0 ||
                    j + dirMap.at(dir).j >= puzzle[i + dirMap.at(dir).i].size()) {
                    return false;
                }
                return true;
            };

            bool skip{false};
            for (auto const& dir : {Direction::NE, Direction::NV}) {
                if (!isDirectionValid(dir) || !isDirectionValid(getOppositeDirection(dir))) {
                    skip = true;
                    break;
                }

                char currentChar{puzzle[i + dirMap.at(dir).i][j + dirMap.at(dir).j]};
                if (currentChar != 'S' && currentChar != 'M') {
                    skip = true;
                    break;
                }

                if (puzzle[i + dirMap.at(getOppositeDirection(dir)).i]
                          [j + dirMap.at(getOppositeDirection(dir)).j] !=
                    getOppositeChar(currentChar)) {
                    skip = true;
                    break;
                }
            }

            if (!skip) {
                count++;
            }
        }
    }
    return count;
}

int main() {
    static string const INPUT_FILE{"advent-of-code-2024/input/day" +
                                   std::string{getDayNumber(__FILE__)} + ".in"};
    std::fstream inputFile(INPUT_FILE.data(), std::ios_base::in);
    if (!inputFile.is_open()) {
        std::cerr << "Couldn't open input file" << std::endl;
        return EXIT_FAILURE;
    }

    vector<string> puzzle;
    string inputString;
    while (std::getline(inputFile, inputString)) {
        puzzle.push_back(inputString);
    }

    cout << countXMASpart2(puzzle) << endl;

    return EXIT_SUCCESS;
}
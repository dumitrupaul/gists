#include <algorithm>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/compare.hpp>
#include <boost/algorithm/string/split.hpp>
#include <cmath>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <queue>
#include <regex>
#include <set>
#include <sstream>
#include <stack>
#include <string>
#include <string_view>
#include <unordered_set>
#include <variant>
#include <vector>

using namespace std;

static vector<vector<uint32_t>> getRock(size_t index) {
    static vector<vector<vector<uint32_t>>> _ = {{{{{0, 0, 0, 0, 0, 0, 0},
                                                    {0, 0, 0, 0, 0, 0, 0},
                                                    {0, 0, 0, 0, 0, 0, 0},
                                                    {0, 0, 1, 1, 1, 1, 0}}},
                                                  {{{0, 0, 0, 0, 0, 0, 0},
                                                    {0, 0, 0, 1, 0, 0, 0},
                                                    {0, 0, 1, 1, 1, 0, 0},
                                                    {0, 0, 0, 1, 0, 0, 0}}},
                                                  {{{0, 0, 0, 0, 0, 0, 0},
                                                    {0, 0, 0, 0, 1, 0, 0},
                                                    {0, 0, 0, 0, 1, 0, 0},
                                                    {0, 0, 1, 1, 1, 0, 0}}},
                                                  {{{0, 0, 1, 0, 0, 0, 0},
                                                    {0, 0, 1, 0, 0, 0, 0},
                                                    {0, 0, 1, 0, 0, 0, 0},
                                                    {0, 0, 1, 0, 0, 0, 0}}},
                                                  {{{0, 0, 0, 0, 0, 0, 0},
                                                    {0, 0, 0, 0, 0, 0, 0},
                                                    {0, 0, 1, 1, 0, 0, 0},
                                                    {0, 0, 1, 1, 0, 0, 0}}}}};
    assert(index < 5);
    return _[index];
}

static bool isCollisionAhead(vector<vector<uint32_t>> const& field,
                             vector<vector<uint32_t>> const& rock,
                             size_t currentLine) {
    if (currentLine == 0) {
        return true;
    }
    for (size_t i = 0; i < 7; i++) {
        if ((field[currentLine - 1][i] == 1 && rock[3][i] == 1) ||
            (field[currentLine][i] == 1 && rock[2][i] == 1)) {
            return true;
        }
    }
    return false;
}

static void setRockInField(vector<vector<uint32_t>>& field,
                           vector<vector<uint32_t>> const& rock,
                           size_t currentLine) {
    for (size_t i = 0; i < 4; i++) {
        for (size_t j = 0; j < 7; j++) {
            if (rock[3 - i][j] == 1) {
                field[currentLine + i][j] = rock[3 - i][j];
            }
        }
    }
}

static vector<vector<uint32_t>> getShiftedRock(vector<vector<uint32_t>> const& field,
                                               vector<vector<uint32_t>> const& rock,
                                               bool shiftRight,
                                               size_t currentLine) {
    vector<vector<uint32_t>> tempRock;
    if (shiftRight) {
        for (size_t i = 0; i < 4; i++) {
            if (rock[i][6] == 1) {
                // no more shifts
                return rock;
            }
            tempRock.push_back({0, 0, 0, 0, 0, 0, 0});
            bool rightLimitFound(false);
            for (int j = 5; j >= 0; j--) {
                if (!rightLimitFound && rock[i][j] == 1) {
                    rightLimitFound = true;
                    if (field[currentLine + 3 - i][j + 1] == 1) {
                        // no more shifts
                        return rock;
                    }
                }
                tempRock[i][j + 1] = rock[i][j];
            }
        }
    } else {
        for (size_t i = 0; i < 4; i++) {
            if (rock[i][0] == 1) {
                // no more shifts
                return rock;
            }
            tempRock.push_back({0, 0, 0, 0, 0, 0, 0});
            bool leftLimitFound(false);
            for (size_t j = 1; j < 7; j++) {
                if (!leftLimitFound && rock[i][j] == 1) {
                    leftLimitFound = true;
                    if (field[currentLine + 3 - i][j - 1] == 1) {
                        // no more shifts
                        return rock;
                    }
                }
                tempRock[i][j - 1] = rock[i][j];
            }
        }
    }
    return tempRock;
}

static stringstream prettyPrint(vector<vector<uint32_t>> const& x) {
    stringstream s;
    for (int32_t i = x.size() - 1; i >= 0; i--) {
        s << i << "|";
        for (auto const& j : x[i]) {
            if (j == 0) {
                s << ".";
            } else {
                s << "#";
            }
        }
        s << "|\n";
    }
    return s;
}

static bool isRightMove(string const& input, size_t pos) {
    // string s;
    // if (input[pos] == '>') {
    //     s = "right";
    // } else {
    //     s = "left";
    // }
    // cout << "checked pos: " << pos << "\t" << s << "\n";
    return input[pos] == '>';
}

static void removeEmptyLines(vector<vector<uint32_t>>& field) {
    bool startDeletion(true);
    size_t initialSize(field.size());
    for (size_t i = initialSize - 1; i >= initialSize - 4 && startDeletion; i--) {
        for (size_t j = 0; j < 7; j++) {
            if (field[i][j] == 1) {
                startDeletion = false;
                break;
            }
        }
        if (startDeletion) {
            field.pop_back();
        }
    }
}

static bool doesThePatternRepeat(vector<vector<uint32_t>> const& field, size_t& returnedLength) {
    if (field.size() == 0) {
        return false;
    }

    size_t maximalSubstringLength(field.size() / 2);
    // cout << "maximalSubstringLength : " << maximalSubstringLength << endl;

    for (size_t length = maximalSubstringLength; length > 3; length--) {
        bool repeat(true);
        // cout << "\t" << length << endl;
        for (int i = field.size() - 1; i >= field.size() - length; i--) {
            // cout << "\t\t"
            //      << "comparing line: " << i << " with: " << i - length << endl;
            if (field[i] != field[i - length]) {
                repeat = false;
                break;
            }
        }
        if (repeat) {
            returnedLength = length;
            cout << "found cycle length: " << length << endl;
            return true;
        }
    }
    returnedLength = 0;
    return false;
}

static bool dropARock(vector<vector<uint32_t>>& field,
                      uint64_t& counter,
                      uint64_t& moveCounter,
                      string const& inputString,
                      size_t maxHeight = UINT64_MAX) {
    auto currentRock(getRock(counter % 5));
    field.push_back({0, 0, 0, 0, 0, 0, 0});
    field.push_back({0, 0, 0, 0, 0, 0, 0});
    field.push_back({0, 0, 0, 0, 0, 0, 0});
    field.push_back({0, 0, 0, 0, 0, 0, 0});
    size_t currentLine(field.size() - 4);

    for (size_t i = 0; i < 4; i++) {
        currentRock =
            getShiftedRock(field, currentRock,
                           isRightMove(inputString, moveCounter % inputString.size()), currentLine);
        moveCounter++;
    }

    while (!isCollisionAhead(field, currentRock, currentLine)) {
        currentLine--;
        currentRock =
            getShiftedRock(field, currentRock,
                           isRightMove(inputString, moveCounter % inputString.size()), currentLine);
        moveCounter++;
    }

    // this is ew
    vector<vector<uint32_t>> tempField(field);
    setRockInField(tempField, currentRock, currentLine);
    removeEmptyLines(tempField);

    if (tempField.size() <= maxHeight) {
        field = tempField;
        counter++;
    } else {
        removeEmptyLines(field);
        return true;
    }
    return false;
}

int main() {
    std::fstream inputFile("../input/day17.in", std::ios_base::in);
    if (!inputFile.is_open()) {
        std::cerr << "Couldn't open input file" << std::endl;
        return EXIT_FAILURE;
    }

    std::string inputString;
    std::getline(inputFile, inputString);

    vector<vector<uint32_t>> field;
    uint64_t counter(0);
    uint64_t moveCounter(0);
    size_t cycleLength(0);

    while (!doesThePatternRepeat(field, cycleLength)) {
        dropARock(field, counter, moveCounter, inputString);
    }

    uint64_t savedHeight(field.size());
    uint64_t previousCounter(counter);
    cout << "savedHeight: " << savedHeight << endl;

    bool limitReached(false);
    while (!limitReached && field.size() <= savedHeight + cycleLength) {
        limitReached =
            dropARock(field, counter, moveCounter, inputString, savedHeight + cycleLength);
    }
    cout << "field size: " << field.size() << endl;

    // cout << prettyPrint(field).str() << endl;

    uint64_t stonesPerCycle(counter - previousCounter);
    cout << "stonesPerCycle " << stonesPerCycle << "\n";

    uint64_t alreadyFallenStones(counter);

    cout << "alreadyFallenStones " << alreadyFallenStones << "\n";
    uint64_t remainingStones(1000000000000 - alreadyFallenStones);
    cout << "remainingStones " << remainingStones << "\n";

    uint64_t fullCyclesLeft = remainingStones / stonesPerCycle;

    uint64_t remainingDrops = remainingStones % stonesPerCycle;

    cout << "remainingDrops: " << remainingDrops << "\n";

    uint64_t newCounter(0);
    while (newCounter < remainingDrops) {
        dropARock(field, counter, moveCounter, inputString);
        newCounter++;
    }

    uint64_t currentHeight(field.size());
    cout << "last height: " << currentHeight << endl;
    cout << fullCyclesLeft * cycleLength + currentHeight << "\n";
    return EXIT_SUCCESS;
}
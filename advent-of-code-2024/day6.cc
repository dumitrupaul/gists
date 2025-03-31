#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <fstream>
#include <future>
#include <iostream>
#include <list>
#include <map>
#include <print>
#include <queue>
#include <ranges>
#include <set>
#include <sstream>
#include <stack>
#include <string>
#include <unordered_map>
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

enum class LookDirection { UP, DOWN, LEFT, RIGHT };

static unordered_map<char, LookDirection> const DIRECTION_MAP{{'^', LookDirection::UP},
                                                              {'v', LookDirection::DOWN},
                                                              {'>', LookDirection::RIGHT},
                                                              {'<', LookDirection::LEFT}};

optional<pair<int, int>> locateGuard(vector<string> const& area) {
    for (size_t i = 0; i < area.size(); i++) {
        for (size_t j = 0; j < area[i].size(); j++) {
            for (auto const& guardStance : DIRECTION_MAP) {
                if (guardStance.first == area[i][j]) {
                    println("found guard: {}, {}", i, j);
                    return pair{i, j};
                }
            }
        }
    }
    return nullopt;
}

LookDirection turn(vector<string> const& area, LookDirection dir) {
    LookDirection newDir{LookDirection::RIGHT};
    if (dir == LookDirection::UP)
        newDir = LookDirection::RIGHT;
    else if (dir == LookDirection::RIGHT)
        newDir = LookDirection::DOWN;
    else if (dir == LookDirection::DOWN)
        newDir = LookDirection::LEFT;
    else if (dir == LookDirection::LEFT)
        newDir = LookDirection::UP;
    return newDir;
}

bool advance(vector<string> const& area, int& i, int& j, LookDirection& dir) {
    static unordered_map<LookDirection, pair<int, int>> const moves{{LookDirection::UP, {-1, 0}},
                                                                    {LookDirection::DOWN, {1, 0}},
                                                                    {LookDirection::RIGHT, {0, 1}},
                                                                    {LookDirection::LEFT, {0, -1}}};

    auto [di, dj] = moves.at(dir);
    int ni = i + di, nj = j + dj;

    if (ni < 0 || ni >= area.size() || nj < 0 || nj >= area[0].size()) {
        return false;
    }
    if (area[ni][nj] != '#') {
        i = ni;
        j = nj;
    } else {
        dir = turn(area, dir);
    }
    return true;
}

// Check if the guard is ever found in a loop before exiting the area
bool isLooping(vector<string> const& area, pair<int, int> guardPosition, LookDirection direction) {
    bool canAdvance{true};
    set<tuple<int, int, LookDirection>> visited;
    int currentPosi = guardPosition.first;
    int currentPosj = guardPosition.second;
    visited.insert({currentPosi, currentPosj, direction});  // first guard pos
    while (canAdvance) {
        canAdvance = advance(area, currentPosi, currentPosj, direction);
        if (canAdvance && visited.contains({currentPosi, currentPosj, direction})) {
            // we have a loop since we have seen this [position, direction] before
            println("we have seen {}:{}:{} before", currentPosi, currentPosj,
                    static_cast<int>(direction));
            return true;
        }
        visited.insert({currentPosi, currentPosj, direction});
    }

    return false;
}

uint32_t countLoops(vector<string>& area) {
    bool canAdvance{true};
    auto [guardPosi, guardPosj] = locateGuard(area).value();
    auto direction{DIRECTION_MAP.at(area[guardPosi][guardPosj])};
    set<pair<int, int>> placedObjects;

    auto tryPlacing = [&area, &placedObjects](int guardPosi, int guardPosj, LookDirection dir) {
        static const unordered_map<LookDirection, pair<int, int>> moves{
            {LookDirection::UP, {-1, 0}},
            {LookDirection::DOWN, {1, 0}},
            {LookDirection::RIGHT, {0, 1}},
            {LookDirection::LEFT, {0, -1}}};
        auto [di, dj] = moves.at(dir);
        int ni = guardPosi + di, nj = guardPosj + dj;

        if (ni < 0 || ni >= area.size() || nj < 0 || nj >= area[0].size() || area[ni][nj] != '.') {
            return;
        }

        area[ni][nj] = '#';
        if (!placedObjects.contains({ni, nj}) && isLooping(area, {guardPosi, guardPosj}, dir)) {
            placedObjects.insert({ni, nj});
        }
        area[ni][nj] = '.';
    };

    // Try and place an obstacle in front of the guard and check for a loop from that position
    // Remove that obstacle after the check and continue through the area
    while (canAdvance) {
        tryPlacing(guardPosi, guardPosj, direction);
        // mark the position so we don't attempt to place obstacles again
        area[guardPosi][guardPosj] = 'X';
        canAdvance = advance(area, guardPosi, guardPosj, direction);
    }

    return placedObjects.size();
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
    vector<string> area;
    while (std::getline(inputFile, inputString)) {
        area.push_back(inputString);
    }

    println("solution: {}", countLoops(area));
    return EXIT_SUCCESS;
}
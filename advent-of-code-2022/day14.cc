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
using Terrain = map<tuple<uint32_t, uint32_t>, bool>;
using Coord = tuple<uint32_t, uint32_t>;

bool isFreefalling(Terrain const& terrain, Coord const& previousCoord, Coord const& currentCoord) {
    for (auto const& [t, _] : terrain) {
        if (abs(static_cast<int32_t>(get<0>(t) - get<0>(currentCoord))) <
                abs(static_cast<int32_t>(get<0>(t) - get<0>(previousCoord))) ||
            abs(static_cast<int32_t>(get<1>(t) - get<1>(currentCoord))) <
                abs(static_cast<int32_t>(get<1>(t) - get<1>(previousCoord)))) {
            return false;
        }
    }
    return true;
}

bool isTerrainThere(Terrain const& terrain, Coord const& there, uint32_t maxY = UINT_MAX) {
    if (std::get<1>(there) >= maxY)
        return true;
    return terrain.contains(there);
}

bool isSourceBlocked(Terrain const& terrain) {
    static Coord source{500, 0};
    return terrain.contains(source);
}

uint32_t getMaxY(Terrain const& terrain) {
    uint32_t maxY(0);
    for (auto const& [t, _] : terrain) {
        if (maxY < get<1>(t)) {
            maxY = get<1>(t);
        }
    }
    return maxY + 2;
}

bool dropSand(Terrain& terrain) {
    Coord previousPosition{500, 0};
    Coord currentPosition(previousPosition);
    bool canMove(true);

    std::unordered_map<string, function<Coord(Coord const&)>> moveMap{
        {"down",
         [](Coord const& c) -> Coord {
             return {get<0>(c), get<1>(c) + 1};
         }},
        {"down-left",
         [](Coord const& c) -> Coord {
             return {get<0>(c) - 1, get<1>(c) + 1};
         }},
        {"down-right", [](Coord const& c) -> Coord {
             return {get<0>(c) + 1, get<1>(c) + 1};
         }}};

    if (!isTerrainThere(terrain, moveMap["down"](currentPosition))) {
        currentPosition = moveMap["down"](currentPosition);
    } else if (!isTerrainThere(terrain, moveMap["down-left"](currentPosition))) {
        currentPosition = moveMap["down-left"](currentPosition);
    } else if (!isTerrainThere(terrain, moveMap["down-right"](currentPosition))) {
        currentPosition = moveMap["down-right"](currentPosition);
    }

    while (!isFreefalling(terrain, previousPosition, currentPosition)) {
        previousPosition = currentPosition;
        if (!isTerrainThere(terrain, moveMap["down"](currentPosition))) {
            currentPosition = moveMap["down"](currentPosition);
        } else if (!isTerrainThere(terrain, moveMap["down-left"](currentPosition))) {
            currentPosition = moveMap["down-left"](currentPosition);
        } else if (!isTerrainThere(terrain, moveMap["down-right"](currentPosition))) {
            currentPosition = moveMap["down-right"](currentPosition);
        } else {
            canMove = false;
            terrain[currentPosition] = true;
            break;
        }
    }

    return canMove;
}

bool dropSandPart2(Terrain& terrain, uint32_t maxY) {
    Coord currentPosition{500, 0};
    bool canMove(true);

    std::unordered_map<string, function<Coord(Coord const&)>> moveMap{
        {"down",
         [](Coord const& c) -> Coord {
             return {get<0>(c), get<1>(c) + 1};
         }},
        {"down-left",
         [](Coord const& c) -> Coord {
             return {get<0>(c) - 1, get<1>(c) + 1};
         }},
        {"down-right", [](Coord const& c) -> Coord {
             return {get<0>(c) + 1, get<1>(c) + 1};
         }}};

    if (!isTerrainThere(terrain, moveMap["down"](currentPosition), maxY)) {
        currentPosition = moveMap["down"](currentPosition);
    } else if (!isTerrainThere(terrain, moveMap["down-left"](currentPosition), maxY)) {
        currentPosition = moveMap["down-left"](currentPosition);
    } else if (!isTerrainThere(terrain, moveMap["down-right"](currentPosition), maxY)) {
        currentPosition = moveMap["down-right"](currentPosition);
    }

    while (!isSourceBlocked(terrain)) {
        if (!isTerrainThere(terrain, moveMap["down"](currentPosition), maxY)) {
            currentPosition = moveMap["down"](currentPosition);
        } else if (!isTerrainThere(terrain, moveMap["down-left"](currentPosition), maxY)) {
            currentPosition = moveMap["down-left"](currentPosition);
        } else if (!isTerrainThere(terrain, moveMap["down-right"](currentPosition), maxY)) {
            currentPosition = moveMap["down-right"](currentPosition);
        } else {
            canMove = false;
            terrain[currentPosition] = true;
            break;
        }
    }

    return canMove;
}

int main() {
    std::fstream inputFile("../input/day14.in", std::ios_base::in);
    if (!inputFile.is_open()) {
        std::cerr << "Couldn't open input file" << std::endl;
        return EXIT_FAILURE;
    }

    std::string inputString;
    Terrain lockedTerrain;

    while (std::getline(inputFile, inputString)) {
        vector<string> temp;
        vector<string> coordPairs;
        boost::split(temp, inputString, boost::is_any_of("-> "));
        for (size_t i = 0; i < temp.size(); i++) {
            if (temp[i].empty()) {
                continue;
            } else {
                coordPairs.push_back(temp[i]);
            }
        }

        for (size_t i = 0; i < coordPairs.size() - 1; i++) {
            vector<string> pair;
            boost::split(pair, coordPairs[i], boost::is_any_of(","));
            uint32_t startX = stoul(pair[0]);
            uint32_t startY = stoul(pair[1]);
            pair.clear();
            boost::split(pair, coordPairs[i + 1], boost::is_any_of(","));
            uint32_t endX = stoul(pair[0]);
            uint32_t endY = stoul(pair[1]);
            if (startX == endX) {
                for (uint32_t c = min(startY, endY); c <= max(startY, endY); c++) {
                    lockedTerrain[{startX, c}] = true;
                }
            } else {
                for (uint32_t c = min(startX, endX); c <= max(startX, endX); c++) {
                    lockedTerrain[{c, startY}] = true;
                }
            }
        }
    }

    uint32_t i(0);
    static const uint32_t maxY = getMaxY(lockedTerrain);
    while (!dropSandPart2(lockedTerrain, maxY)) {
        i++;
    }
    cout << i << endl;

    return EXIT_SUCCESS;
}
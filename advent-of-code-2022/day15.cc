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
using Coord = tuple<int64_t, int64_t>;
using Sensor = map<Coord, int64_t>;

int64_t getManhattanDistance(Coord const& sensor, Coord const& beacon) {
    return abs(get<0>(sensor) - get<0>(beacon)) + abs(get<1>(sensor) - get<1>(beacon));
}

bool isInside(Sensor const& sensors, Coord const& point) {
    auto [lookingForX, lookingForY] = point;
    for (auto const& [sensor, md] : sensors) {
        int64_t startX(get<0>(sensor) - md);
        int64_t endX(get<0>(sensor) + md);
        int64_t startY(get<1>(sensor) - md);
        int64_t endY(get<1>(sensor) + md);
        if (lookingForY >= startY && lookingForY <= endY) {
            int64_t numberOfOccupiedPositions =
                abs(endX - startX) - (abs(lookingForY - get<1>(sensor)) * 2);
            int64_t fromX(get<0>(sensor) - (numberOfOccupiedPositions / 2));
            int64_t toX(fromX + numberOfOccupiedPositions);
            if (lookingForX >= fromX && lookingForX <= toX) {
                return true;
            }
        }
    }
    return false;
}

Coord getXbyLine(Coord sensor, Coord xOfSensor, int64_t line, int64_t maxX = 20) {
    int64_t numberOfOccupiedPositions =
        abs(get<1>(xOfSensor) - get<0>(xOfSensor)) - (abs(line - get<1>(sensor)) * 2);
    int64_t fromX(get<0>(sensor) - (numberOfOccupiedPositions / 2));
    int64_t toX(fromX + numberOfOccupiedPositions);
    if (fromX < 0)
        fromX = 0;
    if (toX > maxX)
        toX = maxX;
    return {fromX, toX};
}

Coord getBeaconLocation(Sensor const& sensors, int64_t maxX = 20, int64_t maxY = 20) {
    for (auto const& [sensor, md] : sensors) {
        int64_t startX(get<0>(sensor) - md);
        int64_t endX(get<0>(sensor) + md);
        int64_t startY(get<1>(sensor) - md);
        int64_t endY(get<1>(sensor) + md);
        if (startY < 0 && endY < 0) {
            continue;
        }
        if (startY > maxY && endY > maxY) {
            continue;
        }
        if (startY < 0)
            startY = 0;
        if (endY > maxY)
            endY = maxY;

        for (int64_t pos = startY; pos <= get<1>(sensor); pos++) {
            auto [fromX, toX] = getXbyLine(sensor, {startX, endX}, pos, maxX);
            if (!isInside(sensors, {toX + 1, pos})) {
                return {toX + 1, pos};
            }
            if (!isInside(sensors, {fromX - 1, pos})) {
                return {fromX + 1, pos};
            }
        }

        for (int64_t pos = get<1>(sensor); pos <= endY; pos++) {

            auto [fromX, toX] = getXbyLine(sensor, {startX, endX}, pos, maxX);
            if (!isInside(sensors, {toX + 1, pos})) {
                return {toX + 1, pos};
            }
            if (!isInside(sensors, {fromX - 1, pos})) {
                return {fromX - 1, pos};
            }
        }
    }
    return {-1, -1};
}

int main() {
    std::fstream inputFile("../input/day15.in", std::ios_base::in);
    if (!inputFile.is_open()) {
        std::cerr << "Couldn't open input file" << std::endl;
        return EXIT_FAILURE;
    }

    std::string inputString;

    Sensor sensors;
    while (std::getline(inputFile, inputString)) {
        smatch sm;
        regex_search(inputString, sm,
                     regex("Sensor at x=(.*), y=(.*): closest beacon is at x=(.*), y=(.*)"));
        Coord sensor{stoi(sm[1]), stoi(sm[2])};
        Coord beacon{stoi(sm[3]), stoi(sm[4])};
        sensors[sensor] = getManhattanDistance(sensor, beacon);
    }

    auto [x, y] = getBeaconLocation(sensors, 4000000, 4000000);
    cout << x << ", " << y << endl;

    return EXIT_SUCCESS;
}
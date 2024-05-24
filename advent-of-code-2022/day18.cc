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

struct Cube {
    Cube(int32_t x, int32_t y, int32_t z) : x_(x), y_(y), z_(z) {
    }
    Cube() = default;
    int32_t x_;
    int32_t y_;
    int32_t z_;

    array<bool, 6> connectedSides_{{false, false, false, false, false, false}};

    friend ostream& operator<<(ostream& os, const Cube& dt);
};

ostream& operator<<(ostream& os, const Cube& dt) {
    os << dt.x_ << ',' << dt.y_ << ',' << dt.z_;
    return os;
}

static void checkAndSetNeighbour(Cube& c1, Cube& c2) {
    if (c1.y_ == c2.y_ && c1.z_ == c2.z_) {
        if (c1.x_ == c2.x_ - 1) {
            c1.connectedSides_[3] = true;
            c2.connectedSides_[1] = true;
        } else if (c1.x_ == c2.x_ + 1) {
            c1.connectedSides_[1] = true;
            c2.connectedSides_[3] = true;
        }
    } else if (c1.x_ == c2.x_ && c1.z_ == c2.z_) {
        if (c1.y_ == c2.y_ - 1) {
            c1.connectedSides_[5] = true;
            c2.connectedSides_[4] = true;

        } else if (c1.y_ == c2.y_ + 1) {
            c1.connectedSides_[4] = true;
            c2.connectedSides_[5] = true;
        }
    } else if (c1.x_ == c2.x_ && c1.y_ == c2.y_) {
        if (c1.z_ == c2.z_ - 1) {
            c1.connectedSides_[2] = true;
            c2.connectedSides_[0] = true;
        } else if (c1.z_ == c2.z_ + 1) {
            c1.connectedSides_[0] = true;
            c2.connectedSides_[2] = true;
        }
    }
}

static void setNeighbours(vector<Cube>& cubes) {
    for (size_t i = 0; i < cubes.size() - 1; ++i) {
        for (size_t j = i + 1; j < cubes.size(); ++j) {
            checkAndSetNeighbour(cubes[i], cubes[j]);
        }
    }
}

static vector<vector<vector<bool>>> get3Dmap(vector<Cube> const& cubes) {
    int32_t maxX(0), maxY(0), maxZ(0);
    for (auto const& c : cubes) {
        if (c.x_ > maxX)
            maxX = c.x_;
        if (c.y_ > maxY)
            maxY = c.y_;
        if (c.z_ > maxZ)
            maxZ = c.z_;
    }
    // access is [z][y][x]
    vector<vector<vector<bool>>> resultedMap(
        maxZ + 1, vector<vector<bool>>(maxY + 1, vector<bool>(maxX + 1, false)));
    for (auto const& c : cubes) {
        resultedMap[c.z_][c.y_][c.x_] = true;
    }
    return resultedMap;
}

static bool getSurroundedCubesRecursive(vector<vector<vector<bool>>>& cubeMap,
                                        vector<vector<vector<bool>>>& visited,
                                        size_t x,
                                        size_t y,
                                        size_t z) {

    if (!cubeMap[z][y][x] && (x == 0 || y == 0 || z == 0 || z == cubeMap.size() - 1 ||
                              y == cubeMap[z].size() - 1 || x == cubeMap[z][y].size() - 1)) {
        return false;
    }

    if (visited[z][y][x] || cubeMap[z][y][x]) {
        return true;
    }

    visited[z][y][x] = true;
    bool endResult(true);
    endResult &= getSurroundedCubesRecursive(cubeMap, visited, x - 1, y, z);
    endResult &= getSurroundedCubesRecursive(cubeMap, visited, x + 1, y, z);
    endResult &= getSurroundedCubesRecursive(cubeMap, visited, x, y - 1, z);
    endResult &= getSurroundedCubesRecursive(cubeMap, visited, x, y + 1, z);
    endResult &= getSurroundedCubesRecursive(cubeMap, visited, x, y, z - 1);
    endResult &= getSurroundedCubesRecursive(cubeMap, visited, x, y, z + 1);
    return endResult;
}

static uint32_t getUnconnectedSides(vector<Cube> const& cubes) {
    uint32_t count(0);
    for (auto const& c : cubes) {
        for (auto const& side : c.connectedSides_) {
            if (side == false) {
                count++;
            }
        }
    }
    return count;
}

static uint32_t getSurroundedCubesUnconnectedSides(vector<vector<vector<bool>>>& cubeMap) {
    vector<Cube> surroundedCubes;
    for (size_t z = 0; z < cubeMap.size(); ++z) {
        for (size_t y = 0; y < cubeMap[z].size(); ++y) {
            for (size_t x = 0; x < cubeMap[z][y].size(); ++x) {
                vector<vector<vector<bool>>> visited(
                    cubeMap.size(),
                    vector<vector<bool>>(cubeMap[0].size(),
                                         vector<bool>(cubeMap[0][0].size(), false)));
                if (!cubeMap[z][y][x] && getSurroundedCubesRecursive(cubeMap, visited, x, y, z)) {
                    surroundedCubes.push_back(Cube(x, y, z));
                }
            }
        }
    }

    setNeighbours(surroundedCubes);
    return getUnconnectedSides(surroundedCubes);
}

[[maybe_unused]] static stringstream prettyPrint(vector<vector<vector<bool>>> const& cubeMap) {
    stringstream s;
    for (auto const& z : cubeMap) {
        for (auto const& y : z) {
            for (auto const& x : y) {
                if (x) {
                    s << "#";
                } else {
                    s << "-";
                }
            }
            s << "\n";
        }
        s << "\n";
    }
    return s;
}

int main() {
    std::fstream inputFile("../input/day18.in", std::ios_base::in);
    if (!inputFile.is_open()) {
        std::cerr << "Couldn't open input file" << std::endl;
        return EXIT_FAILURE;
    }

    string inputString;
    vector<Cube> cubes;
    while (getline(inputFile, inputString)) {
        vector<string> temp;
        boost::split(temp, inputString, boost::is_any_of(","));
        Cube tempCube(stoi(temp[0]), stoi(temp[1]), stoi(temp[2]));
        cubes.emplace_back(tempCube);
    }
    setNeighbours(cubes);
    auto cubeMap(get3Dmap(cubes));

    cout << "exteriorArea "
         << getUnconnectedSides(cubes) - getSurroundedCubesUnconnectedSides(cubeMap) << endl;

    return EXIT_SUCCESS;
}
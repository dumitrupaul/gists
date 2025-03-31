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

uint32_t countAntinodes(vector<vector<bool>> const& antinodeMatrix) {
    uint32_t count{0};
    for (auto const& line : antinodeMatrix) {
        for (auto const& node : line) {
            if (node)
                count++;
        }
    }
    return count;
}

bool isInBounds(pair<int, int> const& node, int i, int j) {
    return node.first >= 0 && node.second >= 0 && node.first < i && node.second < j;
}

void markAntiNodesForAntenna(vector<pair<int, int>> const& antennas,
                             vector<vector<bool>>& antinodesMatrix) {
    for (int i = 0; i < antennas.size(); i++) {
        int firstAntennaI{antennas[i].first}, firstAntennaJ{antennas[i].second};
        for (int j = i + 1; j < antennas.size(); j++) {
            int secondAntennaI{antennas[j].first}, secondAntennaJ{antennas[j].second};
            pair<int, int> distance{secondAntennaI - firstAntennaI, secondAntennaJ - firstAntennaJ};

            for (int k = 0; k < antinodesMatrix.size(); k++) {
                pair<int, int> firstAntiNode{secondAntennaI - k * distance.first,
                                             secondAntennaJ - k * distance.second};
                pair<int, int> secondAntiNode{secondAntennaI + k * distance.first,
                                              secondAntennaJ + k * distance.second};
                // println("{} with {}: first {}, second {}", antennas[i], antennas[j],
                // firstAntiNode,
                //         secondAntiNode);
                if (isInBounds(firstAntiNode, antinodesMatrix.size(), antinodesMatrix[0].size())) {
                    antinodesMatrix[firstAntiNode.first][firstAntiNode.second] = true;
                }
                if (isInBounds(secondAntiNode, antinodesMatrix.size(), antinodesMatrix[0].size())) {
                    antinodesMatrix[secondAntiNode.first][secondAntiNode.second] = true;
                }
            }
        }
    }
}

vector<vector<bool>> generateAntiNodes(vector<string> const& antennaMatrix) {
    vector<vector<bool>> antinodesMatrix(antennaMatrix.size(),
                                         vector<bool>(antennaMatrix[0].size(), false));
    unordered_map<char, vector<pair<int, int>>> antennaPositions;
    for (int i = 0; i < antennaMatrix.size(); i++) {
        for (int j = 0; j < antennaMatrix[i].size(); j++) {
            if (antennaMatrix[i][j] != '.')
                antennaPositions[antennaMatrix[i][j]].push_back({i, j});
        }
    }

    for (auto const& [antenna, antennaPos] : antennaPositions) {
        markAntiNodesForAntenna(antennaPos, antinodesMatrix);
    }

    return antinodesMatrix;
}

int main() {
    static string const INPUT_FILE{"advent-of-code-2024/input/day" +
                                   std::string{getDayNumber(__FILE__)} + ".in"};
    std::fstream inputFile(INPUT_FILE.data(), std::ios_base::in);
    if (!inputFile.is_open()) {
        std::cerr << "Couldn't open input file" << std::endl;
        return EXIT_FAILURE;
    }

    vector<string> antennaMatrix;
    string inputString;
    while (std::getline(inputFile, inputString)) {
        antennaMatrix.push_back(inputString);
    }

    auto result = generateAntiNodes(antennaMatrix);

    println("{}", countAntinodes(result));

    return EXIT_SUCCESS;
}
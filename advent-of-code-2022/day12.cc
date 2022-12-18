#include <algorithm>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <cmath>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <queue>
#include <regex>
#include <stack>
#include <string>
#include <string_view>
#include <unordered_set>
#include <variant>
#include <vector>

struct Step {
    int32_t row = 0;
    int32_t col = 0;
    int32_t distanceFromStart = 0;
};

bool operator>(Step const& o1, Step const& o2) {
    return o1.distanceFromStart > o2.distanceFromStart;
}

std::vector<std::tuple<int32_t, int32_t>>
getValidNeighbours(std::vector<std::vector<int32_t>> const& matrix, int32_t i, int32_t j) {
    std::vector<std::tuple<int32_t, int32_t>> resultedNeighbours;
    if (i + 1 < matrix.size() && (matrix[i + 1][j] + 1 >= matrix[i][j])) {
        resultedNeighbours.push_back({i + 1, j});
    }
    if (i - 1 >= 0 && (matrix[i - 1][j] + 1 >= matrix[i][j])) {
        resultedNeighbours.push_back({i - 1, j});
    }
    if (j + 1 < matrix[0].size() && (matrix[i][j + 1] + 1 >= matrix[i][j])) {
        resultedNeighbours.push_back({i, j + 1});
    }
    if (j - 1 >= 0 && (matrix[i][j - 1] + 1 >= matrix[i][j])) {
        resultedNeighbours.push_back({i, j - 1});
    }
    return resultedNeighbours;
}

// part 2
uint32_t getStepsWithBfs(std::vector<std::vector<int32_t>>& matrix,
                         int32_t startRow,
                         int32_t startCol,
                         int32_t endRow,
                         int32_t endCol) {
    assert(matrix.size() >= 1);
    std::vector<std::vector<std::tuple<int32_t, int32_t>>> parents(
        matrix.size(), std::vector<std::tuple<int32_t, int32_t>>(matrix[0].size(), {0, 0}));
    std::vector<std::vector<bool>> visited(matrix.size(),
                                           std::vector<bool>(matrix[0].size(), false));
    std::queue<std::tuple<int32_t, int32_t>> q;
    q.push({startRow, startCol});
    visited[startRow][startCol] = true;
    int32_t foundRow(0), foundCol(0);

    while (!q.empty()) {
        auto [i, j] = q.front();
        q.pop();

        if (matrix[i][j] == 97) {
            foundRow = i;
            foundCol = j;
            break;
        }

        for (auto const& [neighbourI, neighbourJ] : getValidNeighbours(matrix, i, j)) {
            if (!visited[neighbourI][neighbourJ]) {
                q.push({neighbourI, neighbourJ});
                parents[neighbourI][neighbourJ] = {i, j};
                visited[neighbourI][neighbourJ] = true;
            }
        }
    }

    int32_t i(foundRow), j(foundCol);
    uint32_t steps(0);
    while (!(i == startRow && j == startCol)) {
        std::tie(i, j) = parents[i][j];
        steps++;
    }
    return steps;
}

int main() {
    std::fstream inputFile("../input/day12.in", std::ios_base::in);
    if (!inputFile.is_open()) {
        std::cerr << "Couldn't open input file" << std::endl;
        return EXIT_FAILURE;
    }

    std::string inputString;
    std::vector<std::vector<int32_t>> matrix;
    int32_t startRow(0), startCol(0), endRow(0), endCol(0);
    int32_t i(0), j(0);
    while (inputFile >> inputString) {
        j = 0;
        std::vector<int32_t> line;
        for (auto& c : inputString) {
            if (c == 'S') {
                startRow = i;
                startCol = j;
                c = 'a';
            } else if (c == 'E') {
                endRow = i;
                endCol = j;
                c = 'z';
            }
            line.push_back(c);
            j++;
        }
        matrix.push_back(line);
        i++;
    }

    std::cout << getStepsWithBfs(matrix, endRow, endCol, 0, 0) << std::endl;
    return EXIT_SUCCESS;
}
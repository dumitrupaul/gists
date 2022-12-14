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

void drawPositions(int32_t const headCurrentPosX,
                   int32_t const headCurrentPosY,
                   int32_t const tailCurrentPosX,
                   int32_t const tailCurrentPosY) {
    std::cout << "T: " << tailCurrentPosX << "," << tailCurrentPosY << std::endl;
    std::cout << "H: " << headCurrentPosX << "," << headCurrentPosY << std::endl;
}

void updateTailPosition(int32_t const headCurrentPosX,
                        int32_t const headCurrentPosY,
                        int32_t& tailCurrentPosX,
                        int32_t& tailCurrentPosY,
                        std::vector<std::tuple<int32_t, int32_t>>& visitedPositions) {
    if (std::abs(tailCurrentPosX - headCurrentPosX) + std::abs(tailCurrentPosY - headCurrentPosY) >
        2) {
        if (headCurrentPosX > tailCurrentPosX)
            tailCurrentPosX++;
        else
            tailCurrentPosX--;
        if (headCurrentPosY > tailCurrentPosY)
            tailCurrentPosY++;
        else
            tailCurrentPosY--;
    } else if (headCurrentPosX == tailCurrentPosX && tailCurrentPosY != headCurrentPosY &&
               std::abs(tailCurrentPosY - headCurrentPosY) > 1) {
        if (headCurrentPosY > tailCurrentPosY)
            tailCurrentPosY++;
        else
            tailCurrentPosY--;
    } else if (headCurrentPosY == tailCurrentPosY && headCurrentPosX != tailCurrentPosX &&
               std::abs(tailCurrentPosX - headCurrentPosX) > 1) {
        if (headCurrentPosX > tailCurrentPosX)
            tailCurrentPosX++;
        else
            tailCurrentPosX--;
    }
}

int main() {
    std::fstream inputFile("../input/day9.in", std::ios_base::in);
    if (!inputFile.is_open()) {
        std::cerr << "Couldn't open input file" << std::endl;
        return EXIT_FAILURE;
    }

    std::string inputString;
    char direction;
    uint32_t steps;

    int32_t headCurrentPosX(0), headCurrentPosY(0);
    std::vector<std::tuple<int32_t, int32_t>> visitedPositions{
        std::tie(headCurrentPosX, headCurrentPosY)};

    std::vector<std::tuple<int32_t, int32_t>> tailsPositions(9, {0, 0});

    while (inputFile >> direction >> steps) {
        for (uint32_t i = 0; i < steps; i++) {
            switch (direction) {
            case 'R':
                headCurrentPosX++;
                break;
            case 'L':
                headCurrentPosX--;
                break;
            case 'U':
                headCurrentPosY++;
                break;
            case 'D':
                headCurrentPosY--;
                break;
            }
            updateTailPosition(headCurrentPosX, headCurrentPosY, std::get<0>(tailsPositions[0]),
                               std::get<1>(tailsPositions[0]), visitedPositions);
            for (uint32_t j = 1; j < 9; j++) {
                updateTailPosition(std::get<0>(tailsPositions[j - 1]),
                                   std::get<1>(tailsPositions[j - 1]),
                                   std::get<0>(tailsPositions[j]), std::get<1>(tailsPositions[j]),
                                   visitedPositions);
            }
            bool found(false);
            for (auto const& [x, y] : visitedPositions) {
                if (std::get<0>(tailsPositions[8]) == x && std::get<1>(tailsPositions[8]) == y) {
                    found = true;
                }
            }
            if (!found) {
                visitedPositions.push_back(
                    std::tie(std::get<0>(tailsPositions[8]), std::get<1>(tailsPositions[8])));
            }
        }
    }

    // for (auto const& [posX, posY] : visitedPositions) {
    //     std::cout << posX << " " << posY << std::endl;
    // }
    std::cout << visitedPositions.size();

    return EXIT_SUCCESS;
}
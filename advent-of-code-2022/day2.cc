#include <algorithm>
#include <cmath>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

int main() {
    std::fstream inputFile("../input/day2.in", std::ios_base::in);
    if (!inputFile.is_open()) {
        std::cerr << "Couldn't open input file" << std::endl;
        return EXIT_FAILURE;
    }

    static std::unordered_map<std::string, uint32_t> assignedPoints{{"A", 1}, {"B", 2}, {"C", 3}};
    static std::unordered_map<std::string, std::string> winningMoves{
        {"A", "B"}, {"B", "C"}, {"C", "A"}};
    static std::unordered_map<std::string, std::string> losingMoves{
        {"A", "C"}, {"B", "A"}, {"C", "B"}};

    // A - rock
    // B - paper
    // C - scissor
    // X - rock
    // Y - paper
    // Z - scissor
    std::string inputString;
    uint32_t totalScore(0);
    while (inputFile >> inputString) {
        std::string responseString;
        inputFile >> responseString;
        if (responseString == "Z") {
            totalScore += 6 + assignedPoints[winningMoves[inputString]];
        } else if (responseString == "Y") {
            totalScore += 3 + assignedPoints[inputString];
        } else {
            totalScore += assignedPoints[losingMoves[inputString]];
        }
    }

    std::cout << totalScore << std::endl;
    return EXIT_SUCCESS;
}
#include <algorithm>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <cmath>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <regex>
#include <stack>
#include <string>
#include <string_view>
#include <unordered_set>
#include <vector>

std::vector<std::stack<char>> parseStackConfig(std::vector<std::string> const& stackConfig) {
    uint32_t numberOfStacks = (stackConfig[stackConfig.size() - 1].size() + 1) / 4;
    std::vector<std::stack<char>> stacks(numberOfStacks, std::stack<char>());
    for (int32_t i = stackConfig.size() - 2; i >= 0; i--) {
        for (uint32_t j = 0; j < numberOfStacks; j++) {
            if (std::isalpha(stackConfig[i][j * 4 + 1])) {
                stacks[j].push(stackConfig[i][j * 4 + 1]);
            }
        }
    }

    return stacks;
}

void executeMoves(std::vector<std::string> moves, std::vector<std::stack<char>>& stacks) {
    for (auto const& move : moves) {
        if (move.empty()) {
            continue;
        }
        std::smatch sm;
        std::regex_search(move, sm, std::regex("move (.*?) from (.?) to (.?)"));
        uint32_t numberOfMoves = std::stoul(sm[1]);
        uint32_t moveFrom = std::stoul(sm[2]);
        uint32_t moveTo = std::stoul(sm[3]);
        for (uint32_t i = 0; i < numberOfMoves; i++) {
            stacks[moveTo - 1].push(stacks[moveFrom - 1].top());
            stacks[moveFrom - 1].pop();
        }
    }
}

void executeMovesPart2(std::vector<std::string> moves, std::vector<std::stack<char>>& stacks) {
    for (auto const& move : moves) {
        if (move.empty()) {
            continue;
        }
        std::smatch sm;
        std::regex_search(move, sm, std::regex("move (.*?) from (.?) to (.?)"));
        uint32_t numberOfMoves = std::stoul(sm[1]);
        uint32_t moveFrom = std::stoul(sm[2]);
        uint32_t moveTo = std::stoul(sm[3]);
        std::stack<char> tempStack;
        for (uint32_t i = 0; i < numberOfMoves; i++) {
            tempStack.push(stacks[moveFrom - 1].top());
            stacks[moveFrom - 1].pop();
        }
        for (uint32_t i = 0; i < numberOfMoves; i++) {
            stacks[moveTo - 1].push(tempStack.top());
            tempStack.pop();
        }
    }
}

int main() {
    std::fstream inputFile("../input/day5.in", std::ios_base::in);
    if (!inputFile.is_open()) {
        std::cerr << "Couldn't open input file" << std::endl;
        return EXIT_FAILURE;
    }

    std::string inputString;
    std::vector<std::string> stackConfig;
    std::vector<std::string> moves;
    bool startReadingMoves(false);
    while (std::getline(inputFile, inputString)) {
        if (inputString.empty()) {
            startReadingMoves = true;
        }
        if (startReadingMoves) {
            moves.push_back(inputString);
        } else {
            stackConfig.push_back(inputString);
        }
    }
    auto stacks = parseStackConfig(stackConfig);
    executeMovesPart2(moves, stacks);

    for (auto const& stack : stacks) {
        if (!stack.empty()) {
            std::cout << stack.top();
        }
    }
    std::cout << std::endl;

    return EXIT_SUCCESS;
}
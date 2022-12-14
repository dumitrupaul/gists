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
#include <vector>

struct Operation {
    enum class OperationType { ADDX, NOOP };

    explicit Operation(OperationType type = OperationType::NOOP,
                       uint32_t cycleDuration = 1,
                       int32_t operand = 0)
        : type_(type), operand_(operand), cycleDuration_(cycleDuration) {
    }

    OperationType type_;
    int32_t operand_;
    uint32_t cycleDuration_;
};

int main() {
    std::fstream inputFile("../input/day10.in", std::ios_base::in);
    if (!inputFile.is_open()) {
        std::cerr << "Couldn't open input file" << std::endl;
        return EXIT_FAILURE;
    }

    std::queue<Operation> executionQueue;

    std::string operation;
    int32_t operand(0);

    while (std::getline(inputFile, operation)) {
        if (operation == "noop") {
            executionQueue.push(Operation());
        } else {
            std::stringstream ss;
            ss << operation;
            ss >> operation >> operand;
            if (operation == "addx") {
                executionQueue.push(Operation(Operation::OperationType::ADDX, 2, operand));
            }
        }
    }

    uint32_t cycleNumber(1);
    int32_t registerX(1);
    std::string screen("#");
    while (!executionQueue.empty()) {
        if (executionQueue.front().cycleDuration_ == 1) {
            if (executionQueue.front().type_ == Operation::OperationType::ADDX) {
                registerX += executionQueue.front().operand_;
            }
            executionQueue.pop();
        } else {
            executionQueue.front().cycleDuration_--;
        }

        if (cycleNumber < 240) {
            if (cycleNumber % 40 == registerX - 1 || cycleNumber % 40 == registerX ||
                cycleNumber % 40 == registerX + 1) {
                if (cycleNumber % 40 == 0)
                    screen += "\n";
                screen += "#";
            } else {
                screen += ".";
            }
        }
        cycleNumber++;
    }

    std::cout << screen << std::endl;

    return EXIT_SUCCESS;
}
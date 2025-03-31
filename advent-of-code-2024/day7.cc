#include <__ostream/print.h>
#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <deque>
#include <fmt/chrono.h>
#include <fstream>
#include <future>
#include <iostream>
#include <list>
#include <map>
#include <print>
#include <ranges>
#include <sstream>
#include <string>
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

enum class Operation { ADD = 0, MUL = 1, CONCAT = 2, NR_OF_ITEMS = 3 };
using Operands = vector<uint64_t>;
using OperationsType = pair<uint64_t, Operands>;
using OperationList = deque<Operation>;

bool evaluateAllPermutations(uint64_t endValue, Operands const& operands) {
    if (operands.size() < 2) {
        return operands.empty() ? false : operands[0] == endValue;
    }

    auto concatenate = [](uint64_t i, uint64_t j) {
        uint64_t result{i};
        string jString{to_string(j)};
        reverse(jString.begin(), jString.end());
        while (!jString.empty()) {
            result = result * 10 + (jString.back() - '0');
            jString.pop_back();
        }
        return result;
    };

    uint32_t operationsSize{static_cast<uint32_t>(operands.size()) - 1U};
    vector<Operation> permutation(operationsSize, Operation::ADD);
    while (true) {
        OperationList resultedOperations(permutation.begin(), permutation.end());
        // for (auto const& op : resultedOperations) {
        //     print("{},", op == Operation::ADD ? '+' : (op == Operation::MUL ? '*' : '|'));
        // }
        // println();

        uint64_t value{operands[0]};
        for (auto const& operand : operands | ranges::views::drop(1)) {
            if (resultedOperations.front() == Operation::ADD) {
                value += operand;
            } else if (resultedOperations.front() == Operation::MUL) {
                value *= operand;
            } else {
                value = concatenate(value, operand);
            }

            resultedOperations.pop_front();
            if (value > endValue)
                break;
        }

        if (value == endValue) {
            return true;
        }

        // Find the rightmost element that can be incremented
        int index = operationsSize - 1;
        while (index >= 0 && permutation[index] == Operation::CONCAT) {
            permutation[index] = Operation::ADD;  // Reset to ADD
            --index;  // Move to the next "digit"
        }

        // If we reached the leftmost digit, we're done
        if (index < 0)
            break;

        // Increment the current "digit"
        if (permutation[index] == Operation::ADD) {
            permutation[index] = Operation::MUL;
        } else if (permutation[index] == Operation::MUL) {
            permutation[index] = Operation::CONCAT;
        }
    }

    return false;
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
    vector<OperationsType> input;
    while (std::getline(inputFile, inputString)) {
        auto colonPos{inputString.find(':')};
        uint64_t endValue{stoull(inputString.substr(0, colonPos))};
        auto const& operatorsString{inputString.substr(colonPos + 1)};
        stringstream s{operatorsString};
        string number;
        vector<uint64_t> operators;
        while (getline(s, number, ' ')) {
            try {
                operators.push_back(stoull(number));
            } catch (...) {
                continue;
            }
        }
        input.emplace_back(OperationsType{std::move(endValue), std::move(operators)});
    }

    uint64_t part1Sol{0};
    for (auto const& [value, line] : input) {
        bool evaluation{evaluateAllPermutations(value, line)};
        if (evaluation)
            part1Sol += value;
        println("{} = {} - > {}", value, line, evaluation);
    }

    println("Solution: {}", part1Sol);

    return EXIT_SUCCESS;
}
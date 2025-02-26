#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <fmt/chrono.h>
#include <fstream>
#include <future>
#include <iostream>
#include <istream>
#include <list>
#include <map>
#include <optional>
#include <ranges>
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

struct OperationClosure {
    uint32_t operandA;
    uint32_t operandB;
    size_t size;
};

optional<OperationClosure> isValidOp(string const& memory) {
    size_t backPos{4};
    if (memory.size() <= backPos)
        return nullopt;
    size_t frontPos = memory.find_first_of(',');
    if (frontPos == string::npos) {
        return nullopt;
    }
    if (frontPos - backPos < 1 || frontPos - backPos > 4) {
        return nullopt;
    }

    string first{memory.substr(backPos, frontPos - backPos)};
    backPos = frontPos;
    frontPos = memory.find_first_of(')');
    if (frontPos == string::npos) {
        return nullopt;
    }
    if (frontPos - backPos < 1 || frontPos - backPos > 4) {
        return nullopt;
    }

    string second{memory.substr(backPos + 1, frontPos - backPos + 1)};

    try {
        return optional<OperationClosure>{
            {static_cast<uint32_t>(stoul(first)), static_cast<uint32_t>(stoul(second)), frontPos}};
    } catch (invalid_argument&) {
        return nullopt;
    }

    return nullopt;
}

enum class Operation { Multiply, Do, Dont };
static unordered_map<string, Operation> const OperationMap{
    {"mul(", Operation::Multiply}, {"do()", Operation::Do}, {"don't()", Operation::Dont}};

list<pair<uint32_t, uint32_t>> parseOperations(string const& memory) {
    list<pair<uint32_t, uint32_t>> opList;

    // Find the earliest occurrence of any substring
    size_t searchIndex{0};
    auto searchLamda = [&memory, &searchIndex](string const& sub) {
        size_t ret = memory.find(sub, searchIndex);
        if (ret == string::npos) {
            return numeric_limits<size_t>::max();
        } else {
            return ret;
        }
    };
    auto const& keys = views::keys(OperationMap);
    bool countMultiplication{true};
    for (auto result = ranges::min_element(keys, {}, searchLamda); result != keys.end();
         result = ranges::min_element(keys, {}, searchLamda)) {
        size_t foundIndex = memory.find(*result, searchIndex);
        if (foundIndex == string::npos) {
            break;
        }
        if (OperationMap.at(*result) == Operation::Do) {
            searchIndex = foundIndex + 4;
            countMultiplication = true;
        } else if (OperationMap.at(*result) == Operation::Dont) {
            searchIndex = foundIndex + 7;
            countMultiplication = false;
        } else {
            auto closure = isValidOp(memory.substr(foundIndex));
            if (closure) {
                if (countMultiplication) {
                    opList.push_back({closure.value().operandA, closure.value().operandB});
                }
                searchIndex = foundIndex + closure.value().size;
            } else {
                searchIndex = foundIndex + 4;
            }
        }
    }

    return opList;
}

int main() {
    static string const INPUT_FILE{"advent-of-code-2024/input/day" +
                                   std::string{getDayNumber(__FILE__)} + ".in"};
    std::fstream inputFile(INPUT_FILE.data(), std::ios_base::in);
    if (!inputFile.is_open()) {
        std::cerr << "Couldn't open input file" << std::endl;
        return EXIT_FAILURE;
    }

    stringstream input;
    input << inputFile.rdbuf();

    auto opList = parseOperations(input.str());
    uint64_t sum{0};
    for (auto const& op : opList) {
        sum += (op.first * op.second);
    }

    cout << "Sum: " << sum << endl;

    return EXIT_SUCCESS;
}
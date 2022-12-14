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

struct Monkey {
    enum class OperationType { SUM, MUL };
    struct Operands {
        std::variant<std::string, uint64_t> a_;
        std::variant<std::string, uint64_t> b_;
    };

    std::vector<uint64_t> startingItems_;
    std::tuple<OperationType, Operands> operation_;
    uint64_t testDivision_;
    uint64_t trueBranch_;
    uint64_t falseBranch_;
    uint64_t inspectedItems_ = 0;
};

bool checkForInitialNumbersOfItems(std::vector<uint64_t> const& initialConfiguration,
                                   std::vector<Monkey> const& monkeys) {
    for (size_t i = 0; i <= monkeys.size(); i++) {
        if (initialConfiguration[i] != monkeys[i].startingItems_.size()) {
            return false;
        }
    }
    return true;
}

void processMonkeys(std::vector<Monkey>& monkeys, uint64_t highestMultiple) {
    for (size_t i = 0; i < monkeys.size(); i++) {
        for (size_t itemIdx = 0; itemIdx < monkeys[i].startingItems_.size(); itemIdx++) {
            monkeys[i].inspectedItems_++;

            uint64_t a(0);
            if (std::get<1>(monkeys[i].operation_).a_.index() == 0) {
                a = monkeys[i].startingItems_[itemIdx];
            } else {
                a = std::get<uint64_t>(std::get<1>(monkeys[i].operation_).a_);
            }

            uint64_t b(0);
            if (std::get<1>(monkeys[i].operation_).b_.index() == 0) {
                b = monkeys[i].startingItems_[itemIdx];
            } else {
                b = std::get<uint64_t>(std::get<1>(monkeys[i].operation_).b_);
            }

            uint64_t newWorryLevel(0);
            if (std::get<0>(monkeys[i].operation_) == Monkey::OperationType::SUM) {
                newWorryLevel = a + b;
            } else {
                newWorryLevel = a * b;
            }

            newWorryLevel = newWorryLevel % highestMultiple;
            if (newWorryLevel % monkeys[i].testDivision_ == 0) {
                monkeys[monkeys[i].trueBranch_].startingItems_.push_back(newWorryLevel);
            } else {
                monkeys[monkeys[i].falseBranch_].startingItems_.push_back(newWorryLevel);
            }
        }
        monkeys[i].startingItems_.clear();
    }
}

static bool sortByInspections(const Monkey& d1, const Monkey& d2) {
    return d1.inspectedItems_ > d2.inspectedItems_;
}

int main() {
    std::fstream inputFile("../input/day11.in", std::ios_base::in);
    if (!inputFile.is_open()) {
        std::cerr << "Couldn't open input file" << std::endl;
        return EXIT_FAILURE;
    }

    std::string inputString;

    std::unique_ptr<Monkey> monkeyToReadInto(std::make_unique<Monkey>());
    std::vector<Monkey> monkeys;
    while (std::getline(inputFile, inputString)) {
        if (inputString.empty()) {
            monkeys.push_back(*monkeyToReadInto);
            monkeyToReadInto = std::make_unique<Monkey>();
        }
        std::smatch sm;
        if (std::regex_search(inputString, sm, std::regex("Monkey (.*):"))) {

        } else if (std::regex_search(inputString, sm, std::regex("Starting items: (.*)"))) {
            std::vector<std::string> splitted;
            boost::split(splitted, std::string(sm[1]), boost::is_any_of(","));
            for (auto const& s : splitted) {
                if (!s.empty()) {
                    monkeyToReadInto->startingItems_.push_back(std::stoul(s));
                }
            }
        } else if (std::regex_search(inputString, sm,
                                     std::regex("Operation: new = (.*) (.?) (.*)"))) {
            Monkey::OperationType operationType(Monkey::OperationType::MUL);
            if (sm[2] == "+") {
                operationType = Monkey::OperationType::SUM;
            }

            Monkey::Operands operands;
            if (sm[1] == "old") {
                operands.a_ = "old";
            } else {
                operands.a_ = std::stoul(sm[1]);
            }
            if (sm[3] == "old") {
                operands.b_ = "old";
            } else {
                operands.b_ = std::stoul(sm[3]);
            }

            monkeyToReadInto->operation_ = std::tie(operationType, operands);
        } else if (std::regex_search(inputString, sm, std::regex("Test: divisible by (.*)"))) {
            monkeyToReadInto->testDivision_ = std::stoul(sm[1]);
        } else if (std::regex_search(inputString, sm,
                                     std::regex("If true: throw to monkey (.*)"))) {
            monkeyToReadInto->trueBranch_ = std::stoul(sm[1]);
        } else if (std::regex_search(inputString, sm,
                                     std::regex("If false: throw to monkey (.*)"))) {
            monkeyToReadInto->falseBranch_ = std::stoul(sm[1]);
        }
    }
    monkeys.push_back(*monkeyToReadInto);

    uint64_t highestMultiple(1);
    for (auto const& m : monkeys) {
        highestMultiple *= m.testDivision_;
    }

    for (uint32_t i = 0; i < 10000; i++) {
        processMonkeys(monkeys, highestMultiple);
    }

    std::sort(monkeys.begin(), monkeys.end(), sortByInspections);
    std::cout << monkeys[0].inspectedItems_ * monkeys[1].inspectedItems_ << std::endl;

    return EXIT_SUCCESS;
}
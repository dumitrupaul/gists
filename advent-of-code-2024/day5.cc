#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <fmt/chrono.h>
#include <fmt/format.h>
#include <fstream>
#include <future>
#include <iostream>
#include <list>
#include <map>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
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

bool validate(unordered_map<uint32_t, vector<uint32_t>> const& rules,
              vector<uint32_t> const& page) {
    unordered_set<uint32_t> presentNumbers{page.begin(), page.end()};
    unordered_set<uint32_t> visitedNumbers;

    for (auto const& elem : page) {
        visitedNumbers.insert(elem);
        if (!rules.contains(elem)) {
            continue;
        }
        auto const& elementsBefore(rules.at(elem));
        for (auto const& before : elementsBefore) {
            // if we have seen the element that needs to be before, its valid
            if (visitedNumbers.contains(before)) {
                continue;
            }
            // if we havent seen it but the element is present in the page, page is invalid
            if (presentNumbers.contains(before)) {
                return false;
            }
        }
    }
    return true;
}

vector<uint32_t> reorder(unordered_map<uint32_t, vector<uint32_t>> const& rules,
                         vector<uint32_t> const& page) {
    vector<uint32_t> returnedPage;
    unordered_set<uint32_t> presentNumbers{page.begin(), page.end()};
    unordered_map<uint32_t, vector<uint32_t>> dependencies;
    map<size_t, vector<uint32_t>> sortedDeps;
    // map reduce rules
    // @dependencies is a map that describes ONLY the dependencies of the given input page
    // there is only ONE number with a certain COUNT
    // so our maps don't even need a list as a value and makes the problem much easier
    for (auto const& number : page) {
        if (rules.contains(number)) {
            for (auto const& before : rules.at(number)) {
                if (presentNumbers.contains(before)) {
                    dependencies[number].push_back(before);
                }
            }
            if (dependencies.contains(number)) {
                size_t deps{dependencies.at(number).size()};
                sortedDeps[deps].push_back(number);
            } else {
                sortedDeps[0].push_back(number);
            }
        } else {
            sortedDeps[0].push_back(number);
        }
    }

    // re-create list, NO NEED FOR RECURSION since the input list is crafted so that there are no
    // outliers and fits perfectly ordered as a dependency tree
    // eg: if you add all numbers with 3
    // dependencies you can proceed in adding all numbers with 4 dependencies because their
    // dependencies are garenteed to be satisfied
    // that's because there's only ONE number with a certain COUNT
    for (auto const& [count, numberList] : sortedDeps) {
        returnedPage.insert(returnedPage.end(), numberList.cbegin(), numberList.cend());
    }

    return returnedPage;
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
    // {number, [all required numbers before it]}
    unordered_map<uint32_t, vector<uint32_t>> rules;
    while (std::getline(inputFile, inputString) && !inputString.empty()) {
        auto firstNum(stoul(inputString.substr(0, 2)));
        auto secondNum(stoul(inputString.substr(3, 2)));
        if (rules.contains(secondNum)) {
            rules[secondNum].push_back(firstNum);
        } else {
            rules.insert({static_cast<uint32_t>(secondNum), {static_cast<uint32_t>(firstNum)}});
        }
    }

    vector<vector<uint32_t>> pages;
    size_t i = 0;
    while (std::getline(inputFile, inputString)) {
        stringstream ss(inputString);
        string item;
        pages.push_back({});
        while (getline(ss, item, ',')) {
            pages[i].emplace_back(stoul(item));
        }
        i++;
    }

    uint32_t result{0};
    for (auto const& page : pages) {
        if (!validate(rules, page)) {
            auto newPage{reorder(rules, page)};
            result += newPage[newPage.size() / 2];
        }
    }

    print("{}\n", result);

    return EXIT_SUCCESS;
}
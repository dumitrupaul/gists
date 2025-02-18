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

vector<list<int64_t>> getHistories(list<int64_t> const& input) {
    vector<list<int64_t>> histories;
    histories.push_back(input);
    bool stopHistory{false};
    size_t hIdx{0};
    while (!stopHistory) {
        bool allZeros{true};
        histories.push_back({});
        auto previousValue{histories[hIdx].front()};
        for (auto iter = histories[hIdx].begin(); iter != histories[hIdx].end(); ++iter) {
            if (iter == histories[hIdx].begin()) {
                continue;
            }
            histories[hIdx + 1].push_back(*iter - previousValue);
            if (*iter - previousValue != 0) {
                allZeros = false;
            }
            previousValue = *iter;
        }
        if (allZeros) {
            stopHistory = true;
        }
        hIdx++;
    }
    return histories;
}

int64_t getExtrapolatedLastValue(list<int64_t> const& input) {
    auto histories{getHistories(input)};

    for (int64_t i = histories.size() - 3; i >= 0; i--) {
        histories[i].push_back(histories[i + 1].back() + histories[i].back());
    }

    return histories[0].back();
}

int64_t getExtrapolatedPreviousValue(list<int64_t> const& input) {
    auto histories{getHistories(input)};

    for (int64_t i = histories.size() - 3; i >= 0; i--) {
        histories[i].push_front(histories[i].front() - histories[i + 1].front());
    }

    return histories[0].front();
}

int main() {
    static string const INPUT_FILE{"advent-of-code-2023/input/day" +
                                   std::string{getDayNumber(__FILE__)} + ".in"};
    std::fstream inputFile(INPUT_FILE.data(), std::ios_base::in);
    if (!inputFile.is_open()) {
        std::cerr << "Couldn't open input file" << std::endl;
        return EXIT_FAILURE;
    }

    string inputString;
    vector<list<int64_t>> inputs;
    while (std::getline(inputFile, inputString)) {
        string number;
        istringstream stream{inputString};
        list<int64_t> input;
        while (getline(stream, number, ' ')) {
            input.push_back(stoll(number));
        }
        inputs.push_back(move(input));
    }

    int64_t sum{0};

    for (auto const& input : inputs) {
        sum += getExtrapolatedPreviousValue(input);
    }
    cout << sum << endl;

    return EXIT_SUCCESS;
}
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <map>
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

static unordered_map<std::string, std::string> numberMap{
    {"one", "1"}, {"two", "2"},   {"three", "3"}, {"four", "4"}, {"five", "5"},
    {"six", "6"}, {"seven", "7"}, {"eight", "8"}, {"nine", "9"},
};

int main() {
    static string INPUT_FILE{"advent-of-code-2023/input/day" + std::string{getDayNumber(__FILE__)} +
                             ".in"};
    std::fstream inputFile(INPUT_FILE.c_str(), std::ios_base::in);
    if (!inputFile.is_open()) {
        std::cerr << "Couldn't open input file" << std::endl;
        return EXIT_FAILURE;
    }

    string inputString;
    uint64_t sum{0};
    while (std::getline(inputFile, inputString)) {
        string firstNumber;
        string lastNumber;
        for (int32_t i = 0; i < inputString.size(); i++) {
            if (inputString[i] >= 48 && inputString[i] <= 57) {
                if (firstNumber.empty()) {
                    firstNumber = inputString[i];
                }
                lastNumber = inputString[i];
            } else {
                for (auto const& [key, value] : numberMap) {
                    int32_t j = i;
                    bool foundSpelled{true};
                    for (auto const& c : key) {
                        if (j >= inputString.size() || inputString[j] != c) {
                            foundSpelled = false;
                            break;
                        }
                        j++;
                    }
                    if (foundSpelled) {
                        if (firstNumber.empty()) {
                            firstNumber = value;
                        }
                        lastNumber = value;
                    }
                }
            }
        }
        cout << "number:" << firstNumber << lastNumber << endl;
        sum += std::stoul(firstNumber + lastNumber);
    }

    cout << sum << endl;

    return EXIT_SUCCESS;
}
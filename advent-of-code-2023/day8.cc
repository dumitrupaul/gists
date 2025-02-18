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
#include <numeric>
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

struct Destination {
    string left;
    string right;
};

// Function to calculate LCM of a list of numbers
uint64_t lcmOfList(std::vector<uint64_t> const& numbers) {
    return std::accumulate(numbers.begin(), numbers.end(), 1ULL,
                           [](uint64_t a, uint64_t b) { return std::lcm(a, b); });
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
    string instructions;
    std::getline(inputFile, instructions);

    unordered_map<string, Destination> nodes;

    while (std::getline(inputFile, inputString)) {
        if (inputString.empty()) {
            continue;
        }
        nodes[inputString.substr(0, 3)] = {inputString.substr(7, 3), inputString.substr(12, 3)};
    }

    vector<string> paths;

    for (auto const& [source, destination] : nodes) {
        if (source[2] == 'A') {
            paths.push_back(source);
        }
    }

    vector<uint64_t> pathSteps;
    for (size_t i = 0; i < paths.size(); i++) {
        uint64_t steps{0};
        while (paths[i][2] != 'Z') {
            if (instructions[steps % instructions.size()] == 'R') {
                paths[i] = nodes[paths[i]].right;
            } else {
                paths[i] = nodes[paths[i]].left;
            }
            steps++;
        }
        pathSteps.push_back(steps);
    }

    cout << "steps: " << lcmOfList(pathSteps) << endl;

    return EXIT_SUCCESS;
}
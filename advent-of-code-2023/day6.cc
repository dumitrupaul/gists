#include <algorithm>
#include <cmath>
#include <cstdint>
#include <fstream>
#include <iostream>
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

struct Move {
    unsigned long time{0};
    unsigned long recordDistance{0};
    unsigned long solutions{0};
};

int main() {
    static string const INPUT_FILE{"advent-of-code-2023/input/day" +
                                   std::string{getDayNumber(__FILE__)} + ".in"};
    std::fstream inputFile(INPUT_FILE.data(), std::ios_base::in);
    if (!inputFile.is_open()) {
        std::cerr << "Couldn't open input file" << std::endl;
        return EXIT_FAILURE;
    }

    vector<Move> moves;
    string inputString;
    getline(inputFile, inputString);
    inputString = inputString.substr(inputString.find_first_of("123456789"));
    inputString.erase(remove_if(inputString.begin(), inputString.end(), ::isspace),
                      inputString.end());
    moves.push_back({stoul(inputString)});

    getline(inputFile, inputString);
    inputString = inputString.substr(inputString.find_first_of("123456789"));
    inputString.erase(remove_if(inputString.begin(), inputString.end(), ::isspace),
                      inputString.end());
    moves[0].recordDistance = stoul(inputString);

    for (auto& move : moves) {
        unsigned long time{move.time};
        for (unsigned long speed = 0; speed < move.time && time > 0; speed++, time--) {
            unsigned long distance = speed * time;
            if (distance > move.recordDistance) {
                move.solutions++;
            }
        }
    }

    unsigned long solution{1};
    for (auto const& move : moves) {
        solution *= move.solutions;
    }

    cout << "solution: " << solution << endl;

    return EXIT_SUCCESS;
}
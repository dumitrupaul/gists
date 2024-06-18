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

struct Game {
    struct Set {
        uint32_t nrOfRedCubes{0};
        uint32_t nrOfGreenCubes{0};
        uint32_t nrOfBlueCubes{0};
    };

    vector<Set> sets;
};

Game::Set parseSet(string const& stringSet) {
    Game::Set returnedSet;
    istringstream stringSetStream(stringSet);
    string pair;
    while (getline(stringSetStream, pair, ',')) {
        pair.erase(0, 1);
        istringstream pairStream(pair);
        string cubesNumber;
        string cubeType;
        getline(pairStream, cubesNumber, ' ');
        getline(pairStream, cubeType, ' ');
        if (cubeType == "red") {
            returnedSet.nrOfRedCubes = stoul(cubesNumber);
        } else if (cubeType == "green") {
            returnedSet.nrOfGreenCubes = stoul(cubesNumber);
        } else if (cubeType == "blue") {
            returnedSet.nrOfBlueCubes = stoul(cubesNumber);
        }
    }
    return returnedSet;
}

bool isGamePossible(Game const& game, Game::Set const& configurationSet) {
    for (auto const& set : game.sets) {
        if (set.nrOfBlueCubes > configurationSet.nrOfBlueCubes ||
            set.nrOfGreenCubes > configurationSet.nrOfGreenCubes ||
            set.nrOfRedCubes > configurationSet.nrOfRedCubes) {
            return false;
        }
    }
    return true;
}

Game::Set getMinConfiguration(Game const& game) {
    Game::Set minSet;
    for (auto const& set : game.sets) {
        if (set.nrOfBlueCubes > minSet.nrOfBlueCubes) {
            minSet.nrOfBlueCubes = set.nrOfBlueCubes;
        }
        if (set.nrOfGreenCubes > minSet.nrOfGreenCubes) {
            minSet.nrOfGreenCubes = set.nrOfGreenCubes;
        }
        if (set.nrOfRedCubes > minSet.nrOfRedCubes) {
            minSet.nrOfRedCubes = set.nrOfRedCubes;
        }
    }
    return minSet;
}

int main() {
    static string INPUT_FILE{"advent-of-code-2023/input/day" + std::string{getDayNumber(__FILE__)} +
                             ".in"};
    std::fstream inputFile(INPUT_FILE.data(), std::ios_base::in);
    if (!inputFile.is_open()) {
        std::cerr << "Couldn't open input file" << std::endl;
        return EXIT_FAILURE;
    }

    vector<Game> games;
    string inputString;
    while (std::getline(inputFile, inputString)) {
        istringstream g{inputString.substr(inputString.find_first_of(":") + 1)};
        string stringSet;
        Game gameToInsert;
        while (getline(g, stringSet, ';')) {
            gameToInsert.sets.emplace_back(parseSet(stringSet));
        }
        games.push_back(gameToInsert);
    }

    uint64_t solution{0};
    for (size_t i = 0; i < games.size(); i++) {
        auto set = getMinConfiguration(games[i]);
        uint64_t power = set.nrOfBlueCubes * set.nrOfGreenCubes * set.nrOfRedCubes;
        solution += power;
    }

    cout << "solution: " << solution << endl;

    return EXIT_SUCCESS;
}
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

struct GardenMap {
    uint64_t source{0};
    uint64_t destination{0};
    uint64_t size{0};
};

using Pathway = list<list<GardenMap>>;

void addPathwayEntry(fstream& inputFile, Pathway& pathway) {
    string inputString;
    list<GardenMap> listToInsert;
    while (std::getline(inputFile, inputString) && !inputString.empty()) {
        istringstream inputStream{inputString};
        GardenMap gMap;
        inputStream >> gMap.destination >> gMap.source >> gMap.size;
        listToInsert.push_back(move(gMap));
    }
    pathway.push_back(move(listToInsert));
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
    Pathway pathway;
    vector<uint64_t> seeds;
    std::getline(inputFile, inputString);
    istringstream inputStream{inputString.substr(inputString.find_first_of(':') + 2)};
    string number;
    while (getline(inputStream, number, ' ')) {
        seeds.push_back(stoull(number));
    }
    while (std::getline(inputFile, inputString)) {
        if (inputString.find("map") != std::string::npos) {
            addPathwayEntry(inputFile, pathway);
        }
    }

    auto parseInterval([](Pathway const& pathway, uint64_t start, uint64_t end) -> uint64_t {
        uint64_t minLocation{UINT64_MAX};
        auto startTime{chrono::steady_clock::now()};
        for (uint64_t j = start; j < end; j++) {
            uint64_t key{j};
            for (auto const& path : pathway) {
                for (auto const& gardenMap : path) {
                    if (key >= gardenMap.source && key < gardenMap.source + gardenMap.size) {
                        key = gardenMap.destination + (key - gardenMap.source);
                        break;
                    }
                }
            }
            if (minLocation > key) {
                minLocation = key;
            }
        }
        auto endTime{chrono::steady_clock::now()};
        fmt::print(stderr, "From {} to {} took: {:%T}\n", start, end, endTime - startTime);
        return minLocation;
    });

    uint64_t minLocation{UINT64_MAX};
    vector<future<uint64_t>> results;
    for (uint64_t i = 0; i < seeds.size(); i += 2) {
        results.emplace_back(
            async(std::launch::async, parseInterval, pathway, seeds[i], seeds[i] + seeds[i + 1]));
    }

    for (auto& result : results) {
        uint64_t value = result.get();
        if (value < minLocation) {
            minLocation = value;
        }
    }

    cout << "minLocation " << minLocation << endl;

    return EXIT_SUCCESS;
}
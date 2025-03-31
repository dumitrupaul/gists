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
#include <unordered_map>

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

// part 1
uint32_t calculateSafeLevels(vector<vector<uint32_t>> const& reports) {
    uint32_t safe{static_cast<uint32_t>(reports.size())};
    enum class Order{Unknown, Increasing, Decreasing};
    for (int i = 0; i < reports.size(); i++) {
        auto order = Order::Unknown;
        for (int j = 1; j < reports[i].size(); j++) {
            if(reports[i][j - 1] == reports[i][j]) {
                safe--;
                break;
            }
            if(order == Order::Unknown) {
                if (reports[i][j - 1] > reports[i][j]) order = Order::Decreasing;
                else order = Order::Increasing;
            } else if (order == Order::Increasing) {
                if (reports[i][j - 1] > reports[i][j]) {
                    safe--;
                    break;
                }
            } else {
                if (reports[i][j - 1] < reports[i][j]) {
                    safe--;
                    break;
                }
            }

            if (abs(static_cast<int32_t>(reports[i][j - 1] - reports[i][j])) > 3) {
                safe--;
                break;
            }
        }
    }
    return safe;
}

// part 2
uint32_t calculateTolerateLevels(vector<vector<uint32_t>>& reports, vector<uint32_t>& marks) {
    uint32_t safe{static_cast<uint32_t>(reports.size())};
    enum class Order{Unknown, Increasing, Decreasing};
    for (int i = 0; i < reports.size(); i++) {
        auto order = Order::Unknown;
        bool oneMistake{false};
        bool failed{false};
        for (int j = 1; j < reports[i].size(); j++) {
            if(reports[i][j - 1] == reports[i][j]) {
                if(oneMistake){
                    safe--;
                    failed = true;
                    break;
                } else {
                    oneMistake = true;
                    continue;
                }
            }
            if(order == Order::Unknown) {
                // Establish order
                if (reports[i][j - 1] > reports[i][j]) order = Order::Decreasing;
                else order = Order::Increasing;
            } else if (order == Order::Increasing) {
                if (reports[i][j - 1] > reports[i][j]) {
                    if(oneMistake) {
                        safe--;
                        failed = true;
                        break;
                    } else {
                        reports[i][j] = reports[i][j - 1];
                        oneMistake = true;
                        continue;
                    }
                }
            } else {
                if (reports[i][j - 1] < reports[i][j]) {
                    if(oneMistake) {
                        safe--;
                        failed = true;
                        break;
                    } else {
                        reports[i][j] = reports[i][j - 1];
                        oneMistake = true;
                        continue;
                    }
                }
            }

            if (abs(static_cast<int32_t>(reports[i][j - 1] - reports[i][j])) > 3) {
                if(oneMistake){
                    safe--;
                    failed = true;
                    break;
                } else {
                    reports[i][j] = reports[i][j - 1];
                    oneMistake = true;
                    continue;
                }
            }
        }

        if(!failed) {
            marks[i] = 1;
        }
    }
    return safe;
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
    vector<vector<uint32_t>> reports;
    while (std::getline(inputFile, inputString)) {
        vector<uint32_t> report;
        istringstream inputStream{inputString};
        while(!inputStream.eof()) {
            uint32_t number{0};
            inputStream >> number;
            report.push_back(number);
        }
        reports.push_back(report);
        report.clear();
    }


    vector<vector<uint32_t>> reversedReports;
    for(auto const& report : reports) {
        vector<uint32_t> dest(report.size());
        reverse_copy(report.begin(), report.end(), dest.begin());
        reversedReports.emplace_back(std::move(dest));
    }

    // mark the safe reports
    vector<uint32_t> marks(reports.size(), 0);

    // for both normal (falure case eg: 8 1 2 3 4 5 6 since direction is given by first 2 elements)
    cout << calculateTolerateLevels(reports, marks) << endl;

    // and reversed vectors
    cout << calculateTolerateLevels(reversedReports, marks) << endl;

    uint32_t finalValue{0};
    for(auto const& mark : marks) {
        if (mark == 1) {
            finalValue++;
        }
    }

    cout << finalValue << endl;

    return EXIT_SUCCESS;
}
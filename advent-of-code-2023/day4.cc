#include <algorithm>
#include <cmath>
#include <cstdint>
#include <fstream>
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

int main() {
    static string const INPUT_FILE{"advent-of-code-2023/input/day" +
                                   std::string{getDayNumber(__FILE__)} + ".in"};
    std::fstream inputFile(INPUT_FILE.data(), std::ios_base::in);
    if (!inputFile.is_open()) {
        std::cerr << "Couldn't open input file" << std::endl;
        return EXIT_FAILURE;
    }

    string inputString;
    std::vector<uint32_t> winningNumbers;
    std::vector<uint32_t> numbers;
    std::map<uint32_t, uint32_t> cardMatchMap;
    std::list<uint32_t> cardListLeft;
    uint32_t cards{0};
    uint32_t i{0};
    while (std::getline(inputFile, inputString)) {
        inputString = inputString.substr(inputString.find_first_of(':') + 2);
        auto const& winningString = inputString.substr(0, inputString.find_first_of('|'));
        auto const& numberString = inputString.substr(inputString.find_first_of('|') + 2);
        std::istringstream winStream(winningString);
        std::string number;

        while (getline(winStream, number, ' ')) {
            try {
                winningNumbers.emplace_back(std::stoul(number));
            } catch (const std::exception&) {
                continue;
            }
        }

        std::istringstream numStream(numberString);
        while (getline(numStream, number, ' ')) {
            try {
                numbers.emplace_back(std::stoul(number));
            } catch (const std::exception&) {
                continue;
            }
        }

        uint32_t numbersMatched{0};
        for (auto const& number : numbers) {
            if (find(winningNumbers.begin(), winningNumbers.end(), number) !=
                winningNumbers.end()) {
                numbersMatched++;
            }
        }
        cardMatchMap[++i] = numbersMatched;
        for (uint32_t j = i + 1; j <= numbersMatched + i; j++) {
            cardListLeft.push_back(j);
        }
        cards++;
        winningNumbers.clear();
        numbers.clear();
    }

    for (auto const& el : cardListLeft) {
        if (cardMatchMap[el] > 0) {
            for (uint32_t j = el + 1; j <= cardMatchMap[el] + el; j++) {
                cardListLeft.push_back(j);
            }
        }
    }
    cards += cardListLeft.size();

    cout << "final score " << cards << endl;
    return EXIT_SUCCESS;
}
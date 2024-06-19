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
#include <set>
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

enum class CompareResult { Stronger, Equal, Weaker };
enum class HandType { High = 0, One, Two, Three, FullHouse, Four, Five };

string HandTypeToString(HandType type) {
    switch (type) {
    case HandType::High:
        return "High card";
    case HandType::One:
        return "One pair";
    case HandType::Two:
        return "Two pair";
    case HandType::Three:
        return "Three of a kind";
    case HandType::FullHouse:
        return "Full house";
    case HandType::Four:
        return "Four of a kind";
    case HandType::Five:
        return "Five of a kind";
    }
    return {};
}

HandType GetHandType(string const& hand) {
    unordered_map<char, int> cardMap{{'A', 0}, {'K', 0}, {'Q', 0}, {'J', 0}, {'T', 0},
                                     {'9', 0}, {'8', 0}, {'7', 0}, {'6', 0}, {'5', 0},
                                     {'4', 0}, {'3', 0}, {'2', 0}};
    for (auto const& card : hand) {
        cardMap[card]++;
    }

    bool hasTwo{false};
    bool hasThree{false};
    for (auto const& [card, appearance] : cardMap) {
        switch (appearance) {
        case 5:
            return HandType::Five;
        case 4:
            return HandType::Four;
        case 3:
            if (hasTwo)
                return HandType::FullHouse;
            else
                hasThree = true;
            break;
        case 2:
            if (hasThree)
                return HandType::FullHouse;
            else if (hasTwo)
                return HandType::Two;
            else
                hasTwo = true;
            break;
        }
    }
    if (hasThree) {
        return HandType::Three;
    }

    if (hasTwo) {
        return HandType::One;
    }

    return HandType::High;
}

HandType GetHandTypePart2(string const& hand) {
    unordered_map<char, int> cardMap{{'A', 0}, {'K', 0}, {'Q', 0}, {'J', 0}, {'T', 0},
                                     {'9', 0}, {'8', 0}, {'7', 0}, {'6', 0}, {'5', 0},
                                     {'4', 0}, {'3', 0}, {'2', 0}};
    for (auto const& card : hand) {
        cardMap[card]++;
    }

    bool hasTwo{false};
    bool hasThree{false};
    int wildcards = cardMap['J'];
    if (wildcards == 5 || wildcards == 4) {
        return HandType::Five;
    }
    for (auto const& [card, appearance] : cardMap) {
        if (card == 'J')
            continue;
        switch (appearance) {
        case 5:
            return HandType::Five;
        case 4:
            if (wildcards == 1) {
                return HandType::Five;
            }
            return HandType::Four;
        case 3:
            if (wildcards == 2)
                return HandType::Five;
            if (wildcards == 1)
                return HandType::Four;
            if (hasTwo)
                return HandType::FullHouse;
            else
                hasThree = true;
            break;
        case 2:
            if (wildcards == 3)
                return HandType::Five;
            if (wildcards == 2)
                return HandType::Four;
            if (hasThree)
                return HandType::FullHouse;
            if (hasTwo) {
                if (wildcards == 1) {
                    return HandType::FullHouse;
                } else
                    return HandType::Two;
            } else
                hasTwo = true;
            break;
        }
    }

    if (wildcards == 3) {
        return HandType::Four;
    }

    if (wildcards == 2) {
        return HandType::Three;
    }

    if (hasThree) {
        return HandType::Three;
    }

    if (hasTwo) {
        if (wildcards == 1) {
            return HandType::Three;
        }
        return HandType::One;
    }

    if (wildcards == 1) {
        return HandType::One;
    }

    return HandType::High;
}

CompareResult CompareCards(char a, char b) {
    // if any of them is a integer then lexicographic compare will work
    if (a == b)
        return CompareResult::Equal;
    // part 2 adaptation
    if (a == 'J')
        a = '1';
    if (b == 'J')
        b = '1';
    if (a <= 57 || b <= 57) {
        return a > b ? CompareResult::Stronger : CompareResult::Weaker;
    }

    // otherwise rank them accordingly
    static const unordered_map<char, int> rankings{{'A', 5}, {'K', 4}, {'Q', 3}, {'T', 1}};
    return rankings.at(a) > rankings.at(b) ? CompareResult::Stronger : CompareResult::Weaker;
}

int main() {
    static string const INPUT_FILE{"advent-of-code-2023/input/day" +
                                   std::string{getDayNumber(__FILE__)} + ".in"};
    std::fstream inputFile(INPUT_FILE.data(), std::ios_base::in);
    if (!inputFile.is_open()) {
        std::cerr << "Couldn't open input file" << std::endl;
        return EXIT_FAILURE;
    }

    auto cmpLambda = [](string const& a, string const& b) -> bool {
        auto handTypeA = GetHandTypePart2(a);
        auto handTypeB = GetHandTypePart2(b);
        if (handTypeA == handTypeB) {
            for (size_t i = 0; i < a.size(); ++i) {
                auto result = CompareCards(a[i], b[i]);
                if (result != CompareResult::Equal) {
                    return result == CompareResult::Stronger ? true : false;
                }
            }
        }

        return handTypeA > handTypeB;
    };

    map<string, uint32_t, decltype(cmpLambda)> hands(cmpLambda);

    string inputString;
    while (std::getline(inputFile, inputString)) {
        hands[inputString.substr(0, 5)] = stoul(inputString.substr(6, string::npos));
    }

    uint64_t rank{hands.size()};
    uint64_t solution = {0};
    for (auto const& [key, value] : hands) {
        cout << key << ": " << HandTypeToString(GetHandTypePart2(key)) << endl;
        solution += value * rank;
        rank--;
    }
    cout << "solution: " << solution << endl;

    return EXIT_SUCCESS;
}
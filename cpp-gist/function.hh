#include <algorithm>
#include <any>
#include <array>
#include <chrono>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <list>
#include <numeric>
#include <unordered_map>
#include <vector>

struct Log {
    template <class... A>
    static void log(A&&... a) {
        log_impl<sizeof...(A)>({std::any(std::forward<A>(a))...});
    }

private:
    template <std::size_t N>
    static void log_impl(std::array<std::any, N> v) {
        for (auto&& i : v) {
            std::cout << std::any_cast<int>(i) << "\n";
        }
    }
    template <class... A>
    static void variadic_log(A&&... a) {
    }
};

std::vector<std::pair<uint8_t, uint8_t>>
getLowestPoints(std::vector<std::vector<uint8_t>> const& digits) {
    std::vector<std::pair<uint8_t, uint8_t>> lowestPoints;
    for (size_t i = 0; i < digits.size(); ++i) {
        for (size_t j = 0; j < digits[i].size(); ++j) {
            bool isLowPoint(true);
            if (i != 0) {
                isLowPoint &= digits[i][j] < digits[i - 1][j];
            }
            if (i != digits.size() - 1) {
                isLowPoint &= digits[i][j] < digits[i + 1][j];
            }
            if (j != 0) {
                isLowPoint &= digits[i][j] < digits[i][j - 1];
            }
            if (j != digits[i].size() - 1) {
                isLowPoint &= digits[i][j] < digits[i][j + 1];
            }

            if (isLowPoint) {
                lowestPoints.push_back(std::make_pair(i, j));
            }
        }
    }
    return lowestPoints;
}

void expandOnPoints(std::vector<std::vector<uint8_t>> const& digits,
                    std::vector<std::vector<bool>>& marked,
                    uint32_t i,
                    uint32_t j,
                    uint32_t& sumOfBasin) {
    if (digits[i][j] == 9) {
        return;
    }
    sumOfBasin++;
    marked[i][j] = true;
    if (i != 0 && digits[i][j] < digits[i - 1][j] && !marked[i - 1][j]) {
        expandOnPoints(digits, marked, i - 1, j, sumOfBasin);
    }
    if (i != digits.size() - 1 && digits[i][j] < digits[i + 1][j] && !marked[i + 1][j]) {
        expandOnPoints(digits, marked, i + 1, j, sumOfBasin);
    }
    if (j != 0 && digits[i][j] < digits[i][j - 1] && !marked[i][j - 1]) {
        expandOnPoints(digits, marked, i, j - 1, sumOfBasin);
    }
    if (j != digits[i].size() - 1 && digits[i][j] < digits[i][j + 1] && !marked[i][j + 1]) {
        expandOnPoints(digits, marked, i, j + 1, sumOfBasin);
    }
}

uint32_t getBasins(std::vector<std::vector<uint8_t>> const& digits) {
    std::vector<std::vector<bool>> marked(digits.size(),
                                          std::vector<bool>(digits[0].size(), false));
    auto const& lowestPoints = getLowestPoints(digits);
    std::vector<uint32_t> largestBasins;
    for (auto const& [i, j] : lowestPoints) {
        uint32_t sumOfBasin(0);
        expandOnPoints(digits, marked, i, j, sumOfBasin);
        largestBasins.push_back(sumOfBasin);
    }

    std::sort(largestBasins.begin(), largestBasins.end(), std::greater<uint32_t>());
    return largestBasins[0] * largestBasins[1] * largestBasins[2];
}

struct DisplayLine {
    DisplayLine(std::vector<std::string>& numbers, std::vector<std::string>& digits) {
        std::copy_n(numbers.begin(), 10, numbers_.begin());
        std::copy_n(digits.begin(), 4, digits_.begin());
    }

    friend std::ostream& operator<<(std::ostream& os, const DisplayLine& line);
    std::array<std::string, 10> numbers_;
    std::array<std::string, 4> digits_;
};

std::ostream& operator<<(std::ostream& os, const DisplayLine& line) {
    for (auto const& n : line.numbers_) {
        os << n << " ";
    }
    os << "| ";
    for (auto const& d : line.digits_) {
        os << d << " ";
    }
    os << std::endl;
    return os;
}

bool areAnagram(std::string str1, std::string str2) {
    // Get lengths of both strings
    int n1 = str1.length();
    int n2 = str2.length();

    // If length of both strings is not same, then they
    // cannot be anagram
    if (n1 != n2)
        return false;

    // Sort both the strings
    sort(str1.begin(), str1.end());
    sort(str2.begin(), str2.end());

    // Compare sorted strings
    for (int i = 0; i < n1; i++)
        if (str1[i] != str2[i])
            return false;

    return true;
}

std::unordered_map<uint8_t, std::string>
getSegmentConfiguration(std::array<std::string, 10> const& numbers) {
    std::unordered_map<uint8_t, std::string> mappedNumbers = {
        {0, ""}, {1, ""}, {2, ""}, {3, ""}, {4, ""}, {5, ""}, {6, ""}, {7, ""}, {8, ""}, {9, ""}};
    std::string configuration(".......");
    for (auto const& number : numbers) {
        if (number.size() == 2) {
            mappedNumbers[1] = number;
        } else if (number.size() == 3) {
            mappedNumbers[7] = number;
        } else if (number.size() == 4) {
            mappedNumbers[4] = number;
        } else if (number.size() == 7) {
            mappedNumbers[8] = number;
        }
    }

    for (auto const& segment : mappedNumbers[7]) {
        if (mappedNumbers[1].find(segment) == std::string::npos) {
            configuration[0] = segment;
        }
    }
    std::string configuration1346;
    for (auto const& segment : mappedNumbers[8]) {
        if (mappedNumbers[7].find(segment) == std::string::npos) {
            configuration1346 += segment;
        }
    }
    std::string configuration46;
    std::string configuration13;
    for (auto const& segment : configuration1346) {
        auto const& s = mappedNumbers[4].find(segment);
        if (s == std::string::npos) {
            configuration46 += segment;
        } else {
            configuration13 += segment;
        }
    }

    for (auto const& number : numbers) {
        if (number.size() != 6) {
            continue;
        }
        bool found(true);
        for (auto const& segment : configuration1346 + configuration[0]) {
            if (number.find(segment) == std::string::npos) {
                found = false;
            }
            if (!found) {
                break;
            }
        }
        if (found) {
            mappedNumbers[6] = number;
            break;
        }
    }

    for (auto const& segment : mappedNumbers[1]) {
        if (mappedNumbers[6].find(segment) == std::string::npos) {
            configuration[2] = segment;
        }
    }

    for (auto const& segment : mappedNumbers[1]) {
        if (segment != configuration[2]) {
            configuration[5] = segment;
        }
    }

    for (auto const& number : numbers) {
        if (number.size() != 5) {
            continue;
        }
        bool found(true);
        for (auto const& segment : configuration13 + configuration[0] + configuration[5]) {
            if (number.find(segment) == std::string::npos) {
                found = false;
            }
            if (!found) {
                break;
            }
        }
        if (found) {
            mappedNumbers[5] = number;
            break;
        }
    }

    for (auto const& segment : mappedNumbers[6]) {
        if (mappedNumbers[5].find(segment) == std::string::npos) {
            configuration[4] = segment;
        }
    }

    for (auto const& segment : configuration46) {
        if (segment != configuration[4]) {
            configuration[6] = segment;
        }
    }

    // we now have 02546 as segments, we need to split 1,3
    for (auto const& number : numbers) {
        if (number.size() != 5) {
            continue;
        }
        bool found(true);
        for (auto const& segment : configuration46 + configuration[0] + configuration[2]) {
            if (number.find(segment) == std::string::npos) {
                found = false;
            }
            if (!found) {
                break;
            }
        }
        if (found) {
            mappedNumbers[2] = number;
            break;
        }
    }

    for (auto const& segment :
         configuration46 + configuration[0] + configuration[2] + configuration13) {
        if (mappedNumbers[2].find(segment) == std::string::npos) {
            configuration[1] = segment;
        }
    }

    for (auto const& segment : configuration13) {
        if (segment != configuration[1]) {
            configuration[3] = segment;
        }
    }

    // we mapped every number except 3, 9 and 0
    mappedNumbers[0] = std::string() + configuration[0] + configuration[1] + configuration[2] +
                       configuration[4] + configuration[5] + configuration[6];
    mappedNumbers[3] = std::string() + configuration[0] + configuration[2] + configuration[3] +
                       configuration[5] + configuration[6];
    mappedNumbers[9] = std::string() + configuration[0] + configuration[1] + configuration[2] +
                       configuration[3] + configuration[5] + configuration[6];

    return mappedNumbers;
}

uint32_t getSimpleDigits(std::list<DisplayLine> const& lines) {
    uint32_t numberOfSimpleDigits = 0;
    for (auto const& l : lines) {
        for (auto const& d : l.digits_) {
            if (d.size() == 2 || d.size() == 3 || d.size() == 4 || d.size() == 7) {
                numberOfSimpleDigits++;
            }
        }
    }
    return numberOfSimpleDigits;
}

uint32_t getAllDigits(std::list<DisplayLine> const& lines) {
    uint32_t numberOfSimpleDigits = 0;
    for (auto const& l : lines) {
        for (auto const& d : l.digits_) {
            if (d.size() == 2 || d.size() == 3 || d.size() == 4 || d.size() == 7) {
                numberOfSimpleDigits++;
            }
        }
    }
    return numberOfSimpleDigits;
}

uint64_t getPosition(std::vector<uint32_t> const& crabs) {
    auto start = std::chrono::high_resolution_clock::now();
    uint32_t min(UINT32_MAX);
    uint32_t max(0);
    for (auto const& c : crabs) {
        if (max < c) {
            max = c;
        }
        if (min > c) {
            min = c;
        }
    }
    uint64_t minimumSum(UINT64_MAX);
    for (uint32_t i = min; i <= max; i++) {
        uint64_t sum(0);
        for (size_t crabIdx = 0; crabIdx < crabs.size(); crabIdx++) {
            uint32_t c = crabs[crabIdx];
            if (c >= i) {
                sum += (c - i) * ((c - i) + 1) / 2;
            } else {
                sum += (i - c) * ((i - c) + 1) / 2;
            }
        }
        if (minimumSum > sum) {
            minimumSum = sum;
        }
    }
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "position duration: " << duration.count() << std::endl;
    return minimumSum;
}

// wrong
uint64_t getGeneration(std::vector<uint8_t>& generation, uint32_t days) {
    std::vector<uint32_t> bornAt(days, 0);
    for (auto& g : generation) {
        bornAt[g]++;
    }
    for (uint32_t day = 0; day < days; day++) {
        bornAt[day] += bornAt[day < 7 ? days - 8 + day : day - 7] +
                       bornAt[day < 9 ? days - 10 + day : day - 9];
    }
    return static_cast<uint64_t>(generation.size() +
                                 std::accumulate(bornAt.begin(), bornAt.end(), 0));
}

struct LineCoords {
    uint32_t startingY = 0;
    uint32_t startingX = 0;
    uint32_t endingY = 0;
    uint32_t endingX = 0;

    void swap(LineCoords& other) {
        LineCoords l(*this);
        *this = other;
        other = l;
    }
};

enum class lineType { horizontal, vertical, diagonalRight, diagonalLeft, none };

uint32_t getNumberOfOverlaps(std::vector<std::vector<uint32_t>> const& resultMatrix) {
    uint32_t numberOfOverlaps = 0;
    for (auto const& i : resultMatrix) {
        for (auto const& j : i) {
            if (j > 1) {
                numberOfOverlaps++;
            }
        }
    }
    return numberOfOverlaps;
}

std::string lineTypeToString(lineType l) {
    switch (l) {
    case lineType::horizontal:
        return "horizontal";
    case lineType::vertical:
        return "vertical";
    case lineType::diagonalRight:
        return "diagonal right";
    case lineType::diagonalLeft:
        return "diagonal left";
    case lineType::none:
    default:
        return "none";
    }
}

std::vector<std::vector<uint32_t>> getDrawMatrix(std::vector<LineCoords> const& lines,
                                                 uint32_t maxX,
                                                 uint32_t maxY,
                                                 uint32_t minX,
                                                 uint32_t minY) {
    std::vector<std::vector<uint32_t>> resultMatrix(maxY - minY + 1,
                                                    std::vector<uint32_t>(maxX - minX + 1, 0));
    for (auto const& line : lines) {
        uint32_t starting(0);
        uint32_t ending(0);
        uint32_t startingY(0);
        lineType lineType(lineType::none);
        bool canDraw(false);
        if (line.startingX == line.endingX) {
            lineType = lineType::vertical;
            if (line.startingY > line.endingY) {
                canDraw = true;
                starting = line.endingY;
                ending = line.startingY;
            } else {
                canDraw = true;
                starting = line.startingY;
                ending = line.endingY;
            }

        } else if (line.endingY == line.startingY) {
            lineType = lineType::horizontal;
            if (line.startingX > line.endingX) {
                canDraw = true;
                starting = line.endingX;
                ending = line.startingX;
            } else {
                canDraw = true;
                starting = line.startingX;
                ending = line.endingX;
            }
        } else if (std::abs(static_cast<int32_t>(line.startingX - line.endingX)) ==
                   std::abs(static_cast<int32_t>(line.startingY - line.endingY))) {
            canDraw = true;
            if (line.startingY > line.endingY) {
                startingY = line.endingY;
                if (line.startingX > line.endingX) {
                    lineType = lineType::diagonalRight;
                    starting = line.endingX;
                    ending = line.startingX;
                } else {
                    lineType = lineType::diagonalLeft;
                    starting = line.startingX;
                    ending = line.endingX;
                }

            } else if (line.startingY < line.endingY) {
                startingY = line.startingY;
                if (line.startingX > line.endingX) {
                    lineType = lineType::diagonalLeft;
                    starting = line.endingX;
                    ending = line.startingX;
                } else {
                    lineType = lineType::diagonalRight;
                    starting = line.startingX;
                    ending = line.endingX;
                }
            }
        }
        std::cout << starting << "->" << ending << "\t," << startingY << "\t"
                  << lineTypeToString(lineType) << std::endl;
        if (!canDraw) {
            continue;
        }
        uint32_t j(startingY);
        uint32_t k(0);
        for (uint32_t i = starting; i <= ending; i++) {
            switch (lineType) {
            case lineType::horizontal:
                resultMatrix[line.startingY - minY][i - minX]++;
                break;
            case lineType::vertical:
                resultMatrix[i - minY][line.startingX - minX]++;
                break;
            case lineType::diagonalRight:
                resultMatrix[j - minY][i - minX]++;
                j++;
                break;
            case lineType::diagonalLeft:
                resultMatrix[startingY + (ending - starting) - k - minY][i - minX]++;
                k++;
                break;
            default:
                break;
            }
        }
    }
    return resultMatrix;
}

bool checkWinner(std::array<std::array<bool, 5>, 5> const& markedNumbers) {
    for (size_t i = 0; i < markedNumbers.size(); i++) {
        bool completedLine = true;
        for (size_t j = 0; j < markedNumbers[i].size(); j++) {
            if (!markedNumbers[i][j]) {
                completedLine = false;
                break;
            }
        }
        if (completedLine) {
            return true;
        }
    }

    for (size_t i = 0; i < markedNumbers.size(); i++) {
        bool completedColumn = true;
        for (size_t j = 0; j < markedNumbers[i].size(); j++) {
            if (!markedNumbers[j][i]) {
                completedColumn = false;
                break;
            }
        }
        if (completedColumn) {
            return true;
        }
    }

    return false;
}

uint64_t getSum(std::array<std::array<uint32_t, 5>, 5> const& bingoTable,
                std::array<std::array<bool, 5>, 5> const& markedNumbers) {
    uint64_t sumOfUnmakerdNumbers(0);
    for (size_t i = 0; i < bingoTable.size(); i++) {
        for (size_t j = 0; j < bingoTable[i].size(); j++) {
            if (!markedNumbers[i][j]) {
                sumOfUnmakerdNumbers += bingoTable[i][j];
            }
        }
    }
    return sumOfUnmakerdNumbers;
}

bool lastWinner(std::vector<bool> const& wonTable) {
    uint32_t numbersWon(0);
    for (auto const& won : wonTable) {
        if (won) {
            numbersWon++;
        }
    }
    return numbersWon == wonTable.size();
}

uint64_t determineWinner(std::vector<std::array<std::array<uint32_t, 5>, 5>> const& bingoTables,
                         std::vector<uint32_t> const& winningNumbers) {
    std::vector<bool> wonTables(bingoTables.size());
    std::vector<std::array<std::array<bool, 5>, 5>> markedNumbers(winningNumbers.size());
    for (auto const& number : winningNumbers) {
        for (size_t tableIdx = 0; tableIdx < bingoTables.size(); tableIdx++) {
            for (size_t i = 0; i < bingoTables[tableIdx].size(); i++) {
                for (size_t j = 0; j < bingoTables[tableIdx][i].size(); j++) {
                    if (number == bingoTables[tableIdx][i][j]) {
                        markedNumbers[tableIdx][i][j] = true;
                    }
                }
            }
            if (checkWinner(markedNumbers[tableIdx])) {
                wonTables[tableIdx] = true;
                if (lastWinner(wonTables)) {
                    uint64_t sumOfUnmakerdNumbers(
                        getSum(bingoTables[tableIdx], markedNumbers[tableIdx]));
                    std::cout << sumOfUnmakerdNumbers << "\t" << number << std::endl;
                    for (auto const& i : markedNumbers[tableIdx]) {
                        for (auto const& j : i) {
                            std::cout << j << " ";
                        }
                        std::cout << std::endl;
                    }
                    std::cout << std::endl;
                    return number * sumOfUnmakerdNumbers;
                }
            }
        }
    }
    return 0;
}

std::vector<std::string> split(std::string s, std::string delimiter) {
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    std::string token;
    std::vector<std::string> res;

    while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
        token = s.substr(pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back(token);
    }

    res.push_back(s.substr(pos_start));
    return res;
}

// boost::dynamic_bitset<> filterOxygen(std::vector<boost::dynamic_bitset<>>& report,
//                                      uint32_t startingBitIdx) {
//     if (report.size() == 1) {
//         return report[0];
//     }
//     int32_t averageValue(0);
//     std::vector<boost::dynamic_bitset<>> positiveReport;
//     std::vector<boost::dynamic_bitset<>> negativeReport;
//     for (auto& number : report) {
//         if (number[number.size() - 1 - startingBitIdx]) {
//             averageValue++;
//             positiveReport.push_back(number);
//         } else {
//             averageValue--;
//             negativeReport.push_back(number);
//         }
//     }
//     if (averageValue >= 0) {
//         return filterOxygen(positiveReport, startingBitIdx + 1);
//     } else {
//         return filterOxygen(negativeReport, startingBitIdx + 1);
//     }
// }

// boost::dynamic_bitset<> filterCO(std::vector<boost::dynamic_bitset<>>& report,
//                                  uint32_t startingBitIdx) {
//     if (report.size() == 1) {
//         return report[0];
//     }
//     int32_t averageValue(0);
//     std::vector<boost::dynamic_bitset<>> positiveReport;
//     std::vector<boost::dynamic_bitset<>> negativeReport;
//     for (auto& number : report) {
//         if (number[number.size() - 1 - startingBitIdx]) {
//             averageValue++;
//             positiveReport.push_back(number);
//         } else {
//             averageValue--;
//             negativeReport.push_back(number);
//         }
//     }
//     if (averageValue >= 0) {
//         return filterCO(negativeReport, startingBitIdx + 1);
//     } else {
//         return filterCO(positiveReport, startingBitIdx + 1);
//     }
// }

// boost::dynamic_bitset<> getGammaRate(std::vector<boost::dynamic_bitset<>> const& report) {
//     size_t size = static_cast<int>(report[0].size());
//     std::vector<int32_t> rate(size, 0);
//     for (size_t idx = 0; idx < report.size(); idx++) {
//         for (size_t bitIdx = 0; bitIdx < report[idx].size(); bitIdx++) {
//             if (report[idx][bitIdx]) {
//                 rate[bitIdx]++;
//             } else {
//                 rate[bitIdx]--;
//             }
//         }
//     }

//     for (auto c : rate) {
//         std::cout << c << " ";
//     }
//     std::cout << std::endl;
//     boost::dynamic_bitset<> gammaRate(rate.size());
//     for (size_t bitIdx = 0; bitIdx < rate.size(); bitIdx++) {
//         if (rate[bitIdx] < 0) {
//             gammaRate[bitIdx] = false;
//         } else {
//             gammaRate[bitIdx] = true;
//         }
//     }
//     return gammaRate;
// }

uint32_t getIncreasedMeasurements(std::vector<uint32_t> const& depths) {
    uint32_t measurements(0);
    uint64_t sum(0);
    for (size_t i = 0; i < depths.size(); i++) {
        if (i > 2) {
            uint64_t oldSum(sum);
            sum += depths[i];
            sum -= depths[i - 3];
            if (sum > oldSum) {
                measurements++;
            }
        } else {
            sum += depths[i];
        }
    }

    return measurements;
}
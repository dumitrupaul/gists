#include <algorithm>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <cmath>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <string_view>
#include <unordered_set>
#include <vector>

bool isInside(std::tuple<std::uint32_t, std::uint32_t> a,
              std::tuple<std::uint32_t, std::uint32_t> b) {
    return (std::get<0>(a) >= std::get<0>(b) && std::get<1>(a) <= std::get<1>(b)) ||
           (std::get<0>(b) >= std::get<0>(a) && std::get<1>(b) <= std::get<1>(a));
}
bool isInsidePart2(std::tuple<std::uint32_t, std::uint32_t> a,
                   std::tuple<std::uint32_t, std::uint32_t> b) {
    return (std::get<1>(a) >= std::get<0>(b) && std::get<1>(b) >= std::get<1>(a)) ||
           (std::get<1>(b) >= std::get<0>(a) && std::get<0>(b) <= std::get<1>(a));
}

int main() {
    std::fstream inputFile("../input/day4.in", std::ios_base::in);
    if (!inputFile.is_open()) {
        std::cerr << "Couldn't open input file" << std::endl;
        return EXIT_FAILURE;
    }

    std::string inputString;
    uint32_t count(0);
    while (inputFile >> inputString) {
        std::vector<std::string> strs;

        boost::split(strs, inputString, boost::is_any_of(","));
        assert(strs.size() == 2);

        std::vector<std::string> tempIntervalString;
        boost::split(tempIntervalString, strs[0], boost::is_any_of("-"));
        assert(tempIntervalString.size() == 2);
        std::tuple<std::uint32_t, std::uint32_t> firstInterval{std::stoul(tempIntervalString[0]),
                                                               std::stoul(tempIntervalString[1])};

        boost::split(tempIntervalString, strs[1], boost::is_any_of("-"));
        assert(tempIntervalString.size() == 2);
        std::tuple<std::uint32_t, std::uint32_t> secondInterval{std::stoul(tempIntervalString[0]),
                                                                std::stoul(tempIntervalString[1])};
        if (isInsidePart2(firstInterval, secondInterval)) {
            count++;
        }
    }

    std::cout << count << std::endl;
    return EXIT_SUCCESS;
}
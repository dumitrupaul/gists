#include <algorithm>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/compare.hpp>
#include <boost/algorithm/string/split.hpp>
#include <cmath>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <queue>
#include <regex>
#include <set>
#include <sstream>
#include <stack>
#include <string>
#include <string_view>
#include <unordered_set>
#include <variant>
#include <vector>

using namespace std;

void parseAndMove(vector<int32_t>& values) {
    int32_t i(0);
    for (auto it = values.begin(); it != values.end(); it++) {
        if (*it == 2) {
            values.insert(it + *it, *it);
            it = values.begin() + i;
        }
        i++;
    }
}

int main() {
    std::fstream inputFile("../input/day20.in", std::ios_base::in);
    if (!inputFile.is_open()) {
        std::cerr << "Couldn't open input file" << std::endl;
        return EXIT_FAILURE;
    }

    vector<int32_t> values;
    int32_t tempValue(0);
    while (inputFile >> tempValue) {
        values.push_back(tempValue);
    }

    parseAndMove(values);
    for (auto const& v : values) {
        cout << v << ",";
    }

    return EXIT_SUCCESS;
}
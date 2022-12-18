#include <algorithm>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <cmath>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <queue>
#include <regex>
#include <sstream>
#include <stack>
#include <string>
#include <string_view>
#include <unordered_set>
#include <variant>
#include <vector>

struct Packet {
    std::variant<uint32_t, std::vector<Packet>> _;

    Packet() = default;

    Packet(uint32_t a) {
        _ = a;
    }

    Packet(std::vector<Packet> const& a) {
        _ = a;
    }

    bool isVector() const {
        return _.index() == 1;
    }

    std::stringstream printPacket() const {
        std::stringstream s;
        if (!isVector()) {
            s << asUint() << ",";
        } else {
            s << "[";
            for (auto const& p : asVector()) {
                s << p.printPacket().str();
            }
            s << "],";
        }
        return s;
    }

    void swap(Packet& other) {
        Packet temp;
        temp._ = other._;
        other._ = _;
        _ = temp._;
    }

    std::vector<Packet> const& asVector() const {
        return std::get<std::vector<Packet>>(_);
    }
    std::vector<Packet>& asVector() {
        return std::get<std::vector<Packet>>(_);
    }

    uint32_t asUint() const {
        return std::get<uint32_t>(_);
    }
};

bool operator==(std::vector<Packet> const& left, std::vector<Packet> const& right) {
    if (left.size() != right.size()) {
        return false;
    }

    for (size_t i = 0; i < left.size(); i++) {
        if (!left[i].isVector() && !right[i].isVector()) {
            if (left[i].asUint() != right[i].asUint()) {
                return false;
            } else {
                continue;
            }
        }
        if (!left[i].isVector()) {
            std::vector<Packet> temp{Packet(left[i].asUint())};
            return temp == right[i].asVector();
        }
        if (!right[i].isVector()) {
            std::vector<Packet> temp{Packet(right[i].asUint())};
            return left[i].asVector() == temp;
        }

        return left[i].asVector() == right[i].asVector();
    }
    return true;
}

bool operator<=(std::vector<Packet> const& left, std::vector<Packet> const& right) {
    for (size_t i = 0; i < left.size(); i++) {
        if (i >= right.size()) {
            return false;
        }

        if (!left[i].isVector() && !right[i].isVector()) {
            if (left[i].asUint() == right[i].asUint()) {
                continue;
            }
            return left[i].asUint() < right[i].asUint();
        }
        if (!left[i].isVector()) {
            std::vector<Packet> temp{Packet(left[i].asUint())};
            return temp <= right[i].asVector();
        }
        if (!right[i].isVector()) {
            std::vector<Packet> temp{Packet(right[i].asUint())};
            return left[i].asVector() <= temp;
        }

        if (left[i].asVector() == right[i].asVector()) {
            continue;
        } else {
            return left[i].asVector() <= right[i].asVector();
        }
    }
    return true;
}

size_t parseAndInsert(Packet& result, std::string const& inputString) {
    for (size_t i = 0; i < inputString.size(); i++) {
        if (inputString[i] == '[') {
            Packet temp(std::vector<Packet>{});
            size_t offset = parseAndInsert(temp, inputString.substr(i + 1));
            result.asVector().push_back(temp);
            i += offset;
        } else if (inputString[i] == ']') {
            return i + 1;
        } else if (inputString[i] == ',') {
        } else {
            size_t endColon = inputString.find_first_of(',', i);
            size_t endVector = inputString.find_first_of(']', i);
            size_t end = endColon < endVector ? endColon : endVector;
            if (end != std::string::npos && end - i > 0) {
                uint32_t numberToInsert(
                    static_cast<uint32_t>(std::stoul(inputString.substr(i, end - i))));

                Packet p(numberToInsert);
                result.asVector().emplace_back(std::move(p));
                i = end - 1;
            }
        }
    }
    return inputString.size();
}

std::string beautify(std::string const& input) {
    std::string result;
    for (size_t i = 0; i < input.size() - 1; i++) {
        if (input[i] == ',' && input[i + 1] == ']') {
            continue;
        } else {
            result.push_back(input[i]);
        }
    }
    return result;
}

int main() {
    std::fstream inputFile("../input/day13.in", std::ios_base::in);
    if (!inputFile.is_open()) {
        std::cerr << "Couldn't open input file" << std::endl;
        return EXIT_FAILURE;
    }

    std::string inputStringRight, inputStringLeft;
    std::vector<Packet> packetList;
    Packet divider2(std::vector<Packet>{std::vector<Packet>{Packet(2)}});
    Packet divider6(std::vector<Packet>{std::vector<Packet>{Packet(6)}});

    while (inputFile >> inputStringLeft >> inputStringRight) {
        Packet temp(std::vector<Packet>{});
        Packet temp2(std::vector<Packet>{});
        parseAndInsert(temp, inputStringLeft);
        parseAndInsert(temp2, inputStringRight);
        packetList.push_back(temp.asVector()[0]);
        packetList.push_back(temp2.asVector()[0]);
    }

    for (auto& p1 : packetList) {
        for (auto& p2 : packetList) {
            if (p1.asVector() <= p2.asVector()) {
                p1.swap(p2);
            }
        }
    }

    size_t divider2Location(0), divider6Location(0);
    for (size_t i = 0; i <= packetList.size(); i++) {
        if (beautify(packetList[i].printPacket().str()) == beautify(divider2.printPacket().str())) {
            divider2Location = i;
        } else if (beautify(packetList[i].printPacket().str()) ==
                   beautify(divider6.printPacket().str())) {
            divider6Location = i;
        }
    }

    std::cout << (divider2Location + 1) * (divider6Location + 1) << std::endl;

    return EXIT_SUCCESS;
}
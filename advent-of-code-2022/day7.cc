#include <algorithm>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <cmath>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <regex>
#include <stack>
#include <string>
#include <string_view>
#include <unordered_set>
#include <vector>

struct File {
    enum class FileType { File, Directory };

    explicit File(FileType type = FileType::File,
                  std::string const& name = "default",
                  uint64_t size = 0)
        : children_() {
        type_ = type;
        name_ = name;
        totalSize_ = size;
    }

    FileType type_;
    std::string name_ = "default";
    uint64_t totalSize_;
    std::map<std::string, std::shared_ptr<File>> children_;

    void prettyPrint(std::string const& indent = "") const {
        for (auto const& [name, child] : children_) {
            std::cout << indent + "\t"
                      << "name: " << name << ", size: " << child->totalSize_ << std::endl;
            child->prettyPrint(indent + "\t");
            std::cout << std::endl;
        }
    }

    void populateDirectorySize() {
        for (auto const& [name, child] : children_) {
            if (child->type_ == FileType::Directory) {
                child->populateDirectorySize();
            }

            totalSize_ += child->totalSize_;
        }
    }

    std::vector<std::shared_ptr<File>> childrenDfs() {
        std::vector<std::shared_ptr<File>> result;
        for (auto const& [name, child] : children_) {
            result.push_back(child);
            if (child->type_ == FileType::Directory) {
                auto const& c = child->childrenDfs();
                result.insert(std::end(result), c.begin(), c.end());
            }
        }
        return result;
    }

    void insertAtPath(std::string const& path,
                      std::string const& name,
                      std::shared_ptr<File> toBeInserted) {
        toBeInserted->name_ = name;
        if (path.empty() || path == "/") {
            children_.insert({name, toBeInserted});
        } else {
            std::vector<std::string> paths;
            boost::split(paths, path, boost::is_any_of("/"));
            paths.erase(paths.begin());

            std::shared_ptr<File> temp;
            if (!paths.empty() && children_.find(paths[0]) != children_.end()) {
                temp = children_[paths[0]];
            }

            for (size_t i = 1; i < paths.size(); ++i) {
                if (temp->children_.find(paths[i]) != temp->children_.end()) {
                    temp = temp->children_[paths[i]];
                }
            }
            temp->children_.insert({name, toBeInserted});
        }
    }
};

static bool sortBySize(const std::shared_ptr<File>& d1, const std::shared_ptr<File>& d2) {
    return d1->totalSize_ < d2->totalSize_;
}

int main() {
    std::fstream inputFile("../input/day7.in", std::ios_base::in);
    if (!inputFile.is_open()) {
        std::cerr << "Couldn't open input file" << std::endl;
        return EXIT_FAILURE;
    }

    File aocRoot(File::FileType::Directory);
    std::string inputString;
    std::string currentPath;
    while (std::getline(inputFile, inputString)) {
        std::smatch sm;
        if (std::regex_search(inputString, sm, std::regex(". cd (.*)"))) {
            if (sm[1] == "/")
                continue;
            if (sm[1] == "..") {
                currentPath = currentPath.substr(0, currentPath.find_last_of("/"));
            } else {
                currentPath += "/" + std::string(sm[1]);
            }
        } else if (std::regex_search(inputString, sm, std::regex("dir (.*)"))) {
            aocRoot.insertAtPath(currentPath, sm[1],
                                 std::make_shared<File>(File::FileType::Directory));
        } else if (inputString == "$ ls") {
            // nothing
        } else {
            std::stringstream ss;
            ss << inputString;
            uint64_t size = 0;
            std::string fileName;
            ss >> size >> fileName;
            aocRoot.insertAtPath(currentPath, fileName,
                                 std::make_shared<File>(File::FileType::File, fileName, size));
        }
    }

    aocRoot.populateDirectorySize();

    // part 1
    uint64_t sum = 0;
    for (auto const& c : aocRoot.childrenDfs()) {
        if (c->type_ == File::FileType::Directory) {
            if (c->totalSize_ < 100000) {
                sum += c->totalSize_;
            }
        }
    }

    // part 2
    uint64_t needToFreeSize(aocRoot.totalSize_ - 40000000);
    std::cout << "total root size: " << aocRoot.totalSize_;
    std::vector<std::shared_ptr<File>> sortedDirectories;
    for (auto const& c : aocRoot.childrenDfs()) {
        if (c->type_ == File::FileType::Directory) {
            sortedDirectories.push_back(c);
        }
    }
    std::sort(sortedDirectories.begin(), sortedDirectories.end(), sortBySize);
    for (auto const& c : sortedDirectories) {
        std::cout << "name " << c->name_ << " size: " << c->totalSize_ << std::endl;
        if (c->totalSize_ > needToFreeSize) {
            break;
        }
    }

    return EXIT_SUCCESS;
}
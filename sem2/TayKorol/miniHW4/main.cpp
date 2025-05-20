#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

int main() {
    std::vector<std::string> strings = {"Hello", "World", "C++", "Lambda", "Expression"};


    auto getLengths = [](const std::vector<std::string>& vec) {
        std::vector<size_t> lengths;
        for (const auto& str : vec) {
            lengths.push_back(str.size());
        }
        return lengths;
    };

    std::vector<size_t> lengths = getLengths(strings);

    for (size_t i = 0; i < strings.size(); ++i) {
        std::cout << "String \"" << strings[i] << "\" has length of " << lengths[i] << std::endl;
    }

    return 0;
}
#include "test.hpp"
// Parsing mode
std::vector<std::string> parseModeString(std::string mode) {
    std::vector<std::string> vectorMode;
    if (mode.empty() || (mode[0] != '+' && mode[0] != '-'))
        return vectorMode;
    size_t i = 0;
    while (i < mode.size()) {
        if (mode[i] == '+' || mode[i] == '-') {
            size_t begin = i;
            size_t end = begin + 1;
            while (end < mode.size() && mode[end] != '+' && mode[end] != '-')
                end++;
            std::string subMode = mode.substr(begin, end - begin);
            vectorMode.push_back(subMode);
            i = end;
        }
    }
    return vectorMode;
}

int testParseModeString() {

    // Test with empty string
    std::string empty = "";
    std::vector<std::string> result = parseModeString(empty);
    if (result.size() != 0) {
        std::cout << "Test failed with empty string" << std::endl;
        return 1;
    }

    // Test with no mode indicators
    std::string noMode = "test";
    result = parseModeString(noMode);
    if (result.size() != 0) {
        std::cout << "Test failed with no mode indicators" << std::endl;
        return 1;
    }

    // Test with single mode
    std::string singleMode = "+o";
    result = parseModeString(singleMode);
    if (result.size() != 1 || result[0] != "+o") {
        std::cout << "Test failed with single mode" << std::endl;
        return 1;
    }

    // Test with multiple modes
    std::string multiMode = "+oi-v+kt-l";
    result = parseModeString(multiMode);
    if (result.size() != 4 ||
        result[0] != "+oi" || result[1] != "-v" ||
        result[2] != "+kt" || result[3] != "-l") {
        std::cout << "Test failed with multiple modes" << std::endl;
        return 1;
    }

    std::cout << "All tests passed!" << std::endl;
    return 0;
}

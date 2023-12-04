#include "test.hpp"

std::vector<std::string> splitString(std::string str, char c) {
    std::vector<std::string> strs;
    for (size_t i = 0; i < str.size(); i++) {
        if (str[i] != c) {
            size_t begin = i;
            size_t end = begin + 1;
            while (end < str.size() && str[end] != c)
                end++;
            strs.push_back(str.substr(begin, end - begin));
            i = end;
        }
    }
    return strs;
}

std::vector<std::string> cmdFindArgs(std::string input) {
    size_t colonIndex = input.find(':');
    std::vector<std::string> args = splitString(input.substr(0, colonIndex), ' ');
    return args;
}

int testSplitString() {
    std::string testString = "hello world";
    char splitChar = ' ';
    std::vector<std::string> result = splitString(testString, splitChar);
    if (result.size() == 2 &&
        result[0] == "hello" &&
        result[1] == "world") {
        std::cout << "Test passed" << std::endl;
    } else {
        std::cout << "Test failed" << std::endl;
    }

    testString = " hello world  new     end  before";
    splitChar = ' ';
    result = splitString(testString, splitChar);
    if (result.size() == 5 &&
        result[0] == "hello" &&
        result[1] == "world" &&
        result[2] == "new" &&
        result[3] == "end" &&
        result[4] == "before") {
        std::cout << "Test passed" << std::endl;
    } else {
        std::cout << "Test failed" << std::endl;
    }
    return 0;
}

int testCmdFindArgs() {
    std::string input = "NICK :newnick";
    std::vector<std::string> expectedArgs;
    expectedArgs.push_back("NICK");
    std::vector<std::string> actualArgs = cmdFindArgs(input);
    if (actualArgs == expectedArgs) {
        std::cout << "Test Passed" << std::endl;
    } else {
        std::cout << "Test Failed. Expected: ";
        std::cout << expectedArgs << std::endl;
        std::cout << "Actual: ";
        std::cout << actualArgs << std::endl;
    }

    input = "NICK  MODE     NAMES :newnick";
    expectedArgs.clear();
    expectedArgs.push_back("NICK");
    expectedArgs.push_back("MODE");
    expectedArgs.push_back("NAMES");
    actualArgs = cmdFindArgs(input);
    if (actualArgs == expectedArgs) {
        std::cout << "Test Passed" << std::endl;
    } else {
        std::cout << "Test Failed. Expected: ";
        std::cout << expectedArgs << std::endl;
        std::cout << "Actual: ";
        std::cout << actualArgs << std::endl;
    }

    return 0;
}

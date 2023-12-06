#include "test.hpp"

// Find message by :
std::string cmdFindMessage(std::string input) {
    std::string message = "";
    size_t colonIndex = input.find(':');
    if (colonIndex != std::string::npos) {
        if (colonIndex < input.size() - 1)
            message = input.substr(colonIndex + 1);
    }
    return message;
}

int testCmdFindMessage() {
    // Test finds message after colon
    std::string input = "command:message";
    std::string expected = "message";
    std::string actual = cmdFindMessage(input);
    if (expected != actual) {
        std::cout << "Test failed. Expected: " << expected << " Actual: " << actual << std::endl;
        return 1;
    }

    // Test returns empty if no colon
    input = "command";
    expected = "";
    actual = cmdFindMessage(input);
    if (expected != actual) {
        std::cout << "Test failed. Expected: " << expected << " Actual: " << actual << std::endl;
        return 1;
    }

    // Add more tests here
    input = "command:message:test";
    expected = "message:test";
    actual = cmdFindMessage(input);
    if (expected != actual) {
        std::cout << "Test failed. Expected: " << expected << " Actual: " << actual << std::endl;
        return 1;
    }

    input = "command:";
    expected = "";
    actual = cmdFindMessage(input);
    if (expected != actual) {
        std::cout << "Test failed. Expected: " << expected << " Actual: " << actual << std::endl;
        return 1;
    }

    std::cout << "All tests passed!" << std::endl;

    return 0;
}

#include "test.hpp"

std::string extractMessage(std::string response) {
    std::string key = "\"content\": \"";
    size_t start = response.find(key);
    if (start == std::string::npos) {
        return "";
    }
    start += key.length();
    size_t end = response.find("\"\n", start);
    if (end == std::string::npos) {
        return "";
    }
    std::string ret = response.substr(start, end - start);
    std::string remove = "";
    for (size_t i = 0; i < ret.length(); i++) {
        if (ret[i] == '\n') {
            continue;
        }
        remove += ret[i];
    }
    return remove;
}

void testExtractMessage() {
    // Test case 1: Valid response with message
    std::string response1 = "{\"content\": \"Hello,\n\n world!\"\n}";
    std::string expected1 = "Hello, world!";
    std::string result1 = extractMessage(response1);
    assert(result1 == expected1);
    std::cout << "Test case 1 passed." << std::endl;

    // Test case 2: Valid response with empty message
    std::string response2 = "{\"content\": \"\"\n}";
    std::string expected2 = "";
    std::string result2 = extractMessage(response2);
    assert(result2 == expected2);
    std::cout << "Test case 2 passed." << std::endl;

    // Test case 3: Invalid response without key
    std::string response3 = "{\"other_key\": \"Hello, world!\"\n}";
    std::string expected3 = "";
    std::string result3 = extractMessage(response3);
    assert(result3 == expected3);
    std::cout << "Test case 3 passed." << std::endl;

    // Test case 4: Invalid response without closing quote
    std::string response4 = "{\"content\": \"Hello, world!\n}";
    std::string expected4 = "";
    std::string result4 = extractMessage(response4);
    assert(result4 == expected4);
    std::cout << "Test case 4 passed." << std::endl;
}

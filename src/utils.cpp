#include "utils.hpp"

bool strContainOnlyDigits(std::string str) {
    for (size_t i = 0; i < str.length(); i++) {
        if (!std::isdigit(str[i])) {
            return false;
        }
    }
    return true;
}

std::string getCurrentTime(void) {
    std::time_t currentTime = std::time(NULL);
    int bufferSize = 80;
    char buffer[bufferSize];
    std::strftime(buffer, bufferSize, "%Y-%m-%d %H:%M:%S", std::localtime(&currentTime));
    return std::string(buffer);
}

std::string getClientIpAddress(const struct sockaddr_in6 *clientAddress) {
    char ipAddress[INET6_ADDRSTRLEN];
    inet_ntop(AF_INET6, &(clientAddress->sin6_addr), ipAddress, INET6_ADDRSTRLEN);
    return std::string(ipAddress);
}

std::vector<std::string> splitString(std::string str, char c) {
    std::vector<std::string> strs;
    std::string part;
    std::istringstream partStream(str);

    while (std::getline(partStream, part, c)) {
        strs.push_back(part);
    }
    return strs;
}

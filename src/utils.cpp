#include "../inc/irc.hpp"

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

std::string getIpAddressFromSockaddr(const struct sockaddr_in6 *sockaddr) {
    char ipAddress[INET6_ADDRSTRLEN];
    inet_ntop(AF_INET6, &(sockaddr->sin6_addr), ipAddress, INET6_ADDRSTRLEN);
    std::string ip = std::string(ipAddress);
    if (ip.find("::ffff:") != std::string::npos) {
        ip = ip.substr(7);
    }
    if (ip.find("::") != std::string::npos) {
        ip = ip.substr(2);
    }
    if (ip.empty() || ip == "1")
        ip = "127.0.0.1";
    return ip;
}

std::vector<std::string> splitString(std::string str, char c) {
    std::vector<std::string> strs;
    std::string part = "";
    for (size_t i = 0; i < str.size(); i++) {
        if (str[i] == c && part != "") {
            strs.push_back(part);
            part = "";
        } else {
            part += str[i];
        }
    }
    if (part != "") {
        strs.push_back(part);
    }
    return strs;
}

// Find client port from client socket
int getPortFromSocket(int fd) {
    struct sockaddr_in6 clientAddress;
    socklen_t clientAddressSize = sizeof(clientAddress);
    getpeername(fd, (struct sockaddr *)&clientAddress, &clientAddressSize);
    return ntohs(clientAddress.sin6_port);
}

// Remove end of line of a string
void trimEndOfLine(std::string &str) {
    if (!str.empty() && str[str.size() - 1] == '\n') {
        str.erase(str.size() - 1);
    }
}

// Get unix timestamp
int getUnixTimestamp(void) {
    return std::time(NULL);
}

std::string cmdFindMessage(std::string input) {
    std::string message = "";
    size_t colonIndex = input.find(':');
    if (colonIndex != std::string::npos) {
        if (colonIndex < input.size() - 1)
            message = input.substr(colonIndex + 1);
    }
    return message;
}

std::vector<std::string> cmdFindArgs(std::string input) {
    size_t colonIndex = input.find(':');
    std::vector<std::string> args = splitString(input.substr(0, colonIndex), ' ');
    return args;
}

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

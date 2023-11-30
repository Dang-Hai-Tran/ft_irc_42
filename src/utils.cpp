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

std::string getClientIpAddress(const struct sockaddr_in6 *clientAddress) {
    char ipAddress[INET6_ADDRSTRLEN];
    inet_ntop(AF_INET6, &(clientAddress->sin6_addr), ipAddress, INET6_ADDRSTRLEN);
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

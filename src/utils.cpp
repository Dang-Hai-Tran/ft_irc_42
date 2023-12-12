#include "../inc/irc.hpp"

/**
 * @brief Check if a string contains only digits.
 * 
 * @param str The string to check.
 * @return bool True if the string contains only digits, false otherwise.
 */
bool strContainOnlyDigits(std::string str) {
    for (size_t i = 0; i < str.length(); i++) {
        if (!std::isdigit(str[i])) {
            return false;
        }
    }
    return true;
}

/**
 * @brief Get the current time as a string in the format "YYYY-MM-DD HH:MM:SS".
 * 
 * @return std::string The current time as a string.
 */
std::string getCurrentTime(void) {
    std::time_t currentTime = std::time(NULL);
    int bufferSize = 80;
    char buffer[bufferSize];
    std::strftime(buffer, bufferSize, "%Y-%m-%d %H:%M:%S", std::localtime(&currentTime));
    return std::string(buffer);
}

/**
 * @brief Get the IP address from a sockaddr_in6 structure.
 * 
 * @param sockaddr The sockaddr_in6 structure.
 * @return std::string The IP address as a string.
 */
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

/**
 * @brief Split a string into a vector of substrings based on a delimiter character.
 * 
 * @param str The string to split.
 * @param c The delimiter character.
 * @return std::vector<std::string> A vector of substrings.
 */
std::vector<std::string> splitString(std::string str, char c) {
    std::vector<std::string> strs;
    for (size_t i = 0; i < str.size(); i++) {
        if (str[i] == c && str[i+1] == c && i < str.size() - 1) {
            strs.push_back("");
        }
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

/**
 * @brief Get the port number from a socket file descriptor.
 * 
 * @param fd The socket file descriptor.
 * @return int The port number.
 */
int getPortFromSocket(int fd) {
    struct sockaddr_in6 clientAddress;
    socklen_t clientAddressSize = sizeof(clientAddress);
    getpeername(fd, (struct sockaddr *)&clientAddress, &clientAddressSize);
    return ntohs(clientAddress.sin6_port);
}

/**
 * @brief Remove the end of line character from a string.
 * 
 * @param str The string to trim.
 */
void trimEndOfLine(std::string &str) {
    if (!str.empty() && str[str.size() - 1] == '\n') {
        str.erase(str.size() - 1);
    }
}

/**
 * @brief Get the current Unix timestamp.
 * 
 * @return int The Unix timestamp.
 */
int getUnixTimestamp(void) {
    return std::time(NULL);
}

/**
 * @brief Find the message part of a command string.
 * 
 * @param input The command string.
 * @return std::string The message part of the command.
 */
std::string cmdFindMessage(std::string input) {
    std::string message = "";
    size_t colonIndex = input.find(':');
    if (colonIndex != std::string::npos) {
        if (colonIndex < input.size() - 1)
            message = input.substr(colonIndex + 1);
    }
    return message;
}

/**
 * @brief Find the arguments part of a command string.
 * 
 * @param input The command string.
 * @return std::vector<std::string> A vector of arguments.
 */
std::vector<std::string> cmdFindArgs(std::string input) {
    size_t colonIndex = input.find(':');
    std::vector<std::string> args = splitString(input.substr(0, colonIndex), ' ');
    return args;
}

/**
 * @brief Parse a mode string and return a vector of submodes.
 * 
 * @param mode The mode string to parse.
 * @return std::vector<std::string> A vector containing the submodes.
 */
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

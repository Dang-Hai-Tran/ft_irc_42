#ifndef UTILS_HPP
#define UTILS_HPP

#include <arpa/inet.h>
#include <ctime>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

bool strContainOnlyDigits(std::string str);
std::string getCurrentTime(void);
std::string getClientIpAddress(const struct sockaddr_in6 *clientAddress);
std::vector<std::string> splitString(std::string str, char c);
std::string getIpAddressFromSocket(int fd);
int getPortFromSocket(int fd);
void trimEndOfLine(std::string &str);

// Print vector of elements type T
template <typename T>
void printVector(std::vector<T> vec) {
    for (size_t i = 0; i < vec.size(); i++) {
        std::cout << "[" << vec[i] << "]";
    }
    std::cout << std::endl;
}

#endif

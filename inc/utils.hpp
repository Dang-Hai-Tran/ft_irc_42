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

#endif

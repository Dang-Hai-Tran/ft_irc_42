#include "Client.hpp"

Client::Client(Server *server, int fd, std::string ip, int port) : fd(fd), ip(ip), port(port), server(server) {
    this->correctPassword = false;
}

Client::~Client(){};

int Client::getFD(void) {
    return this->fd;
}

std::string Client::getIP(void) {
    return this->ip;
}

int Client::getPort(void) {
    return this->port;
}

std::string Client::getNickName(void) {
    return this->nickName;
}

std::string Client::getUserName(void) {
    return this->userName;
}

std::string Client::getRealName(void) {
    return this->realName;
}

std::string Client::getPartialRecv(void) {
    return this->partialRecv;
}

void Client::setFD(int fd) {
    this->fd = fd;
}

void Client::setIP(std::string ip) {
    this->ip = ip;
}

void Client::setPort(int port) {
    this->port = port;
}

void Client::setNickName(std::string nickName) {
    this->nickName = nickName;
}

void Client::setUserName(std::string userName) {
    this->userName = userName;
}

void Client::setRealName(std::string realName) {
    this->realName = realName;
}

void Client::setPartialRecv(std::string partialRecv) {
    this->partialRecv = partialRecv;
}

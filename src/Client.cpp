#include "../inc/irc.hpp"

Client::Client(Server *server, int fd) : server(server), fd(fd) {}

Client::~Client(){};

int Client::getFD(void) {
    return this->fd;
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

void Client::setFD(int fd) {
    this->fd = fd;
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

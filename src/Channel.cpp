#include "irc.hpp"

Channel::Channel() {
    this->nameChannel = "";
    this->havePassword = false;
    this->server = NULL;
    this->inviteOnly = false;
    this->haveMaxUsers = false;
    this->haveMaxUsers = false;
    this->maxUsers = std::numeric_limits<int>::max();
};

Channel::Channel(Server *server, std::string nameChannel) : server(server), nameChannel(nameChannel) {
    this->havePassword = false;
    this->inviteOnly = false;
    this->haveMaxUsers = false;
    this->haveMaxUsers = false;
    this->maxUsers = std::numeric_limits<int>::max();
}

Channel::Channel(Server *server, std::string nameChannel, std::string password) : server(server), nameChannel(nameChannel), password(password) {
    this->havePassword = true;
    this->inviteOnly = false;
    this->haveMaxUsers = false;
    this->haveMaxUsers = false;
    this->maxUsers = std::numeric_limits<int>::max();
}

Channel::~Channel() {
    for (size_t i = 0; i < this->admins.size(); i++) {
        delete this->admins[i];
    }
    for (size_t i = 0; i < this->users.size(); i++) {
        delete this->users[i];
    }
}

std::string Channel::getNameChannel(void) {
    return this->nameChannel;
}

std::vector<std::string> Channel::getTopics(void) {
    return this->topics;
}

std::vector<Client *> Channel::getAdmins(void) {
    return this->admins;
}

std::vector<Client *> Channel::getUsers(void) {
    return this->users;
}

bool Channel::getHavePassword(void) {
    return this->havePassword;
}

std::string Channel::getPassWord(void) {
    return this->password;
}

bool Channel::getInviteOnly(void) {
    return this->inviteOnly;
}

bool Channel::getHaveMaxUsers(void) {
    return this->haveMaxUsers;
}

int Channel::getMaxUsers(void) {
    return this->maxUsers;
}

Server *Channel::getServer(void) {
    return this->server;
}

void Channel::addUser(Client *client) {
    // Check if the number of user is over the max number of user
    if (this->haveMaxUsers) {
        if (this->users.size() >= (size_t)this->maxUsers) {
            return;
        }
    }
    // Check if the user is already in the channel
    for (size_t i = 0; i < this->users.size(); i++) {
        if (this->users[i]->getNickName() == client->getNickName()) {
            return;
        }
    }
    this->users.push_back(client);
}

void Channel::kickUser(Client *client) {
    for (size_t i = 0; i < this->users.size(); i++) {
        if (this->users[i]->getNickName() == client->getNickName()) {
            this->users.erase(this->users.begin() + i);
            return;
        }
    }
}

void Channel::kickUser(Client *client, std::string reason) {
    this->server->sendData(client->getFD(), "KICK " + this->nameChannel + " " + client->getNickName() + " :" + reason + "\r\n");
    this->kickUser(client);
}

void Channel::invite(Client *src, Client *target) {
    this->server->sendData(target->getFD(), "INVITE " + src->getNickName() + " " + this->nameChannel + "\r\n");
}

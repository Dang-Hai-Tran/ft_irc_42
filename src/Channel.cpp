#include "../inc/irc.hpp"

Channel::Channel(Server *server, std::string nameChannel) : server(server), nameChannel(nameChannel) {
    this->havePassword = false;
    this->inviteOnly = false;
    this->haveMaxUsers = false;
    this->haveMaxUsers = false;
    this->maxUsers = std::numeric_limits<int>::max();
    this->topicRestrictions = true;
}

Channel::Channel(Server *server, std::string nameChannel, std::string password) : server(server), nameChannel(nameChannel), password(password) {
    this->havePassword = true;
    this->inviteOnly = false;
    this->haveMaxUsers = false;
    this->haveMaxUsers = false;
    this->maxUsers = std::numeric_limits<int>::max();
    this->topicRestrictions = true;
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

std::string Channel::getTopic(void) {
    return this->topic;
}

std::vector<Client *> &Channel::getAdmins(void) {
    return this->admins;
}

std::vector<Client *> &Channel::getUsers(void) {
    return this->users;
}

std::vector<Client *> &Channel::getInvited(void) {
    return this->invited;
}

bool Channel::getHavePassword(void) {
    return this->havePassword;
}

std::string Channel::getPassword(void) {
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

bool Channel::getTopicRestriction() {
    return this->topicRestrictions;
}

void Channel::setNameChannel(std::string nameChannel) {
    this->nameChannel = nameChannel;
}

void Channel::setTopic(std::string topic) {
    this->topic = topic;
}

void Channel::setHavePassword(bool havePassword) {
    this->havePassword = havePassword;
}

void Channel::setPassword(std::string password) {
    this->password = password;
}

void Channel::setInviteOnly(bool inviteOnly) {
    this->inviteOnly = inviteOnly;
}

void Channel::setHaveMaxUsers(bool haveMaxUsers) {
    this->haveMaxUsers = haveMaxUsers;
}

void Channel::setMaxUsers(int maxUsers) {
    this->maxUsers = maxUsers;
}

void Channel::setTopicRestriction(bool topicRestriction) {
    this->topicRestrictions = topicRestriction;
}
void Channel::addAdmins(Client *admin) {
    this->admins.push_back(admin);
}

void Channel::addAdmins(std::vector<Client *> admins) {
    this->admins.insert(this->admins.end(), admins.begin(), admins.end());
}

void Channel::delAdmin(Client *client) {
    for (size_t i = 0; i < this->admins.size(); i++) {
        if (this->getAdmins()[i]->m_getNickName() == client->m_getNickName()) {
            this->admins.erase(this->admins.begin() + i);
        }
        break;
    }
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
        if (this->users[i]->m_getNickName() == client->m_getNickName()) {
            return;
        }
    }
    this->users.push_back(client);
}

void Channel::delUser(Client *client) {
    for (size_t i = 0; i < this->users.size(); i++) {
        if (this->users[i]->m_getNickName() == client->m_getNickName()) {
            this->users.erase(this->users.begin() + i);
            return;
        }
    }
    // Delete from list admins if user is admin
    if (this->isAdmin(client) == true)
        this->delAdmin(client);
}

void Channel::kickUser(Client *client, std::string reason) {
    if (!reason.empty())
        this->sendMessageToAll("Kick " + client->m_getNickName() + " to channel " + this->nameChannel + " :" + reason + "\r\n");
    else
        this->sendMessageToAll("Kick " + client->m_getNickName() + " to channel " + this->nameChannel + "\r\n");
    this->delUser(client);
}

void Channel::invite(Client *src, Client *target) {
    this->server->sendData(target->m_getSocket(), src->m_getNickName() + " invite " + target->m_getNickName() + " join " + this->nameChannel + "\r\n");
    // Check if target is in list of invited users
    if (isInvited(target) == false)
        this->invited.push_back(target);
}

bool Channel::isAdmin(Client *client) {
    for (size_t i = 0; i < this->admins.size(); i++) {
        if (this->admins[i]->m_getNickName() == client->m_getNickName()) {
            return true;
        }
    }
    return false;
}

size_t Channel::getNumberUsers(void) {
    return this->users.size();
}

size_t Channel::getNumberAdmins(void) {
    return this->admins.size();
}

void Channel::sendMessageToAll(std::string message) {
    for (size_t i = 0; i < this->users.size(); i++) {
        this->server->sendData(this->users[i]->m_getSocket(), message + "\r\n");
    }
}

bool Channel::isInChannel(Client *client) {
    for (size_t i = 0; i < this->users.size(); i++) {
        if (this->users[i]->m_getNickName() == client->m_getNickName()) {
            return true;
        }
    }
    return false;
}

bool Channel::isInvited(Client *client) {
    for (size_t i = 0; i < this->invited.size(); i++) {
        if (this->invited[i]->m_getNickName() == client->m_getNickName()) {
            return true;
        }
    }
    return false;
}

Client *Channel::getUserByNickName(std::string nickName) {
    for (size_t i = 0; i < this->users.size(); i++) {
        if (this->users[i]->m_getNickName() == nickName) {
            return this->users[i];
        }
    }
    return NULL;
}

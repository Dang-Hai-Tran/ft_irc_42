#include "../inc/irc.hpp"

Channel::Channel(Server *server, std::string nameChannel, std::string password) : server(server), nameChannel(nameChannel), password(password) {
    if (password.empty()) {
        this->havePassword = false;
        this->mode = "";
    } else {
        this->havePassword = true;
        this->mode = "+k";
    }
    this->inviteOnly = false;
    this->haveMaxUsers = false;
    this->haveMaxUsers = false;
    this->maxUsers = std::numeric_limits<int>::max();
    this->topicRestrictions = false;
}

Channel::~Channel() {
    this->users.clear();
    this->admins.clear();
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
void Channel::addAdmins(Client *user) {
    if (!this->isAdmin(user))
        this->admins.push_back(user);
}

void Channel::delAdmin(Client *client) {
    for (size_t i = 0; i < this->admins.size(); i++) {
        if (this->getAdmins()[i] == client) {
            this->admins.erase(this->admins.begin() + i);
        }
        break;
    }
}

void Channel::addUser(Client *client) {
    this->users.push_back(client);
}

void Channel::delUser(Client *client) {
    for (size_t i = 0; i < this->users.size(); i++) {
        if (this->users[i] == client) {
            this->users.erase(this->users.begin() + i);
            return;
        }
    }
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
        this->server->sendData(this->users[i], message);
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

std::string Channel::getListOfMembers(void) {
    std::vector<Client *> users = this->getUsers();
    std::string strList = "";
    for (size_t i = 0; i < users.size(); i++) {
        if (this->isAdmin(users[i]))
            strList += "@";
        strList += users[i]->m_getNickName();
        if (i != users.size() - 1)
            strList += " ";
    }
    return strList;
}

void Channel::addToInvited(Client *client) {
    if (!this->isInvited(client)) {
        this->invited.push_back(client);
    }
}

void Channel::delFromInvited(Client *client) {
    for (size_t i = 0; i < this->invited.size(); i++) {
        if (client == this->invited[i]) {
            this->invited.erase(this->invited.begin() + i);
        }
    }
}

std::string &Channel::getMode(void) {
    return this->mode;
}

void Channel::addMode(std::string mode) {
    if (this->mode.empty()) {
        this->mode += "+";
    }
    this->mode += mode;
}

void Channel::delMode(std::string mode) {
    size_t pos = this->mode.find(mode);
    if (pos != std::string::npos) {
        this->mode.erase(pos, 1);
    }
}

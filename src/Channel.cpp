#include "../inc/irc.hpp"

/**
 * @brief Constructor for the Channel class.
 * @param server A pointer to the IRC server.
 * @param nameChannel The name of the channel.
 * @param password The password for the channel (optional).
 */
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

/**
 * @brief Destructor for the Channel class.
 */
Channel::~Channel() {
    this->users.clear();
    this->admins.clear();
}

/**
 * @brief Get the name of the channel.
 * @return The name of the channel.
 */
std::string Channel::getNameChannel(void) {
    return this->nameChannel;
}

/**
 * @brief Get the topic of the channel.
 * @return The topic of the channel.
 */
std::string Channel::getTopic(void) {
    return this->topic;
}

/**
 * @brief Get the list of admins in the channel.
 * @return A reference to the vector of admins.
 */
std::vector<Client *> &Channel::getAdmins(void) {
    return this->admins;
}

/**
 * @brief Get the list of users in the channel.
 * @return A reference to the vector of users.
 */
std::vector<Client *> &Channel::getUsers(void) {
    return this->users;
}

/**
 * @brief Get the list of invited users in the channel.
 * @return A reference to the vector of invited users.
 */
std::vector<Client *> &Channel::getInvited(void) {
    return this->invited;
}

/**
 * @brief Check if the channel has a password.
 * @return True if the channel has a password, false otherwise.
 */
bool Channel::getHavePassword(void) {
    return this->havePassword;
}

/**
 * @brief Get the password of the channel.
 * @return The password of the channel.
 */
std::string Channel::getPassword(void) {
    return this->password;
}

/**
 * @brief Check if the channel is invite-only.
 * @return True if the channel is invite-only, false otherwise.
 */
bool Channel::getInviteOnly(void) {
    return this->inviteOnly;
}

/**
 * @brief Check if the channel has a maximum number of users.
 * @return True if the channel has a maximum number of users, false otherwise.
 */
bool Channel::getHaveMaxUsers(void) {
    return this->haveMaxUsers;
}

/**
 * @brief Get the maximum number of users allowed in the channel.
 * @return The maximum number of users allowed in the channel.
 */
int Channel::getMaxUsers(void) {
    return this->maxUsers;
}

/**
 * @brief Get a pointer to the IRC server.
 * @return A pointer to the IRC server.
 */
Server *Channel::getServer(void) {
    return this->server;
}

/**
 * @brief Check if the channel has topic restrictions.
 * @return True if the channel has topic restrictions, false otherwise.
 */
bool Channel::getTopicRestriction() {
    return this->topicRestrictions;
}

/**
 * @brief Set the name of the channel.
 * @param nameChannel The new name of the channel.
 */
void Channel::setNameChannel(std::string nameChannel) {
    this->nameChannel = nameChannel;
}

/**
 * @brief Set the topic of the channel.
 * @param topic The new topic of the channel.
 */
void Channel::setTopic(std::string topic) {
    this->topic = topic;
}

/**
 * @brief Set whether the channel has a password.
 * @param havePassword True if the channel has a password, false otherwise.
 */
void Channel::setHavePassword(bool havePassword) {
    this->havePassword = havePassword;
}

/**
 * @brief Set the password of the channel.
 * @param password The new password of the channel.
 */
void Channel::setPassword(std::string password) {
    this->password = password;
}

/**
 * @brief Set whether the channel is invite-only.
 * @param inviteOnly True if the channel is invite-only, false otherwise.
 */
void Channel::setInviteOnly(bool inviteOnly) {
    this->inviteOnly = inviteOnly;
}

/**
 * @brief Set whether the channel has a maximum number of users.
 * @param haveMaxUsers True if the channel has a maximum number of users, false otherwise.
 */
void Channel::setHaveMaxUsers(bool haveMaxUsers) {
    this->haveMaxUsers = haveMaxUsers;
}

/**
 * @brief Set the maximum number of users allowed in the channel.
 * @param maxUsers The new maximum number of users allowed in the channel.
 */
void Channel::setMaxUsers(int maxUsers) {
    this->maxUsers = maxUsers;
}

/**
 * @brief Set whether the channel has topic restrictions.
 * @param topicRestriction True if the channel has topic restrictions, false otherwise.
 */
void Channel::setTopicRestriction(bool topicRestriction) {
    this->topicRestrictions = topicRestriction;
}

/**
 * @brief Add a user as an admin of the channel.
 * @param user A pointer to the user to be added as an admin.
 */
void Channel::addAdmins(Client *user) {
    if (!this->isAdmin(user))
        this->admins.push_back(user);
}

/**
 * @brief Remove an admin from the channel.
 * @param client A pointer to the admin to be removed.
 */
void Channel::delAdmin(Client *client) {
    for (size_t i = 0; i < this->admins.size(); i++) {
        if (this->getAdmins()[i] == client) {
            this->admins.erase(this->admins.begin() + i);
        }
        break;
    }
}

/**
 * @brief Add a user to the channel.
 * @param client A pointer to the user to be added.
 */
void Channel::addUser(Client *client) {
    this->users.push_back(client);
}

/**
 * @brief Remove a user from the channel.
 * @param client A pointer to the user to be removed.
 */
void Channel::delUser(Client *client) {
    for (size_t i = 0; i < this->users.size(); i++) {
        if (this->users[i] == client) {
            this->users.erase(this->users.begin() + i);
            return;
        }
    }
}

/**
 * @brief Check if a user is an admin of the channel.
 * @param client A pointer to the user to be checked.
 * @return True if the user is an admin of the channel, false otherwise.
 */
bool Channel::isAdmin(Client *client) {
    for (size_t i = 0; i < this->admins.size(); i++) {
        if (this->admins[i]->m_getNickName() == client->m_getNickName()) {
            return true;
        }
    }
    return false;
}

/**
 * @brief Get the number of users in the channel.
 * @return The number of users in the channel.
 */
size_t Channel::getNumberUsers(void) {
    return this->users.size();
}

/**
 * @brief Get the number of admins in the channel.
 * @return The number of admins in the channel.
 */
size_t Channel::getNumberAdmins(void) {
    return this->admins.size();
}

/**
 * @brief Send a message to all users in the channel.
 * @param message The message to be sent.
 */
void Channel::sendMessageToAll(std::string message) {
    for (size_t i = 0; i < this->users.size(); i++) {
        this->server->sendData(this->users[i], message);
    }
}

/**
 * @brief Check if a user is in the channel.
 * @param client A pointer to the user to be checked.
 * @return True if the user is in the channel, false otherwise.
 */
bool Channel::isInChannel(Client *client) {
    for (size_t i = 0; i < this->users.size(); i++) {
        if (this->users[i]->m_getNickName() == client->m_getNickName()) {
            return true;
        }
    }
    return false;
}

/**
 * @brief Check if a user is invited to the channel.
 * @param client A pointer to the user to be checked.
 * @return True if the user is invited to the channel, false otherwise.
 */
bool Channel::isInvited(Client *client) {
    for (size_t i = 0; i < this->invited.size(); i++) {
        if (this->invited[i]->m_getNickName() == client->m_getNickName()) {
            return true;
        }
    }
    return false;
}

/**
 * @brief Get a user in the channel by their nickname.
 * @param nickName The nickname of the user.
 * @return A pointer to the user with the specified nickname, or NULL if not found.
 */
Client *Channel::getUserByNickName(std::string nickName) {
    for (size_t i = 0; i < this->users.size(); i++) {
        if (this->users[i]->m_getNickName() == nickName) {
            return this->users[i];
        }
    }
    return NULL;
}

/**
 * @brief Get a list of members in the channel.
 * @return A string containing the list of members.
 */
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

/**
 * @brief Add a user to the list of invited users.
 * @param client A pointer to the user to be added.
 */
void Channel::addToInvited(Client *client) {
    if (!this->isInvited(client)) {
        this->invited.push_back(client);
    }
}

/**
 * @brief Remove a user from the list of invited users.
 * @param client A pointer to the user to be removed.
 */
void Channel::delFromInvited(Client *client) {
    for (size_t i = 0; i < this->invited.size(); i++) {
        if (client == this->invited[i]) {
            this->invited.erase(this->invited.begin() + i);
        }
    }
}

/**
 * @brief Get the mode of the channel.
 * @return A reference to the mode of the channel.
 */
std::string &Channel::getMode(void) {
    return this->mode;
}

/**
 * @brief Add a mode to the channel.
 * @param mode The mode to be added.
 */
void Channel::addMode(std::string mode) {
    if (this->mode.empty()) {
        this->mode += "+";
    }
    this->mode += mode;
}

/**
 * @brief Remove a mode from the channel.
 * @param mode The mode to be removed.
 */
void Channel::delMode(std::string mode) {
    size_t pos = this->mode.find(mode);
    if (pos != std::string::npos) {
        this->mode.erase(pos, 1);
    }
}

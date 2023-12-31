/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_client.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: datran <datran@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 19:00:31 by xuluu             #+#    #+#             */
/*   Updated: 2023/12/06 14:48:30 by datran           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/irc.hpp"

Client::Client(void) {
    this->m_socket = 0;
    this->m_id = 0;

    this->m_nickName = "";
    this->m_userName = "";
    this->m_realName = "";

    this->m_input = "";
    this->m_cmd = "";
    this->m_parameter = "";

    this->m_irssi = false;
    this->m_connected = false;
    this->m_statusS = false;
    this->m_statusC = false;

    this->m_invisible = true;
    this->m_adminServer = false;
    this->m_adminChannel = false;
}

Client::Client(Client &objet) {
    if (this == &objet)
        return;
    *this = objet;
}

Client::~Client(void) {
    this->channels.clear();
}

Client &Client::operator=(const Client &objet) {
    if (this != &objet) {
        this->m_id = objet.m_getID();
        this->m_nickName = objet.m_getNickName();
        this->m_userName = objet.m_getUserName();
        this->m_realName = objet.m_getRealName();
        this->m_invisible = objet.m_isInvisible();
        this->m_adminServer = objet.m_getAdminServer();
        this->m_adminChannel = objet.m_getAdminChannel();
    }
    return *this;
}

/*************************************************************************/

/* Nickname */
void Client::m_setNickName( const std::string &nn) {
    this->m_nickName = nn;
}

std::string Client::m_getNickName(void) const {
    return (this->m_nickName);
}

/* User name */
void Client::m_setUserName(const std::string un) {
    this->m_userName = un;
}

std::string Client::m_getUserName(void) const {
    return (this->m_userName);
}

/* Real name */
void Client::m_setRealName(const std::string rn) {
    this->m_realName = rn;
}

std::string Client::m_getRealName(void) const {
    return (this->m_realName);
}

/* Socket */
void Client::m_setSocket(int sk) {
    this->m_socket = sk;
}

int Client::m_getSocket(void) const {
    return (this->m_socket);
}

/* Input */
void Client::m_setInput(std::string input) {
    this->m_input = input;
}

std::string Client::m_getInput(void) const {
    return (this->m_input);
}

/* Command + Parameter */
void Client::m_setCmd(std::string cmd) {
    this->m_cmd = cmd;
}

std::string Client::m_getCmd(void) const {
    return (this->m_cmd);
}

void Client::m_setParameter(std::string parameter) {
    this->m_parameter = parameter;
}

std::string Client::m_getParameter(void) const {
    return (this->m_parameter);
}

/* ID */
void Client::m_setID(int id) {
    this->m_id = id;
}

int Client::m_getID(void) const {
    return (this->m_id);
}

/* Admin server */
void Client::m_setAdminServer(bool status) {
    this->m_adminServer = status;
}

bool Client::m_getAdminServer(void) const {
    return (this->m_adminServer);
}

/* Admin channel */
void Client::m_setAdminChannel(bool status) {
    this->m_adminChannel = status;
}

bool Client::m_getAdminChannel(void) const {
    return (this->m_adminChannel);
}

/* Status on server */
void Client::m_setStatusS(bool status) {
    this->m_statusS = status;
}

bool Client::m_getStatusS(void) const {
    return (this->m_statusS);
}

/* Status on channel */
void Client::m_setStatusC(bool status) {
    this->m_statusC = status;
}

bool Client::m_getStatusC(void) const {
    return (this->m_statusC);
}

/* Modes */
void Client::m_setMode(bool mode) {
    this->m_invisible = mode;
}

bool Client::m_isInvisible(void) const {
    return (this->m_invisible);
}

/* Connected */
void Client::m_setConnected(bool status) {
    this->m_connected = status;
}

bool Client::m_isConnected(void) const {
    return (this->m_connected);
}

/************************************************/
void Client::m_setModeClient(bool mode) {
    this->m_irssi = mode;
}

bool Client::m_usingIrssi(void) const {
    return (this->m_irssi);
}

std::vector<Channel *> &Client::getChannelsUserIn(void) {
    return this->channels;
}

void Client::addChannel(Channel *channel) {
    this->getChannelsUserIn().push_back(channel);
}

void Client::delChannel(Channel *channel) {
    for (size_t i = 0; i < this->getChannelsUserIn().size(); i++) {
        if (this->getChannelsUserIn()[i] == channel) {
            this->getChannelsUserIn().erase(this->getChannelsUserIn().begin() + i);
            break;
        }
    }
}

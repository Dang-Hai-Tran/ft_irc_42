#ifndef UTILS_HPP
#define UTILS_HPP

#include "irc.hpp"

bool strContainOnlyDigits(std::string str);
std::string getCurrentTime(void);
std::string getIpAddressFromSockaddr(const struct sockaddr_in6 *clientAddress);
std::vector<std::string> splitString(std::string str, char c);
int getPortFromSocket(int fd);
void trimEndOfLine(std::string &str);
void commandChannel(Server &server, Client &client);
void commandJoin(Server *server, Client *client, std::string input);
void commandPart(Server *server, Client *client, std::string input);
void commandTopic(Server *server, Client *client, std::string input);
void commandNames(Server *server, Client *client, std::string input);
void commandInvite(Server *server, Client *client, std::string input);
void commandKick(Server *server, Client *client, std::string input);
void commandMode(Server *server, Client *client, std::string input);
void commandBot(Server *server, Client *client, std::string input);
void commandTransfer(Server *server, Client *client, std::string input);

std::string cmdFindMessage(std::string input);
std::vector<std::string> cmdFindArgs(std::string input);
std::vector<std::string> parseModeString(std::string mode);

// Print vector of elements type T
template <typename T>
std::ostream &operator<<(std::ostream &os, const std::vector<T> &vec) {
    for (size_t i = 0; i < vec.size(); ++i) {
        os << "[" << vec[i] << "]";
    }
    return os;
}

// utiles
void ft_send(Client *client, std::string text);
void error_syntax(Client *client);
int ft_nbrSpace(const std::string &str);
bool ft_isStringEmpty(const std::string &str);
std::string upperCase(std::string &str);
std::string ft_delete_space(const std::string &input);
std::string str_without_space_start_end(const std::string &arg);
std::string int_to_string(int nbr);

// find
int ft_find_username(Server &server, std::string &userName);
int ft_find_nickname(Server &server, std::string &nickName);

// request
bool ft_requestPassword(Server &server, Client *client);
bool ft_requestNickName(Server &server, Client *client);
bool ft_requestUserName(Server &server, Client *&client);

// get input
void get_input(Server &server, Client *&client);
bool get_command_parameter(Server &server, Client *&client);
void ft_command_outside(Server &server, Client *&client);

bool is_adminServer(Client *client);

// run commands
bool ft_run(Server &server, Client *&client);
void ft_guide(Client *client);
void reset_data(Client *client);

// command HELP
void ft_command_help(Client *client);

// command PASSWORD
void ft_command_pass(Server &server, Client *client);

// command NICK
void ft_command_nick(Server &server, Client *client);

// command USER
void ft_command_user(Server &server, Client *&client);
void connected_successfully(Server &server, Client *&client);
void ft_irssi_get_realname(Server &server, Client *&client);
bool ft_check_username(Server &server, Client *&client, std::string userName);

// command WHO
void ft_command_who(Server &server, Client *client);

// command WHOIS
void ft_command_whois(Server &server, Client *client);

// command PRIVMSG
void ft_command_privmsg(Server &server, Client *client);
void ft_conversation(Client *client, Channel *channel);
void ft_message(Client *client);

// command CLEAR
void ft_command_clear(Client *client);

void copy_data(Server *server, Client *client);

#endif

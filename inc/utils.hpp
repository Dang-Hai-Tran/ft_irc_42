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

// utiles
void	ft_send(Client* client, int type, std::string output);
void	error_syntax(Client* client);
int		ft_nbrSpace(const std::string& str);
bool	ft_isStringEmpty(const std::string& str);
bool	is_upperCase(std::string& str);
std::string	ft_delete_space(const std::string& input);
std::string	str_without_space_start_end(const std::string& arg);
std::string int_to_string(int nbr);

// find
int	ft_find_username(Server& server, std::string& userName);
int	ft_find_nickname(Server& server, std::string& nickName);

// request
void	ft_requestPassword(Server& server, Client* client);
void	ft_requestUserName(Server& server, Client* &client);
void	ft_requestNickName(Server& server, Client* client);

// get input
void	get_input(Server& server, Client* client);
void	get_command_parameter(Server& server, Client* client);
void	ft_command_outside(Server& server, Client* client);

bool	is_adminServer(Client* client);

// run commands
void	ft_run(Server& server, Client* client);
void	ft_guide(Client* client);
void	reset_data(Client* client);

// command HELP
void	ft_command_help(Client* client);

// command PASSWORD
void	ft_command_password(Server& server, Client* client);

// command LOGIN
void	ft_command_login(Server& server, Client* &client);

// command NICK
void	show_nickname(Server& server);
void	ft_command_nickname(Server& server, Client* client);

// command USER
void	show_username(Server& server);
void	ft_command_username(Server& server, Client* client);

// command WHO
void	ft_command_who(Server& server, Client* client);

// command WHOIS
void	ft_command_whois(Server& server, Client* client);

// command PRIVMSG
void	ft_command_privmsg(Server& server, Client* client);

#endif

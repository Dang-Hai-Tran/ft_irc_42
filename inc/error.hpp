#ifndef ERROR_HPP
#define ERROR_HPP

#include "irc.hpp"

#define ERR_NEEDMOREPARAMS(client, command) "461 " + client + " " + command + " :Not enough parameters"
#define ERR_NOSUCHCHANNEL(client, channel) "403 " + src + " " + channel + " :No such channel"

#endif

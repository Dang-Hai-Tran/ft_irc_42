#ifndef IRC_HPP
#define IRC_HPP

#include <algorithm>
#include <arpa/inet.h>
#include <cstdlib>
#include <cstring>
#include <errno.h>
#include <fcntl.h>
#include <iostream>
#include <netinet/in.h>
#include <signal.h>
#include <stdexcept>
#include <string>
#include <sys/poll.h>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>
#include <limits>

#include "Channel.hpp"
#include "Client.hpp"
#include "Server.hpp"
#include "utils.hpp"

#define DEFAULT_PORT 8888
#define BUFFER_SIZE 2
#ifndef DEBUG
# define DEBUG 0
#endif

#endif

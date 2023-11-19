# Program name
NAME	= ircserv

# Compiler
CC		= c++
CFLAGS	= -Werror -Wextra -Wall -g -std=c++98

# Build files
INC_PATH	= ./inc/
INC_FLAG	= -I $(INC_PATH)

SRC_PATH	= ./src/
SRC			= $(wildcard $(SRC_PATH)*.cpp)
OBJ_PATH	= ./obj/
OBJ			= $(SRC:$(SRC_PATH)%.cpp=$(OBJ_PATH)%.o)

# Debug
DEBUG = 0
DEBUG_FLAG = -D DEBUG=$(DEBUG)

# Build rule
all: $(OBJ_PATH) $(NAME)

debugflag:
	$(eval DEBUG=1)

debug: debugflag all

# Object-building rules
$(OBJ_PATH):
	mkdir -p $(OBJ_PATH)

$(OBJ_PATH)%.o: $(SRC_PATH)%.cpp
	$(CC) $(CFLAGS) -c $< -o $@ $(INC_FLAG) $(DEBUG_FLAG)

# Program-building rule
$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $@ $(INC_FLAG) $(DEBUG_FLAG)

# Cleaning rules
clean:
	rm -rf $(OBJ_PATH)

fclean: clean
	rm -f $(NAME)

# Remake
re: fclean all

.PHONY: all re clean fclean

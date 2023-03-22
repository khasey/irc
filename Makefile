INC_DIR := inc/
SRC_DIR	:= srcs/
CLASS_DIR := class/
COM_DIR := com/
OBJ_DIR	:= obj/

OBJ_DIRS = $(OBJ_DIR)

GREEN = '\033[92m'
RED = '\033[91m'
NONE = '\033[3m'

CLASS_SRCS = Socket Client Server Channel

COM_SRCS = com messages poll welcome 

PRE_SRCS =	main init connexion \
			$(addprefix $(CLASS_DIR), $(CLASS_SRCS)) \
			$(addprefix $(COM_DIR), $(COM_SRCS))

SRCS = $(addsuffix .cpp, $(addprefix $(SRC_DIR), $(PRE_SRCS)))
OBJS = $(addsuffix .o, $(addprefix $(OBJ_DIR), $(PRE_SRCS)))

NAME := ircserv

CC := clang++
CFLAGS := -Wall -Wextra -Werror -std=c++98
INC	:= -I./$(INC_DIR) -I./$(INC_DIR)Messages/
RM = rm -f

all:	$(NAME)
	echo $(GREEN)"----- | --- --- --- --- USAGE --- --- --- --- | -----"$(NONE)
	echo $(GREEN)"----- | ./ircserv     6667     <set password> | -----"$(NONE)
	echo $(GREEN)"----- | nc 127.0.0.1  6667                    | -----"$(NONE)
	echo $(GREEN)"----- | --- --- --- --- ------ --- --- --- ---| -----"$(NONE)

$(NAME): $(OBJ_DIRS) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME) 
	echo $(GREEN)"-----  $@ has been created  -----" $(NONE)


$(OBJ_DIRS):
	mkdir -p $(OBJ_DIRS)
	mkdir -p $(OBJ_DIRS)class
	mkdir -p $(OBJ_DIRS)com

$(OBJS) : $(INC_DIR)

$(OBJ_DIR)%.o: $(SRC_DIR)%.cpp $(wildcard $(INC_DIR)*.hpp)
	$(CC) $(CFLAGS) $(INC) -c $< -o $@
	echo "\033[0m$@ \033[32mcreated\033[0m"

clean:
	echo $(RED) "-- Deleting-- " $(NONE)
	sleep 0.2
	rm -rf $(OBJ_DIR)
	echo $(RED) "Objects was deleted" $(NONE)

fclean: 	clean
	echo $(RED) "Deleting --$(NAME)-- " $(NONE)
	sleep 0.2
	rm -rf $(NAME)
	echo $(RED) "$(NAME) was deleted !" $(NONE)

re:		fclean all

.PHONY : all clean fclean re
.SILENT :
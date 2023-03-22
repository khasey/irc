#include "../../inc/irc.hpp"
#include <sys/ioctl.h>
#define NBCMD 11

static int	checkRegister(int fd, int msg, Server &s);
static void	init_arr_funct(void (*arr[NBCMD])(Server &s, int fd, std::string str)); // init array for messages
static void	manageMsg(std::string input, Server &s, int fd);

int msg(int fd, Server &s){
	unsigned long dsize = 0;
	std::string input;
	int rc;

	rc =ioctl(fd, FIONREAD, &dsize);
	if (rc == ERROR){
		perror("\e[31mioctl failed \e[0m ");
		return (-1);
	}
	char* buffer = (char*)malloc(sizeof(char) * dsize);
	if (recv(fd, buffer, dsize, 0) == ERROR){
		perror("\e[31mconnection issue\e[0m ");
		return FAIL;
	}
	input += std::string(buffer, dsize);
	free(buffer);
	std::cout << "Received from fd " << fd << ": " << input; //Display msg
	manageMsg(input, s, fd);
	return SUCCESS;
}

//	
//	--- --- statics functions --- ---
//

static void	manageMsg(std::string input, Server &s, int fd){
	void		(*arr[NBCMD])(Server &s, int fd, std::string str);
	std::string	firstWord;
	const int	msgsNb = NBCMD;
	int			msg;
	int			reg;
	std::string msgs[msgsNb] = {"PASS", "NICK", "USER", "JOIN", "NAMES", "LIST", "PRIVMSG", "NOTICE", "PING", "KICK", "PART"};

	input = input.substr(0, input.find("\n")).substr(0, input.find("\r"));
	firstWord = input.substr(0, input.find(" "));
	init_arr_funct(arr);
	for(msg = 0; msg < msgsNb; msg++){
		if (msgs[msg] == firstWord)
			break ;
	}
	reg = checkRegister(fd, msg, s);
	if (reg && msg != msgsNb)
		arr[msg](s, fd, input.substr(msgs[msg].length(), 201));
	else if (!reg)
		return;
	else
		send(fd, ERR_UNKNOWNCOMMAND, sizeof(ERR_UNKNOWNCOMMAND), 0);
}

static void	init_arr_funct(void (*arr[NBCMD])(Server &s, int fd, std::string str)){
	arr[E_PASS] = pass;
	arr[E_NICK] = nick;
	arr[E_USER] = user;
	arr[E_JOIN] = join;
	arr[E_NAMES] = names;
	arr[E_LIST] = list;
	arr[E_PRIVMSG] = privmsg;
	arr[E_NOTICE] = notice;
	arr[E_PING] = ign;
	arr[E_KICK] = kick;
	arr[E_PART] = part;
}
static int	checkRegister(int fd, int msg, Server &s){
	if (msg == E_PASS || msg == E_NICK || msg == E_USER)
		return 1;
	if (!s.getClients(fd)->getRegister()){
		send(fd, ERR_NOTREGISTERED, sizeof(ERR_NOTREGISTERED), 0);
		return 0;
	}
	return 1;
}

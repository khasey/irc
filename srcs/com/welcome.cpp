#include "../../inc/irc.hpp"

//rfc2812 : The server sends Replies 001 to 004 to a user upon successful registration.
void	welcomeMsg(int fd, Server &s, Client &c){
	t_data dS;

	dS = s.getData();
	// 001 RPL_WELCOME
	send(fd, "001    RPL_WELCOME ", strlen("001    RPL_WELCOME "), 0);
	send(fd, c.getNickname().c_str(), c.getNickname().length(), 0);
	send(fd, " :Welcome to the Internet Relay Network ", strlen(" :Welcome to the Internet Relay Network "), 0);
	send(fd, c.getID().c_str(), c.getID().length(), 0);
	send(fd, "\r\n",strlen("\r\n") , 0);

	// 002    RPL_YOURHOST
	send(fd,"002    RPL_YOURHOST ", strlen("002    RPL_YOURHOST "),0);
    send(fd,":Your host is ", strlen(":Your host is "), 0);
	send(fd, dS.name.c_str(), dS.name.length(), 0);
	send(fd, ", running version ", strlen(", running version "), 0);
	send(fd, dS.version.c_str(), dS.version.length(), 0);
	send(fd, "\r\n", 2, 0);

    // 003    RPL_CREATED
	send(fd,"003    RPL_CREATED ", strlen("003    RPL_CREATED "),0);
	send(fd, ":This server was created ", strlen(":This server was created "), 0);
	send(fd, dS.dateCreat.c_str(), dS.dateCreat.length(), 0);
	send(fd, "\r\n", 2, 0);


    // 004    RPL_MYINFO
	send(fd,"004    RPL_MYINFO ", strlen("004    RPL_MYINFO "),0);
	send(fd, dS.name.c_str(), dS.name.length(), 0);
	send(fd, " ", 1, 0);
	send(fd, dS.version.c_str(), dS.version.length(), 0);
	send(fd, " ", 1, 0);
	send(fd, dS.userMode.c_str(), dS.userMode.length(), 0);
	send(fd, " ", 1, 0);
	send(fd, dS.chanMode.c_str(), dS.chanMode.length(), 0);
	send(fd, "\r\n", 2, 0);
}
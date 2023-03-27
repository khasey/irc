#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <string>
#include <string.h>
#include <sys/poll.h>
#include <map>
#include<list>

#include "Client.hpp"
#include "Channel.hpp"
#include "Socket.hpp"

class	Client;
class	Socket;
class	Channel;

typedef	struct s_data{
	std::string	name;
	std::string	version;
	std::string	dateCreat;
	std::string	userMode;
	std::string	chanMode;
}	t_data;

class Server{

	t_data										d;
	Socket const 								&sock;
	std::string									password;
	std::map<int, Client*>						clientList;
	std::map<Channel *, std::list<Client*>* >	channelList;

public:

	int					nfds;
	struct pollfd		fds[200];
	Server(Socket &b, std::string pwd);
	~Server();
	Server (Server const &);
	Server & operator=(Server const &);

	void	addChannel(Channel * chan);
	void	addChannelUser(std::string chan, Client *c);
	void	rmChannelUser(std::string chan, Client *c);
	void	promotChannelUser(std::string chan, Client *c);
	void	getChannel(const std::string& channelName) const;
	void	listChannel(int fd);
	void	addClient(int fd, Client *);
	void	delClient(std::string username);
	void	displayClients() const;
	bool	clientOnChan(std::string client, std::string chan);

	std::list<Client *> *getNames(std::string chanName) const;
	std::map<Channel *, std::list<Client*>* > getChanList() const;

	t_data				getData() const;
	Client	*			getClients(int fd) const;
	Client	*			getClients(std::string nickname) const;
	Client	*			getClientsUser(std::string user) const;
	Socket const &		getSocket() const;
	std::string const &	getPwd() const;
};

std::ostream & operator<<(std::ostream &, Server const &);

#endif
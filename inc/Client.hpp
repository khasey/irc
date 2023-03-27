#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <string>
#include <netinet/in.h> // struct sockaddr_in
#include <unistd.h>		//close
#include <list>
#include "Server.hpp"

class Server;

class Client{

	int			clientSocket;	//ret of accept
    sockaddr_in	addrClient;		//needed for accept
	socklen_t	addrClientSize;	//neede for accept

	bool		pass;			// 0 need the correct password ; 1 ok, can register
	bool		sRegister;		// NICK USER cmd needed for the server
	std::string	nickname;
	std::string	username;
	std::string	host;
	std::string	idFormMsg;
	std::list<std::string> chanRights; //superuser chan lst


public:
	Client();
	Client(int clientSocket, sockaddr_in addrClient, socklen_t addrClientSize);
	~Client();
	Client (Client const &);
	Client & operator=(Client const &);

	//Functions
	void	promot(std::string chan);
	void	demot(std::string chan);
	void	addChanRights(const std::string& channelName, const std::string& rights);
	void	promotUser(Server &s, Client &c, std::string data0, std::string data1);
	
	//setters
	void	setClientSocket(int v);
	void	setAddrClient(sockaddr_in v);
	void	setAddrClientSize(socklen_t v);

	void	setPass(bool v);
	void	setRegister(bool v);
	void	setNickname(std::string v);
	void	setUsername(std::string v);

	// getters
	int				getClientSocket() const;
	sockaddr_in		getAddrClient() const;
	socklen_t		getAddrClientSize() const;

	bool			getPass() const;
	bool			getRegister() const;
	std::string		getNickname() const;
	std::string		getUsername() const;
	std::string		getHost() const;
	std::string		getID() const;
	std::list<std::string> getChanRights() const;
	
};

#endif
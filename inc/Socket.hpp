#ifndef Socket_HPP
#define Socket_HPP

#include <iostream>
#include <string>

#include <netinet/in.h> // struct sockaddr_in
#include <sys/types.h>	//sockets
#include <sys/socket.h>	//sockets

class Socket{

	struct sockaddr_in	addr;
	int					sockFd;

public:
	Socket();
	~Socket();
	Socket (Socket const &);
	Socket & operator=(Socket const &);

	void	setAddr(struct sockaddr_in v);
	void	setSockFd(int v);

	struct sockaddr_in	getAddr() const;
	int					getSockFd() const;
};

#endif
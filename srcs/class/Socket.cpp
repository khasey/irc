#include "../../inc/Socket.hpp"

//	---	---	---	Construcor - Destructor --- --- ---
Socket::Socket() {}
Socket::~Socket() {}

//	---	---	---	Setters --- --- ---
void	Socket::setAddr(struct sockaddr_in v){
	addr = v;
}
void	Socket::setSockFd(int v){
	sockFd = v;
}

// --- --- --- Getters --- --- ---

struct sockaddr_in Socket::getAddr() const{
	return addr;
}
int Socket::getSockFd() const{
	return sockFd;
}

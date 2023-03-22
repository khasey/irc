#include "../inc/irc.hpp"

static void	errorCheck();	// check accept() return

Client *connectingClient(Socket const &b){	// ---	accept a connexion --
	int sockfd = b.getSockFd();

    sockaddr_in addrClient;
	socklen_t addrClientSize;
	int rc;

	addrClientSize = sizeof(addrClient);
	rc = accept(sockfd, (struct sockaddr *)&addrClient, &addrClientSize);
	if (rc == ERROR)
		errorCheck();
	return new Client(rc, addrClient, addrClientSize);
}

//	
//	--- --- statics functions --- ---
//

static void	errorCheck(){
	if (errno != EWOULDBLOCK){
		perror("  accept() failed");
		std::cerr << "\e[31mfailed to connect a new client\n\e[0m";
		throw MyException();
	}
}

#include "../../inc/irc.hpp"

static void	addClient(Server &s);
static void closeFd_RearrangePoll(Server &s, int i); // close fd client left + re sort the fd list

void pollLoop(Socket &b, std::string pw){
	int rc;
	int end_server = FALSE;
	int current_size = 0, i;
	Server s(b, pw);

	while (end_server == FALSE){
		rc = poll(s.fds, s.nfds, -1);
		if (rc < 0){
			perror("poll function failed ");
			break;
		}
		current_size = s.nfds;
		for (i = 0; i < current_size; i++){
			if (s.fds[i].revents == 0)
				continue;
			if (s.fds[i].fd == b.getSockFd()){
				addClient(s);
			}else{
				if (s.fds[i].revents & POLLHUP || s.fds[i].revents & POLLERR || s.fds[i].revents & POLLNVAL){
					std::cout << "\e[33mThe client was disconnected\n\e[0m";
					delete s.getClients(i);
					closeFd_RearrangePoll(s, i);
				}else if (s.fds[i].revents & POLLIN)
					msg(s.fds[i].fd, s);
			}
		}
	}
}

//	
//	--- --- statics functions --- ---
//

static void	addClient(Server &s){
	int new_sd = -1;
	Client *c;
	
	do{
		try{
			c = connectingClient(s.getSocket());
			new_sd = c->getClientSocket();
			if (new_sd < 0)
				continue;
			std::cout << "  New incoming connection - " << new_sd << std::endl;
			s.addClient(new_sd, c);
			std::cout << "\e[36mnew client !\n\e[0m";
			s.fds[s.nfds].fd = new_sd;
			s.fds[s.nfds].events = POLLIN;
			s.nfds++;
		}catch (const MyException &e){
			std::cout << e.what();
			new_sd = ERROR;
		}
	}while (new_sd != ERROR);
}

static void closeFd_RearrangePoll(Server &s, int i){
	int j;

	close(s.fds[i].fd);
	s.fds[i].fd = -1;
	for (i = 0; i < s.nfds; i++){
		if (s.fds[i].fd == -1){
			for (j = i; j < s.nfds; j++){
				s.fds[j].fd = s.fds[j + 1].fd;
			}
			i--;
			s.nfds--;
		}
	}
}
